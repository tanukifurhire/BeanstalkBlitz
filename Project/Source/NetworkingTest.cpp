/* Start Header ************************************************************************/
/*!
\file		NetworkingTest.cpp
\author		Tai Xiao Xuan
\par		xiaoxuan.tai@digipen.edu
\date		09/03/2025
\brief		This file implements functions for everything Networking-related, for Multiplayer
			purposes. This includes an RPC Bus to translate Network command IDs into function calls,
			Lobby object management and Client/Server logic.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#include "main.hpp"
#include "NetworkingTest.hpp"
#include <thread>
#include "GameDefn.hpp"
#include <chrono>
#include <algorithm>

#pragma region RPC

void RPCBus(std::string message, sockaddr_in* client) {
	std::stringstream stream;

	stream << message;

	int message_id{};

	stream >> message_id;

	switch (message_id) {
	case CONNECT:
		std::cout << "Connect Request" << std::endl;
		GetNetworkServer()->OnPlayerConnected(message, client);
		break;
	case ONPLAYERCONNECTED:
		std::cout << "Return Connect Request" << std::endl;
		GetNetworkClient()->OnPlayerConnected(message);
		break;
	case UPDATE_CLIENT:
		//std::cout << "Return Update Request" << std::endl;
		break;
	case UPDATE_SERVER:
		//std::cout << "Update Request" << std::endl;
		break;
	case CREATE_LOBBY:
		std::cout << "Received Create Lobby Request!" << std::endl;
		GetNetworkServer()->CreateLobby(message, client);
		break;
	case RETURN_CREATE_LOBBY:
		std::cout << "Received Create Lobby Request Return!" << std::endl;
		GetNetworkClient()->ReturnCreateLobby(message);
		break;
	case REQUEST_LOBBIES:
		GetNetworkServer()->RequestLobbies(message, client);
		break;
	case RETURN_REQUEST_LOBBIES:
		GetNetworkClient()->ReturnRequestLobbies(message);
		break;
	case JOIN_LOBBY:
		GetNetworkServer()->JoinLobby(message, client);
		break;
	case RETURN_JOIN_LOBBY:
		GetNetworkClient()->ReturnJoinLobby(message);
		break;
	case DISCONNECT:
		GetNetworkServer()->OnDisconnected(message, client);
		break;
	case ON_DISCONNECTED:
		GetNetworkClient()->OnReturnDisconnected(message);
		break;
	case LEAVE_LOBBY:
		GetNetworkServer()->LeaveLobby(message, client);
		break;
	case RETURN_LEAVE_LOBBY:
		GetNetworkClient()->ReturnLeaveLobby(message);
		break;
	case REQUEST_LOBBY_DETAILS:
		GetNetworkServer()->RequestLobbyDetails(message, client);
		break;
	case RETURN_REQUEST_LOBBY_DETAILS:
		GetNetworkClient()->ReturnRequestLobbyDetails(message);
	case PING:
		GetNetworkClient()->OnPing();
		break;
	case RETURN_PING:
		GetNetworkServer()->ReturnPing(message, client);
		break;
	case REFRESH_CLIENTS:
		GetNetworkClient()->OnRefreshClients(message);
		break;
	case REFRESH_LOBBY_CLIENTS:
		GetNetworkClient()->OnRefreshLobbyClients(message);
		break;
	case ON_READY:
		GetNetworkServer()->OnPlayerReady(message, client);
		break;
	case ON_RETURN_START_GAME:
		GetNetworkClient()->OnStartGame(message);
		break;
	case UPDATE_PLAYER:
		GetNetworkServer()->UpdatePlayer(message, client);
		break;
	case RETURN_PLAYER_UPDATE:
		GetNetworkClient()->OnPlayerInputUpdate(message);
		break;
	}
}

#pragma endregion

#pragma region Server

void NetworkServer::OnPlayerConnected(std::string message, sockaddr_in* client) {
	std::shared_ptr<ClientConnection> client_connection = std::make_shared<ClientConnection>();

	s32 id{};
	//u64 player_time{};

	std::stringstream stream;

	stream << message;

	stream >> id;

	bool id_available = true;

	if (children.size() >= max_clients) {
		std::cout << "Max Clients Reached!" << std::endl;
		return;
	}

	AERandFloat();

	do {

		id = static_cast<s32>((AERandFloat() + 1.0f) * max_clients - max_clients);

		auto children_copy = children;

		for (const auto& child : children_copy) {
			std::shared_ptr<ClientConnection> client_check = std::dynamic_pointer_cast<ClientConnection>(child);
			
			if (client_check->player_id == id) {
				id_available = false;
				break;
			}
			
			id_available = true;
		}

	} while (!id_available);

	client_connection->player_id = id;
	client_connection->addr = *client;

	AddChild(client_connection);

	ReturnPlayerConnected(client_connection);
}

void NetworkServer::ReturnPlayerConnected(std::shared_ptr<ClientConnection>& client) {
	std::stringstream stream;

	stream << static_cast<int>(ONPLAYERCONNECTED) << " " << client->player_id << " " << current_time;

	std::cout << "Returned player connection request! " << stream.str() << std::endl;

	SendToClient(stream.str(), client);
}

void NetworkServer::Enter() {
	int err;

	WSADATA wsadata;
	err = WSAStartup(MAKEWORD(2, 2), &wsadata);

	if (err != 0) {
		printf("WSAStartup failed with error %d\n", err);
	}
	else {
		printf("WSAStartup success!\n");
	}

	if (LOBYTE(wsadata.wVersion) != 2 || HIBYTE(wsadata.wVersion) != 2) {
		/* Tell the user that we could not find a usable */
		/* WinSock DLL.                                  */
		printf("Could not find a usable version of Winsock.dll\n");
	}
	else
		printf("The Winsock 2.2 dll was found okay\n");

	server_socket = socket(AF_INET, SOCK_DGRAM, 0);

	if (server_socket < 0) {
		perror("Socket creation failed");
	}
	else {
		printf("Socket Creation Success!\n");
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(PORT);

	// Bind the socket to the address and port
	if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
		perror("Bind failed");
	}
	else {
		printf("Bind Success!\n");
	}

	std::weak_ptr<NetworkServer> self = std::dynamic_pointer_cast<NetworkServer>(shared_from_this());

	listen = std::thread([self]() { self.lock()->Network_Listen(); });
	
	listening = true;

	listen.detach();
}

void NetworkServer::Exit() {
	listening = false;

	if (listen.joinable()) {
		listen.join();
	}

	closesocket(server_socket);
}

void NetworkServer::Network_Listen() {
	while (listening) {
		char buffer[MAX_BUFFER]{};

		int len = recvfrom(server_socket, buffer, MAX_BUFFER, 0, reinterpret_cast<sockaddr*>(&client_addr), &client_addr_len);

		if (len == SOCKET_ERROR) {
			if (WSAGetLastError() == WSAEWOULDBLOCK) {

			}
			else {
				std::cout << "Network recvfrom() failed" << std::endl;
			}
		}
		else {
			std::string message(buffer, MAX_BUFFER);
			RPCBus(message, &client_addr);
		}
	}
}

void NetworkServer::Update() {
	static f32 time_since_last_ping{};

	current_time += static_cast<u64>(std::chrono::system_clock::now().time_since_epoch().count() - last_time);

	auto children_copy = children;

	if (time_since_last_ping > ping_gap) {
		for (auto& child : children_copy) {
			std::shared_ptr<ClientConnection> client = std::dynamic_pointer_cast<ClientConnection>(child);

			std::stringstream stream;

			stream << static_cast<int>(PING);

			SendToClient(stream.str(), client);
		}

		time_since_last_ping = 0.0f;
	}
	
	for (auto& child : children_copy) {
		std::shared_ptr<ClientConnection> client = std::dynamic_pointer_cast<ClientConnection>(child);

		if (client) {
			client->last_ping += static_cast<f32>(AEFrameRateControllerGetFrameTime());

			if (client->last_ping >= ping_timeout) {
				std::cout << "Client " << client->player_id << " timeout!" << std::endl;

				std::shared_ptr<Lobby> client_lobby = GetClientLobby(client->player_id);

				if (client_lobby) {
					client_lobby->RemovePlayer(client->player_id);
				}

				RemoveChild(client);
			}
		}
	}

	time_since_last_ping += static_cast<f32>(AEFrameRateControllerGetFrameTime());

	last_time = static_cast<u64>(current_time);
}

void NetworkServer::RefreshClients() {
	std::stringstream lobby_stream;

	lobby_stream << static_cast<int>(REFRESH_CLIENTS) << " " << GetLobbyContainer()->GetChildren().size() << " ";

	for (auto child : GetLobbyContainer()->GetChildren()) {
		std::shared_ptr<Lobby> lobby = std::dynamic_pointer_cast<Lobby>(child);

		if (lobby) {
			lobby_stream << lobby->lobby_id << " " << lobby->name << " ";

			s32 player_count{}, spectator_count{};

			for (s32 player : lobby->players) {
				if (player >= 0) {
					player_count++;
				}
			}

			for (s32 spectator : lobby->spectators) {
				if (spectator >= 0) {
					spectator_count++;
				}
			}

			lobby_stream << player_count << " " << spectator_count << " ";
		}
	}

	for (auto& child : children) {
		std::shared_ptr<ClientConnection> client = std::dynamic_pointer_cast<ClientConnection>(child);

		if (client) {
			ReturnRequestLobbies(lobby_stream.str(), client);
		}
	}

	for (auto& child : GetLobbyContainer()->GetChildren()) {
		std::shared_ptr<Lobby> lobby = std::dynamic_pointer_cast<Lobby>(child);

		if (lobby) {
			std::stringstream lobby_details_stream;

			lobby_details_stream << static_cast<int>(REFRESH_LOBBY_CLIENTS) << " ";

			s32 player_count{};

			for (s32 player : lobby->players) {
				if (player >= 0) {
					player_count++;
				}
			}

			lobby_details_stream << player_count << " ";

			for (s32 player : lobby->players) {
				if (player < 0) {
					continue;
				}

				std::shared_ptr<ClientConnection> client = GetClientFromID(player);

				if (client) {
					lobby_details_stream << client->player_id << " " << client->player_name << " " << client->ready << " ";
				}
			}

			for (s32 player : lobby->players) {
				if (player < 0) {
					continue;
				}

				std::shared_ptr<ClientConnection> client = GetClientFromID(player);

				if (client) {
					SendToClient(lobby_details_stream.str(), client);
				}
			}
		}
	}
}

void NetworkServer::ReturnPing(std::string message, sockaddr_in* client) { (void)client;
	std::stringstream stream;

	stream << message;

	s32 id{}, player_id{};

	stream >> id >> player_id;

	std::shared_ptr<ClientConnection> client_connection;

	if (!DoesClientExist(player_id, client_connection)) {
		return;
	}
	
	client_connection->last_ping = 0.0f;
}

void NetworkServer::SendToClient(std::string message, std::shared_ptr<ClientConnection>& client) {
	if (sendto(server_socket, message.c_str(), static_cast<int>(message.length()), 0, reinterpret_cast<sockaddr*>(&client->addr), client_addr_len) == SOCKET_ERROR) {
		std::cout << "Server sendto() failed!" << std::endl;
	}
}

void NetworkServer::CreateLobby(std::string message, sockaddr_in* client) {
	(void)client;

	s32 id{}, player_id{};
	std::string lobby_name;
	std::string player_name;
	std::shared_ptr<ClientConnection> client_connection;

	std::stringstream stream;

	stream << message;

	stream >> id >> player_id >> lobby_name >> player_name;

	if (!DoesClientExist(player_id, client_connection)) {
		std::cout << "Client " << player_id << " does not exist!" << std::endl;
		return;
	}

	if (IsClientInLobby(player_id)) {
		std::cout << "Client " << player_id << " is already in a Lobby!" << std::endl;
		return;
	}

	if (client_connection && (GetLobbyContainer()->GetChildren().size() >= max_lobbies)) {
		// Return Create Lobby, max lobby size reached!
		std::stringstream return_stream;

		return_stream << static_cast<int>(RETURN_CREATE_LOBBY) << " " << -1;

		ReturnCreateLobby(return_stream.str(), client_connection);

		return;
	}

	std::shared_ptr<Lobby> lobby = std::make_shared<Lobby>();

	s32 lobby_id{};
	
	bool id_available = true;

	do {
		lobby_id = static_cast<s32>((AERandFloat() + 1.0f) * max_lobbies - max_lobbies);

		auto children_copy = GetLobbyContainer()->GetChildren();

		for (const auto& child : children_copy) {
			std::shared_ptr<Lobby> child_lobby = std::dynamic_pointer_cast<Lobby>(child);

			if (child_lobby && child_lobby->lobby_id == lobby_id) {
				id_available = false;
				break;
			}

			id_available = true;
		}

	} while (!id_available);

	lobby->lobby_id = lobby_id;

	lobby->AddPlayer(player_id);

	while (player_name.at(player_name.length() - 1) == '\0') {
		player_name.pop_back();
	}
	
	lobby->name = lobby_name;

	GetLobbyContainer()->AddChild(lobby);

	client_connection->player_name = player_name;

	std::cout << "Created Lobby " << lobby->name << std::endl;

	std::stringstream return_stream;

	return_stream << static_cast<int>(RETURN_CREATE_LOBBY) << " " << lobby->lobby_id;

	ReturnCreateLobby(return_stream.str(), client_connection);

	RefreshClients();
}

void NetworkServer::ReturnCreateLobby(std::string message, std::shared_ptr<ClientConnection>& client_connection) {
	std::cout << "Returned lobby create request! " << message << std::endl;

	SendToClient(message, client_connection);
}

void NetworkServer::RequestLobbies(std::string message, sockaddr_in* client) {
	(void)client;

	std::stringstream stream;
	stream << message;

	s32 id{}, player_id{};

	stream >> id >> player_id;

	std::shared_ptr<ClientConnection> client_connection;

	if (!DoesClientExist(player_id, client_connection)) {
		std::cout << "Client " << player_id << " does not Exist!" << std::endl;
		return;
	}

	std::stringstream return_stream;

	return_stream << static_cast<int>(RETURN_REQUEST_LOBBIES) << " " << GetLobbyContainer()->GetChildren().size() << " ";

	for (auto child : GetLobbyContainer()->GetChildren()) {
		std::shared_ptr<Lobby> lobby = std::dynamic_pointer_cast<Lobby>(child);

		if (lobby) {
			return_stream << lobby->lobby_id << " " << lobby->name << " ";

			s32 player_count{}, spectator_count{};

			for (s32 player : lobby->players) {
				if (player >= 0) {
					player_count++;
				}
			}

			for (s32 spectator : lobby->spectators) {
				if (spectator >= 0) {
					spectator_count++;
				}
			}

			return_stream << player_count << " " << spectator_count << " ";
		}
	}

	ReturnRequestLobbies(return_stream.str(), client_connection);
}

void NetworkServer::ReturnRequestLobbies(std::string message, std::shared_ptr<ClientConnection>& client) {
	std::cout << "Returning Request Lobby Request!" << std::endl;

	SendToClient(message, client);
}

void NetworkServer::JoinLobby(std::string message, sockaddr_in* client) {
	(void)client;

	std::stringstream stream, return_stream;
	stream << message;

	s32 id{}, player_id{}, lobby_id{};
	std::string player_name;

	stream >> id >> player_id >> lobby_id >> player_name;

	while (player_name.at(player_name.length() - 1) == '\0') {
		player_name.pop_back();
	}

	std::shared_ptr<ClientConnection> client_connection;

	std::cout << "Client " << player_id << " is attempting to join Lobby " << lobby_id << std::endl;

	if (!DoesClientExist(player_id, client_connection)) {
		std::cout << "Client " << player_id << " does not Exist!" << std::endl;
		return;
	}

	if (IsClientInLobby(player_id)) {
		return_stream << static_cast<int>(RETURN_JOIN_LOBBY) << " " << -2;

		SendToClient(return_stream.str(), client_connection);

		return;
	}

	for (auto child : GetLobbyContainer()->GetChildren()) {
		std::shared_ptr<Lobby> lobby = std::dynamic_pointer_cast<Lobby>(child);

		if (lobby && (lobby->lobby_id == lobby_id)) {

			if (lobby->AddPlayer(player_id)) {
				return_stream << static_cast<int>(RETURN_JOIN_LOBBY) << " " << lobby_id;

				SendToClient(return_stream.str(), client_connection);

				client_connection->player_name = player_name;

				std::cout << "Return Join Lobby Success!" << std::endl;

				return;
			}
			else {
				return_stream << static_cast<int>(RETURN_JOIN_LOBBY) << " " << -2;

				SendToClient(return_stream.str(), client_connection);

				return;
			}
		}
	}

	return_stream << static_cast<int>(RETURN_JOIN_LOBBY) << " " << -1;

	SendToClient(return_stream.str(), client_connection);
}

void NetworkServer::LeaveLobby(std::string message, sockaddr_in* client) {
	(void)client;

	std::stringstream stream, return_stream;
	stream << message;

	s32 id{}, player_id{};

	stream >> id >> player_id;

	std::shared_ptr<ClientConnection> client_connection;

	if (!DoesClientExist(player_id, client_connection)) {
		std::cout << "Client " << player_id << " does not Exist!" << std::endl;
		return;
	}

	client_connection->ready = false;

	for (auto child : GetLobbyContainer()->GetChildren()) {
		std::shared_ptr<Lobby> lobby = std::dynamic_pointer_cast<Lobby>(child);

		if (lobby) {
			lobby->RemovePlayer(player_id);
		}
	}

	RefreshClients();

	return_stream << static_cast<int>(RETURN_LEAVE_LOBBY);

	SendToClient(return_stream.str(), client_connection);
}

void NetworkServer::OnDisconnected(std::string message, sockaddr_in* client) {
	(void)client;

	std::stringstream stream, return_stream;
	stream << message;

	s32 id{}, player_id{};

	stream >> id >> player_id;

	std::shared_ptr<ClientConnection> client_connection;

	if (!DoesClientExist(player_id, client_connection)) {
		std::cout << "Client " << player_id << " does not Exist!" << std::endl;
		return;
	}

	for (auto child : GetLobbyContainer()->GetChildren()) {
		std::shared_ptr<Lobby> lobby = std::dynamic_pointer_cast<Lobby>(child);

		if (!lobby) {
			continue;
		}

		lobby->RemovePlayer(player_id);
	}

	return_stream << static_cast<int>(ON_DISCONNECTED);

	SendToClient(return_stream.str(), client_connection);

	std::cout << "Disconnected Client " << client_connection->player_id << std::endl;

	RemoveChild(client_connection);

	std::cout << "Children Left: " << GetChildren().size() << std::endl;
}

void NetworkServer::RequestLobbyDetails(std::string message, sockaddr_in* client_v) { (void)client_v;
	std::stringstream stream, return_stream;

	stream << message;

	s32 id{}, player_id{};

	stream >> id >> player_id;

	std::shared_ptr<ClientConnection> client_connection;

	if (!DoesClientExist(player_id, client_connection)) {
		std::cout << "Client " << player_id << " does not Exist!" << std::endl;
		return;
	}

	if (!IsClientInLobby(player_id)) {
		return;
	}

	return_stream << static_cast<int>(RETURN_REQUEST_LOBBY_DETAILS) << " ";

	s32 player_count{};

	for (s32 player : GetClientLobby(player_id)->players) {
		if (player >= 0) {
			player_count++;
		}
	}

	return_stream << player_count << " ";

	for (s32 player : GetClientLobby(player_id)->players) {
		if (player < 0) {
			continue;
		}

		std::shared_ptr<ClientConnection> client = GetClientFromID(player);

		if (client) {
			return_stream << client->player_id << " " << client->player_name << " " << client->ready << " ";
		}
	}

	std::cout << "Returning Request for Lobby Details!" << std::endl;

	SendToClient(return_stream.str(), client_connection);
}

void NetworkServer::OnPlayerReady(std::string message, sockaddr_in* client) {(void)client;
	std::stringstream stream, return_stream;

	stream << message;

	s32 id{}, player_id{};

	stream >> id >> player_id;

	std::shared_ptr<ClientConnection> client_connection;

	if (!DoesClientExist(player_id, client_connection)) {
		std::cout << "Client " << player_id << " does not Exist!" << std::endl;
		return;
	}

	client_connection->ready = !client_connection->ready;

	std::shared_ptr<Lobby> client_lobby;

	client_lobby = GetClientLobby(player_id);

	bool all_ready = true;

	if (!client_lobby) return;

	for (s32& player : client_lobby->players) {
		std::shared_ptr<ClientConnection> client_p;

		if (DoesClientExist(player, client_p)) {
			if (!client_p->ready) {
				all_ready = false;
			}
		}
	}

	if (all_ready) {
		// Start Game
		return_stream << static_cast<int>(ON_RETURN_START_GAME) << " " << static_cast<u64>(current_time) << " ";

		u32 seed = static_cast<u32>(std::time(0));

		u32 seed2 = static_cast<u32>(AEGetTime(nullptr));

		u32 seed3 = static_cast<u32>(std::time(0) * AERandFloat());

		u32 seed4 = static_cast<u32>(std::time(0) - AEGetTime(nullptr) * AERandFloat());

		return_stream << seed << " " << seed2 << " " << seed3 << " " << seed4;

		for (s32& player : client_lobby->players) {
			std::shared_ptr<ClientConnection> client_p;

			if (DoesClientExist(player, client_p)) {
				SendToClient(return_stream.str(), client_p);
				client_p->ready = false;
			}
		}
	}

	RefreshClients();
}

void NetworkServer::UpdatePlayer(std::string message, sockaddr_in* client) {(void)client;
	std::stringstream stream, return_stream;

	stream << message;

	u64 input_time{};

	f32 dt{}, pos_x{}, pos_y{};

	s32 id{}, player_id{}, jump_pressed{}, jump_released{}, jump_triggered{}, left_pressed{}, right_pressed{};

	stream >> id >> player_id >> input_time >> jump_pressed >> jump_released >> jump_triggered >> left_pressed >> right_pressed >> dt >> pos_x >> pos_y;

	std::shared_ptr<ClientConnection> client_connection;

	if (!DoesClientExist(player_id, client_connection)) {
		std::cout << "Client " << player_id << " does not Exist!" << std::endl;
		return;
	}

	std::shared_ptr<Lobby> client_lobby;

	client_lobby = GetClientLobby(player_id);

	if (!client_lobby) return;

	return_stream << static_cast<int>(RETURN_PLAYER_UPDATE) << " " << player_id << " " << input_time << " " << jump_pressed << " " << jump_released << " " << jump_triggered << " " << left_pressed << " " << right_pressed << " " << dt << " " << pos_x << " " << pos_y;

	for (s32& player : client_lobby->players) {
		std::shared_ptr<ClientConnection> client_p;

		if (DoesClientExist(player, client_p)) {
			if (client_p == client_connection) continue;

			SendToClient(return_stream.str(), client_p);
		}
	}
}

#pragma endregion

#pragma region Client
void NetworkClient::Enter() {
	int err{};

	WSADATA wsadata;

	err = WSAStartup(MAKEWORD(2, 2), &wsadata);

	if (err != 0) {
		std::cout << "WSAStartup failed with error " << err << std::endl;
		return;
	}
	else {
		std::cout << "WSAStartup success!" << std::endl;
	}

	if (LOBYTE(wsadata.wVersion) != 2 || HIBYTE(wsadata.wVersion) != 2) {
		std::cout << "Could not find a usable version of Winsock.dll" << std::endl;
		return;
	}
	else {
		std::cout << "The Winsock 2.2 dll was found okay" << std::endl;
	}

	addrinfo hints{}, * res = nullptr;
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;

	if (getaddrinfo(host_name.c_str(), NULL, &hints, &res) != 0) {
		std::cout << "Failed to resolve hostname" << std::endl;
		return;
	}

	sockaddr_in* addr_in = reinterpret_cast<sockaddr_in*>(res->ai_addr);
	char ip_str[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &(addr_in->sin_addr), ip_str, INET_ADDRSTRLEN);

	std::cout << "Resolved IP Address for eisenfaust.ddns.net: " << ip_str << std::endl;

	client_socket = socket(AF_INET, SOCK_DGRAM, 0);

	if (client_socket < 0) {
		std::cout << "Socket Creation failed!" << std::endl;
		return;
	}
	else {
		std::cout << "Socket Creation Success!" << std::endl;
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT);
	server_addr.sin_addr = addr_in->sin_addr;

	std::stringstream stream;

	stream << CONNECT;

	SendToServer(stream.str());

	request_time = static_cast<u64>(std::chrono::system_clock::now().time_since_epoch().count());

	std::weak_ptr<NetworkClient> self = std::dynamic_pointer_cast<NetworkClient>(shared_from_this());

	listen = std::thread([self]() { self.lock()->NetworkClient_Listen(); });

	listening = true;

	listen.detach();

	freeaddrinfo(res);
}

void NetworkClient::SendToServer(std::string message) {
	if (sendto(client_socket, message.c_str(), static_cast<int>(message.length()), 0, reinterpret_cast<sockaddr*>(&server_addr), server_addr_len) == SOCKET_ERROR) {
		std::cout << "sendto() failed!" << std::endl;
	}
}

void NetworkClient::Exit() {
	Disconnect();

	player_id = -1;
	current_lobby_id = -1;

	listening = false;

	if (listen.joinable()) {
		listen.join();
	}
	
	closesocket(client_socket);

	Object::Exit();
}

void NetworkClient::NetworkClient_Listen() {
	while (listening) {
		char buffer[MAX_BUFFER]{};

		int len = recvfrom(client_socket, buffer, MAX_BUFFER, 0, reinterpret_cast<sockaddr*>(&server_addr), &server_addr_len);

		if (len == SOCKET_ERROR) {
			if (WSAGetLastError() == WSAEWOULDBLOCK) {
				
			}
			else {
				std::cout << "Client recvfrom() failed" << std::endl;
			}
		}
		else {
			std::string string(buffer, MAX_BUFFER);
			RPCBus(string, &server_addr);
		}
	}

	return;
}

void NetworkClient::OnPlayerConnected(std::string message) {
	std::stringstream stream;

	stream << message;

	std::cout << "Received Connection Request Return!" << std::endl;

	int operation_id{}, p_id{};

	u64 time_server{};

	stream >> operation_id >> p_id >> time_server;

	server_time = time_server + (static_cast<u64>(std::chrono::system_clock::now().time_since_epoch().count() - request_time) / 2);

	std::cout << p_id << std::endl;

	player_id = p_id;

	GetMultiplayerUIContainer()->MoveTo(0.0f);

	GetLobbyUIContainer()->MoveTo(0.0f);

	GetMainMenuUIContainer()->MoveTo(-screen_height);

	RequestLobbies();

	std::stringstream ss2;

	ss2 << p_id;

	GetPlayer()->SetName(ss2.str());
	
	std::shared_ptr<ErrorButton> error_button = std::dynamic_pointer_cast<ErrorButton>(GetErrorUIContainer()->GetChildren().at(0));

	if (error_button) {
		error_button->SetTransparency(0.0f);
		current_layer = 0;
	}
}

void NetworkClient::CreateLobby(std::string lobby_name, std::string player_name) {
	std::stringstream stream;

	stream << static_cast<int>(CREATE_LOBBY) << " " << player_id << " " << lobby_name << " " << player_name;

	std::cout << "Creating " << lobby_name << std::endl;

	SendToServer(stream.str());
}

void NetworkClient::ReturnCreateLobby(std::string message) {
	s32 id{}, lobby_id{};

	std::stringstream stream;

	stream << message;

	stream >> id >> lobby_id;

	if (lobby_id < 0) {
		// Failed to create lobby!
		return;
	}

	current_lobby_id = lobby_id;

	std::cout << "Successfully Joined Lobby: "  << current_lobby_id << std::endl;

	GetCurrentLobbyUI()->MoveTo(0.0f);

	GetLobbyUIContainer()->MoveTo(-screen_height);

	GetMultiplayerUIContainer()->MoveTo(-screen_height);

	RequestLobbyDetails();

	std::shared_ptr<ErrorButton> error_button = std::dynamic_pointer_cast<ErrorButton>(GetErrorUIContainer()->GetChildren().at(0));

	if (error_button) {
		error_button->SetTransparency(0.0f);
		current_layer = 0;
	}
}

void NetworkClient::RequestLobbies() {
	std::stringstream stream;

	stream << static_cast<int>(REQUEST_LOBBIES) << " " << player_id;

	SendToServer(stream.str());
}

void NetworkClient::ReturnRequestLobbies(std::string message) {
	std::stringstream stream;

	stream << message;

	s32 id{}, lobby_count{};

	stream >> id >> lobby_count;

	GetLobbyUIContainer()->MoveTo(0.0f);

	GetMainMenuUIContainer()->MoveTo(-screen_height);

	GetLobbyContainer()->ClearChildren();

	for (s32 i = 0; i < lobby_count; i++) {
		s32 player_count{}, spectator_count{}, lobby_id{};
		std::string lobby_name;
		
		stream >> lobby_id >> lobby_name >> player_count >> spectator_count;

		std::cout << "Lobby ID: " << lobby_id << std::endl;
		std::cout << "Lobby Name: " << lobby_name << std::endl;
		std::cout << "Player Count: " << player_count << std::endl;
		std::cout << "Spectator Count: " << spectator_count << std::endl;

		std::shared_ptr<Lobby> lobby = std::make_shared<Lobby>();
		lobby->lobby_id = lobby_id;
		lobby->num_players = player_count;
		lobby->num_spectators = spectator_count;
		lobby->name = lobby_name;
		GetLobbyContainer()->AddChild(lobby);
	}

	GetLobbyContainer()->lobby_added = true;
}

void NetworkClient::JoinLobby(s32 lobby_id, std::string player_name) {
	std::stringstream stream;

	stream << static_cast<int>(JOIN_LOBBY) << " " << player_id << " " << lobby_id << " " << player_name;

	std::cout << "Joining Lobby " << lobby_id << std::endl;

	SendToServer(stream.str());
}

void NetworkClient::ReturnJoinLobby(std::string message) {
	std::stringstream stream;
	stream << message;

	s32 id{}, lobby_id{};

	stream >> id >> lobby_id;

	if (lobby_id >= 0) {
		current_lobby_id = lobby_id;
		std::cout << "Successfully joined Lobby: " << current_lobby_id << std::endl;

		GetCurrentLobbyUI()->MoveTo(0.0f);

		GetLobbyUIContainer()->MoveTo(-screen_height);

		GetMultiplayerUIContainer()->MoveTo(-screen_height);

		RequestLobbyDetails();
	}
	else {
		std::cout << "Lobby Join Failed!" << std::endl;
	}

	std::shared_ptr<ErrorButton> error_button = std::dynamic_pointer_cast<ErrorButton>(GetErrorUIContainer()->GetChildren().at(0));

	if (error_button) {
		error_button->SetTransparency(0.0f);
		current_layer = 0;
	}
}

void NetworkClient::Disconnect() {
	std::stringstream stream;

	if (player_id == -1) {
		return;
	}

	stream << static_cast<int>(DISCONNECT) << " " << player_id;

	SendToServer(stream.str());
}

void NetworkClient::OnReturnDisconnected(std::string message) {
	player_id = -1;
	current_lobby_id = -1;

	std::cout << "Disconnected from Server!" << std::endl;
}

void NetworkClient::LeaveLobby() {
	std::stringstream stream;

	if (current_lobby_id == -1) {
		return;
	}

	stream << static_cast<int>(LEAVE_LOBBY) << " " << player_id;

	SendToServer(stream.str());
}

void NetworkClient::ReturnLeaveLobby(std::string message) {
	std::cout << "Successfully Left Lobby " << current_lobby_id;

	current_lobby_id = -1;

	std::cout << "Lobby ID: " << current_lobby_id;

	std::shared_ptr<ErrorButton> error_button = std::dynamic_pointer_cast<ErrorButton>(GetErrorUIContainer()->GetChildren().at(0));

	if (error_button) {
		error_button->SetTransparency(0.0f);
		current_layer = 0;
	}
}

void NetworkClient::RequestLobbyDetails() {
	std::stringstream stream;

	stream << static_cast<int>(REQUEST_LOBBY_DETAILS) << " " << player_id;

	SendToServer(stream.str());
}

void NetworkClient::ReturnRequestLobbyDetails(std::string message) {
	std::stringstream stream;

	std::cout << message;

	stream << message;

	s32 id{}, num_players{};

	stream >> id >> num_players;

	for (auto& detail : GetLobbyDetails()->details) {
		detail.player_id = -1;
	}

	for (int i = 0; i < num_players; i++) {
		s32 player_id_p{}, ready{};
		std::string player_name;

		stream >> player_id_p >> player_name >> ready;

		GetLobbyDetails()->details[i].player_id = player_id_p;
		GetLobbyDetails()->details[i].player_name = player_name;
		GetLobbyDetails()->details[i].ready = ready;
	}

	GetLobbyDetails()->on_change = true;

	std::shared_ptr<ErrorButton> error_button = std::dynamic_pointer_cast<ErrorButton>(GetErrorUIContainer()->GetChildren().at(0));

	if (error_button) {
		error_button->SetTransparency(0.0f);
		current_layer = 0;
	}
}

void NetworkClient::OnPing() {
	std::stringstream stream;

	stream << static_cast<int>(RETURN_PING) << " " << player_id << std::endl;

	SendToServer(stream.str());
}

void NetworkClient::OnRefreshClients(std::string message) {
	std::stringstream stream;

	stream << message;

	s32 id{}, lobby_count{};

	stream >> id >> lobby_count;

	GetLobbyContainer()->ClearChildren();

	for (s32 i = 0; i < lobby_count; i++) {
		s32 player_count{}, spectator_count{}, lobby_id{};
		std::string lobby_name;

		stream >> lobby_id >> lobby_name >> player_count >> spectator_count;

		std::cout << "Lobby ID: " << lobby_id << std::endl;
		std::cout << "Lobby Name: " << lobby_name << std::endl;
		std::cout << "Player Count: " << player_count << std::endl;
		std::cout << "Spectator Count: " << spectator_count << std::endl;

		std::shared_ptr<Lobby> lobby = std::make_shared<Lobby>();
		lobby->lobby_id = lobby_id;
		lobby->num_players = player_count;
		lobby->num_spectators = spectator_count;
		lobby->name = lobby_name;
		GetLobbyContainer()->AddChild(lobby);
	}

	GetLobbyContainer()->lobby_added = true;

	std::shared_ptr<ErrorButton> error_button = std::dynamic_pointer_cast<ErrorButton>(GetErrorUIContainer()->GetChildren().at(0));

	if (error_button) {
		error_button->SetTransparency(0.0f);
		current_layer = 0;
	}
}

void NetworkClient::OnRefreshLobbyClients(std::string message) {
	std::stringstream stream;

	std::cout << message;

	stream << message;

	s32 id{}, num_players{};

	stream >> id >> num_players;
	
	for (auto& detail : GetLobbyDetails()->details) {
		detail.player_id = -1;
	}

	for (int i = 0; i < num_players; i++) {
		s32 player_id_p{}, ready{};
		std::string player_name;

		stream >> player_id_p >> player_name >> ready;

		GetLobbyDetails()->details[i].player_id = player_id_p;
		GetLobbyDetails()->details[i].player_name = player_name;
		GetLobbyDetails()->details[i].ready = ready;
	}

	GetLobbyDetails()->on_change = true;

	std::shared_ptr<ErrorButton> error_button = std::dynamic_pointer_cast<ErrorButton>(GetErrorUIContainer()->GetChildren().at(0));

	if (error_button) {
		error_button->SetTransparency(0.0f);
		current_layer = 0;
	}
}

void NetworkClient::ReadyPlayer() {
	std::stringstream stream;

	stream << static_cast<int>(ON_READY) << " " << player_id;

	SendToServer(stream.str());
}

void NetworkClient::OnStartGame(std::string message) {
	std::stringstream stream;

	stream << message;

	u64 curr_server_time{};
	unsigned int seed1{}, seed2{}, seed3{}, seed4{};
	s32 id{};

	stream >> id >> curr_server_time >> seed1 >> seed2 >> seed3 >> seed4;

	f64 diff = static_cast<f64>((static_cast<s64>(server_time) + static_cast<s64>(std::chrono::system_clock::now().time_since_epoch().count() - static_cast<s64>(last_time)) - static_cast<s64>(curr_server_time)) / 10000000.0);

	std::cout << "Curr: " << server_time + static_cast<u64>(std::chrono::system_clock::now().time_since_epoch().count() - last_time) << std::endl;
	std::cout << "Curr_Server: " << curr_server_time << std::endl;
	std::cout << "Diff: " << diff << std::endl;

	GetCountdownTimer()->SetCountdown(5.0f - static_cast<f32>(diff));
	GetCountdownTimer()->StartCountdown();

	std::cout << "Start Game! Seed: " << seed1 << ", " << seed2 << ", " << seed3 << ", " << seed4 << std::endl;

	std::shared_ptr<Map> map = std::make_shared<Map>();
	map->SetName("MAP1");
	std::vector<Map::MapObject> mapObj = map->CreateSeed(-200, -50, -350, 10000, 250, 300, 100, 100.0f, 200.0f, seed1);
	map->mapObj = mapObj;
	GetMapContainer()->AddChild(map);

	std::shared_ptr<Map> map2 = std::make_shared<Map>();
	map2->SetName("MAP2");
	std::vector<Map::MapObject> mapObj2 = map->CreateSeed(50, 200, -325, 10000, 250, 300, 100, 100.0f, 200.0f, seed2);
	map2->mapObj = mapObj2;
	GetMapContainer()->AddChild(map2);

	std::shared_ptr<Map> map3 = std::make_shared<Map>();
	map3->SetName("MAP3");
	std::vector<Map::MapObject> mapObj3 = map->CreateSeed(-400, -400, -375, 10000, 250, 450, 100, 100.0f, 200.0f, seed3);
	map3->mapObj = mapObj3;
	GetMapContainer()->AddChild(map3);

	std::shared_ptr<Map> map4 = std::make_shared<Map>();
	map4->SetName("MAP4");
	std::vector<Map::MapObject> mapObj4 = map->CreateSeed(400, 400, -275, 10000, 250, 450, 100, 100.0f, 200.0f, seed4);
	map4->mapObj = mapObj4;
	GetMapContainer()->AddChild(map4);

	new_seed = seed4;

	GetMapContainer()->on_create_map = true;

	GetCamera()->SetParent(GetPlayer());
	GetMainMenuUIContainer()->MoveTo(-screen_height);
	GetCurrentLobbyUI()->MoveTo(-screen_height);

	GetPlayerBranch()->on_player_added = true;
}

void NetworkClient::UpdatePlayerInput(std::string message) {
	std::stringstream stream;

	stream << static_cast<int>(UPDATE_PLAYER) << " " << player_id << " " << message;

	SendToServer(stream.str());
}

void NetworkClient::OnPlayerInputUpdate(std::string message) {
	std::stringstream stream;

	stream << message;

	u64 input_time{};
	f32 dt{}, pos_x{}, pos_y{};
	s32 id{}, player_id_p{}, jump_press{}, jump_release{}, jump_triggered{}, left{}, right{};

	stream >> id >> player_id_p >> input_time >> jump_press >> jump_release >> jump_triggered >> left >> right >> dt >> pos_x >> pos_y;

	for (auto& child : GetPlayerBranch()->GetChildren()) {
		std::shared_ptr<Player> player = std::dynamic_pointer_cast<Player>(child);

		if (player) {

			std::stringstream player_stream;

			player_stream << player->GetName();

			s32 check_player_id{};

			player_stream >> check_player_id;

			if (check_player_id != player_id_p) {
				continue;
			}

			std::shared_ptr<ServerInputManager> server_input = std::dynamic_pointer_cast<ServerInputManager>(player->GetChildren().at(5));

			if (server_input) {
				server_input->last_input_time = input_time;

				server_input->input_history.push_back({ server_input->last_input_time, static_cast<bool>(jump_press), static_cast<bool>(jump_release), static_cast<bool>(jump_triggered), static_cast<bool>(left), static_cast<bool>(right), dt, pos_x, pos_y });

				// We need to rollback!
				if (server_input->latest_input_time > server_input->last_input_time) {
					std::cout << "=========================== ROLLBACK! ===========================" << std::endl;
				}
				else {
					server_input->latest_input_time = server_input->last_input_time;
				}
			}
		}
	}
}

void NetworkClient::Update() {

	current_time += static_cast<u64>(std::chrono::system_clock::now().time_since_epoch().count() - last_time);

	if (player_id > 0) {
		server_time += static_cast<u64>(std::chrono::system_clock::now().time_since_epoch().count() - last_time);
	}

	last_time = static_cast<u64>(current_time);
}

#pragma endregion

#pragma region LobbyContainer

void LobbyContainer::Update() {
	if (lobby_added) {
		GetLobbyUIContainer()->GetChildren().at(1)->ClearChildren();

		int i = 0;

		for (const auto& child : children) {
			std::shared_ptr<Lobby> const& lobby = std::dynamic_pointer_cast<Lobby>(child);

			if (lobby) {
				CreateLobbyUI(lobby->name, lobby->lobby_id, lobby->num_players, lobby->num_spectators, 0.0f, 0.3f * screen_height - i * 0.06f * screen_height);

				i++;
			}
		}

		lobby_added = false;
	}
}

void LobbyContainer::Exit() {
	Object::Exit();
}

#pragma endregion

#pragma region Helper Methods

bool DoesClientExist(s32 player_id, std::shared_ptr<ClientConnection>& client_check) {
	auto children_copy = GetNetworkServer()->GetChildren();

	for (auto child : children_copy) {
		std::shared_ptr<ClientConnection> client_connection = std::dynamic_pointer_cast<ClientConnection>(child);

		if (client_connection && client_connection->player_id == player_id) {
			client_check = client_connection;
			return true;
		}
	}

	return false;
}

bool IsClientInLobby(s32 player_id) {
	for (auto child : GetLobbyContainer()->GetChildren()) {
		std::shared_ptr<Lobby> lobby = std::dynamic_pointer_cast<Lobby>(child);

		for (s32 player : lobby->players) {
			if (player == player_id) {
				return true;
			}
		}
	}

	return false;
}


std::shared_ptr<Lobby> GetClientLobby(s32 player_id) {
	for (auto child : GetLobbyContainer()->GetChildren()) {
		std::shared_ptr<Lobby> lobby = std::dynamic_pointer_cast<Lobby>(child);

		for (s32 player : lobby->players) {
			if (player == player_id) {
				return lobby;
			}
		}
	}

	return nullptr;
}

std::shared_ptr<ClientConnection> GetClientFromID(s32 player_id) {
	for (auto& player : GetNetworkServer()->GetChildren()) {
		std::shared_ptr<ClientConnection> client = std::dynamic_pointer_cast<ClientConnection>(player);

		if (client) {
			if (client->player_id == player_id) {
				return client;
			}
		}
	}

	return nullptr;
}

#pragma endregion

#pragma region LobbyDetails

void LobbyDetails::Update() {
	if (on_change) {
		int i{};

		GetCurrentLobbyUI()->GetChildren().at(1)->ClearChildren();

		for (auto& lobby_details : details) {

			if (lobby_details.player_id <= -1) {
				continue;
			}

			std::string name_p = lobby_details.player_name;
			if (lobby_details.ready) {
				name_p += "    |    READY";
			}
			else {
				name_p += "    |    NOT READY";
			}
			std::shared_ptr<Button> player_name = std::make_shared<Button>();
			player_name->SetColor(0xFF3F3F3F);
			player_name->SetScale({ 0.43f * screen_width, 0.05f * screen_height });
			player_name->SetPosition({ 0.0f, 0.3f * screen_height - i * 0.05f * screen_height });
			player_name->SetTextScale(0.5f);
			player_name->SetText(name_p);
			GetCurrentLobbyUI()->GetChildren().at(1)->AddChild(player_name);

			i++;
		}

		on_change = false;
	}
}

#pragma endregion

#pragma region Lobby

bool Lobby::AddPlayer(s32 player_id) {
	for (s32& player : players) {
		if (player == player_id) {
			return false;
		}
		else if (player == -1) {
			player = player_id;
			num_players++;

			GetNetworkServer()->RefreshClients();
			return true;
		}

		std::cout << player << " ";
	}

	return false;
}

void Lobby::RemovePlayer(s32 player_id) {
	for (s32& player : players) {
		if (player == player_id) {
			player = -1;
			num_players--;
		}

		if (num_players <= 0) {
			parent.lock()->RemoveChild(shared_from_this());
		}
	}
}

#pragma endregion
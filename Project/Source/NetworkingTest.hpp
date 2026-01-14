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

#pragma once
#include "Object.hpp"
#include <thread>
#include <array>

void RPCBus(std::string message, sockaddr_in* client);

class ClientConnection : public Object {
public:
	ClientConnection(std::shared_ptr<Object> parent = nullptr) : Object(std::move(parent)) {}

	sockaddr_in addr{};

	s32 player_id = -1;

	f32 last_ping = 0.0f;

	std::string player_name = "PLAYER";

	bool ready = false;
};

bool DoesClientExist(s32 player_id, std::shared_ptr<ClientConnection>& client_connection);

bool IsClientInLobby(s32 player_id);

class Lobby : public Object {
public: 
	Lobby(std::shared_ptr<Object> parent = nullptr) : Object(std::move(parent)) {}

	s32 lobby_id{ -1 };

	const s32 max_players{ 4 };

	const s32 max_spectators{ 6 };

	std::string name = "LobbyName";

	s32 num_players{};
	s32 num_spectators{};

	std::array<s32, 4> players{-1, -1, -1, -1};
	
	std::array<s32, 6> spectators{-1, -1, -1, -1, -1, -1};

	bool AddPlayer(s32 player_id);

	void RemovePlayer(s32 player_id);
};

class NetworkServer : public Object {
public:	
	NetworkServer(std::shared_ptr<Object> parent = nullptr) : Object(std::move(parent)) {}

	void Network_Listen();

	void SendToClient(std::string message, std::shared_ptr<ClientConnection>& client);

	void OnPlayerConnected(std::string message, sockaddr_in* client);

	void ReturnPlayerConnected(std::shared_ptr<ClientConnection>& client);

	void CreateLobby(std::string message, sockaddr_in* client);

	void ReturnCreateLobby(std::string message, std::shared_ptr<ClientConnection>& client);

	void RequestLobbies(std::string message, sockaddr_in* client);

	void ReturnRequestLobbies(std::string message, std::shared_ptr<ClientConnection>& client);

	void RefreshClients();

	void ReturnPing(std::string message, sockaddr_in* client);

	void RequestLobbyDetails(std::string message, sockaddr_in* client);

	void JoinLobby(std::string message, sockaddr_in* client);

	void OnPlayerReady(std::string message, sockaddr_in* client);

	void LeaveLobby(std::string message, sockaddr_in* client);

	void OnDisconnected(std::string message, sockaddr_in* client);

	void UpdatePlayer(std::string message, sockaddr_in* client);

	void Enter() override;

	void Update() override;

	void Exit() override;

	void StartServer(void* context) {
		(void)context;
		Enter();
	}

	const s32 max_clients = 256;

	const s32 max_lobbies = 64;

	u64 last_time{};

	u64 current_time{};

protected:
	SOCKET server_socket{};

	sockaddr_in server_addr{}, client_addr{};

	s32 client_addr_len = sizeof(client_addr);

	bool listening = false;

	std::thread listen{};

	const f32 ping_gap{ 1.0f };
	const f32 ping_timeout{ 5.0f };
};

class LobbyContainer : public Object {
public:
	LobbyContainer(std::shared_ptr<Object> parent = nullptr) : Object(std::move(parent)) {}

	void Update() override;

	void Exit() override;

	bool lobby_added = false;
};

class NetworkClient : public Object {
public: 
	NetworkClient(std::shared_ptr<Object> parent = nullptr) : Object(std::move(parent)) {}

	void NetworkClient_Listen();

	void SendToServer(std::string message);

	void OnPlayerConnected(std::string message);

	void CreateLobby(std::string lobby_name, std::string player_name);

	void ReturnCreateLobby(std::string message);

	void RequestLobbies();

	void ReturnRequestLobbies(std::string message);

	void RequestLobbyDetails();

	void ReturnRequestLobbyDetails(std::string message);

	void JoinLobby(s32 lobby_id, std::string player_name);

	void ReturnJoinLobby(std::string message);

	void ReadyPlayer();

	void OnStartGame(std::string message);

	void UpdatePlayerInput(std::string message);

	void OnPlayerInputUpdate(std::string message);

	void LeaveLobby();

	void ReturnLeaveLobby(std::string message);

	void Disconnect();

	void OnReturnDisconnected(std::string message);

	void OnPing();

	void OnRefreshClients(std::string message);

	void OnRefreshLobbyClients(std::string message);

	void StartClient(void* context) {
		(void)context;
		Enter();
	}

	std::string host_name = "eisenfaust.ddns.net";

	s32 player_id = -1;

	s32 current_lobby_id = -1;

	u64 last_time{};

	u64 current_time{};

	u64 request_time{};

	u64 server_time{};

	void Enter() override;

	void Update() override;

	void Exit() override;

	u32 new_seed{};

protected:
	SOCKET client_socket{};

	sockaddr_in server_addr{};

	s32 server_addr_len = sizeof(server_addr);

	std::vector<std::shared_ptr<Lobby>> lobbies;

	bool listening = false;

	std::thread listen{};
};

class PlayerDetails : public Object {
public:
	PlayerDetails(std::shared_ptr<Object> parent = nullptr) : Object(std::move(parent)) {}

	std::string player_name;
	s32 player_id{ -1 };
	s32 ready{ 0 };
};

class LobbyDetails : public Object {
public:
	LobbyDetails(std::shared_ptr<Object> parent = nullptr) : Object(std::move(parent)) {
		for (auto& detail : details) {
			detail.player_id = -1;
		}
	}

	std::string lobby_name;
	s32 lobby_id{ -1 };

	bool on_change{ false };

	std::array<PlayerDetails, 4> details;

	void Update() override;
};

std::shared_ptr<Lobby> GetClientLobby(s32 player_id);

std::shared_ptr<ClientConnection> GetClientFromID(s32 player_id);
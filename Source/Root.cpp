/* Start Header ************************************************************************/
/*!
\file		Root.cpp
\author		Tai Xiao Xuan, Vu Phan Hung, Xavier Seet Long Chuan, Tan Chun Kwang Xavier, Tok Duan Jie Nelson
\par		xiaoxuan.tai@digipen.edu, phanhung.vu@digipen.edu, x.seet@digipen.edu, chunkwangxavier.tan@digipen.edu, t.duanjienelson@digipen.edu
\date		09/03/2025
\brief		This file implements the Base Scene as well as Static members for the game, and handling UI scene callbacks
			Each member handled the functions for the features they were responsible for, thus 20%
			contribution for each member.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#include "pch.hpp"
#include "AIPlayer.hpp"
#include "GameDefn.hpp"
#include <fstream>
#include <iomanip>
#include "PlayerMovementStateMachine.hpp"

f32 world_boundary_x_min{ -400.0f };
f32 world_boundary_x_max{ 400.0f };

s8 current_layer = 2;

void Static_Exit() {
	GetRoot()->Exit();
	GetNetworkServer()->Exit();
	GetNetworkClient()->Exit();
	GetLobbyContainer()->Exit();
}

std::shared_ptr<Object2D>& VictoryZone() {
	static std::shared_ptr<Object2D> zone = std::make_shared<Object2D>();
	return zone;
}

std::shared_ptr<Object>& GetRoot() {
	static std::shared_ptr<Object> root = std::make_shared<Object>();
	return root;
}

std::shared_ptr<NetworkClient>& GetNetworkClient() {
	static std::shared_ptr<NetworkClient> client = std::make_shared<NetworkClient>();
	return client;
}

std::shared_ptr<NetworkServer>& GetNetworkServer() {
	static std::shared_ptr<NetworkServer> server = std::make_shared<NetworkServer>();
	return server;
}

std::shared_ptr<LobbyContainer>& GetLobbyContainer() {
	static std::shared_ptr<LobbyContainer> lobby_container = std::make_shared<LobbyContainer>();
	return lobby_container;
}

std::shared_ptr<Camera2D>& GetCamera() {
	static std::shared_ptr<Camera2D> camera = std::make_shared<Camera2D>();
	camera->SetName("CAMERA");
	return camera;
}

std::shared_ptr<UIContainer>& GetMultiplayerUIContainer() {
	static std::shared_ptr<UIContainer> multiplayer_ui = std::make_shared<UIContainer>();
	multiplayer_ui->SetName("MULTIPLAYERUI");
	return multiplayer_ui;
}

std::shared_ptr<Credits>& GetCreditUIContainer() {
	static std::shared_ptr<Credits> credit_ui = std::make_shared<Credits>();
	credit_ui->SetName("CREDITUI");
	return credit_ui;
}

std::shared_ptr<UIContainer>& GetSettingUIContainer() {
	static std::shared_ptr<UIContainer> setting_ui = std::make_shared<UIContainer>();
	setting_ui->SetName("SETTINGUI");
	return setting_ui;
}

std::shared_ptr<UIContainer>& GetTutorialUIContainer() {
	static std::shared_ptr<UIContainer> tutorial_ui = std::make_shared<UIContainer>();
	tutorial_ui->SetName("TUTORIALUI");
	return tutorial_ui;
}

std::shared_ptr<UIContainer>& GetMainMenuUIContainer() {
	static std::shared_ptr<UIContainer> mainmenu_ui = std::make_shared<UIContainer>();
	mainmenu_ui->SetName("MAINMENU");
	return mainmenu_ui;
}

std::shared_ptr<PauseMenu>& GetPauseMenu() {
	static std::shared_ptr<PauseMenu> pause = std::make_shared<PauseMenu>();
	return pause;
}


std::shared_ptr<UIContainer>& GetLobbyUIContainer() {
	static std::shared_ptr<UIContainer> lobby_ui_container = std::make_shared<UIContainer>();
	lobby_ui_container->SetName("LOBBYUI");
	return lobby_ui_container;
}

std::shared_ptr<UIContainer>& GetCurrentLobbyUI() {
	static std::shared_ptr<UIContainer> current_lobby = std::make_shared<UIContainer>();
	return current_lobby;
}

std::shared_ptr<ImageRenderer2D>& GetBrickUILeft() {
	static std::shared_ptr<ImageRenderer2D> brick_bg = std::make_shared<ImageRenderer2D>();
	brick_bg->SetName("BRICK");
	return brick_bg;
}

std::shared_ptr<UIContainer>& GetErrorUIContainer() {
	static std::shared_ptr<UIContainer> error_ui_container = std::make_shared<UIContainer>();
	error_ui_container->SetName("ERRORUI");
	return error_ui_container;
}

std::shared_ptr<TextBox>& GetPlayerName() {
	static std::shared_ptr<TextBox> player_name = std::make_shared<TextBox>();
	return player_name;
}

std::shared_ptr<ImageRenderer2D>& GetBrickUIRight() {
	static std::shared_ptr<ImageRenderer2D> brick_bg = std::make_shared<ImageRenderer2D>();
	brick_bg->SetName("BRICK");
	return brick_bg;
}

std::shared_ptr<ImageRenderer2D>& GetClouds() {
	static std::shared_ptr<ImageRenderer2D> cloud_bg = std::make_shared<ImageRenderer2D>();
	cloud_bg->SetName("CLOUD");
	return cloud_bg;
}

std::shared_ptr<ImageRenderer2D>& GetBeanstalk() {
	static std::shared_ptr<ImageRenderer2D> beanstalk = std::make_shared<ImageRenderer2D>();
	return beanstalk;
}

std::shared_ptr<Player>& GetPlayer() {
	static std::shared_ptr<Player> player = std::make_shared<Player>();
	player->SetName("PLAYER");
	return player;
}

std::shared_ptr<LobbyDetails>& GetLobbyDetails() {
	static std::shared_ptr<LobbyDetails> lobby_details = std::make_shared<LobbyDetails>();
	return lobby_details;
}

std::shared_ptr<Object2D>& GetUIBranch() {
	static std::shared_ptr<Object2D> ui = std::make_shared<Object2D>();
	return ui;
}

std::shared_ptr<Object2D>& GetWorldBranch() {
	static std::shared_ptr<Object2D> world = std::make_shared<Object2D>();
	return world;
}

std::shared_ptr<PlayerManager>& GetPlayerBranch() {
	static std::shared_ptr<PlayerManager> players = std::make_shared<PlayerManager>();
	return players;
}

std::shared_ptr<MapContainer>& GetMapContainer() {
	static std::shared_ptr<MapContainer> map_container = std::make_shared<MapContainer>();
	return map_container;
}

std::shared_ptr<CountdownText>& GetCountdownTimer() {
	static std::shared_ptr<CountdownText> countdown = std::make_shared<CountdownText>();
	return countdown;
}

std::shared_ptr<VictoryScreen>& GetVictoryScreen() {
	static std::shared_ptr<VictoryScreen> victory_screen = std::make_shared<VictoryScreen>();
	return victory_screen;
}

std::shared_ptr<AudioDriver>& GetAudioDriver() {
	static std::shared_ptr<AudioDriver> audio_driver = std::make_shared<AudioDriver>();
	return audio_driver;
}

void CreateLobbyUI(std::string name, int lobby_id, int num_players, int num_spectators, f32 x, f32 y) {
	std::stringstream stream;

	stream << std::left << std::setw(18) << name;

	stream << std::right << std::setw(5) << "|" << std::right << std::setw(5) << num_players << "/" << 4;

	stream << std::right << std::setw(5) << "|" << std::right << std::setw(5) << " " << num_spectators << "/" << 10;

	std::shared_ptr<Button> lobby_button = std::make_shared<Button>();
	lobby_button->SetColor(0xFF3F3F3F);
	lobby_button->SetScale({ 0.43f * screen_width, 0.05f * screen_height });
	lobby_button->SetPosition({ x, y });
	lobby_button->SetTextScale(0.5f);
	lobby_button->SetText(stream.str());
	lobby_button->data = lobby_id;

	GetLobbyUIContainer()->GetChildren().at(1)->AddChild(lobby_button);

	std::weak_ptr<Button> weak_button = lobby_button;

	std::weak_ptr<ErrorButton> weak_error_button = std::dynamic_pointer_cast<ErrorButton>(GetErrorUIContainer()->GetChildren().at(0));

	Callback join_lobby_callback = [weak_button, weak_error_button](void* context) {
		(void)context;

		if (!weak_button.lock()) {
			std::cout << "No Lobby Button!" << std::endl;
			return;
		}

		GetNetworkClient()->JoinLobby(weak_button.lock()->data, GetPlayerName()->GetText());

		if (weak_error_button.lock()) {
			weak_error_button.lock()->SetErrorText("Joining Lobby");
		}

		current_layer = 2;
	};

	lobby_button->OnPressed += join_lobby_callback;
}

void SaveHierarchy() {
	rapidjson::StringBuffer buffer;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);

	GetRoot()->Serialize(writer);

	std::ofstream file("hierarchy.json");

	if (file.is_open()) {
		file << buffer.GetString();
		file.close();
		std::cout << "Hierarchy saved to: hierarchy.json" << std::endl;
	}
}

void CreateObject(const rapidjson::Value& child_value) {
	if (!child_value.HasMember("type")) {
		std::cout << "JSON must contain a object with a type!" << std::endl;
		return;
	}

	switch (child_value["type"].GetInt()) {
		case OBJECT:
		{
			std::cout << "Creating an Object!" << std::endl;
			std::shared_ptr<Object> object = std::make_shared<Object>();
			object->Init(child_value);
			GetRoot()->AddChild(object);
			break;
		}
		case OBJECT2D:
		{
			std::cout << "Creating an Object2D!" << std::endl;
			std::shared_ptr<Object2D> object = std::make_shared<Object2D>();
			object->Init(child_value);
			GetRoot()->AddChild(object);
			break;
		}
		case EDITOR2D:
		{
			std::cout << "Creating an Editor2D!" << std::endl;
			std::shared_ptr<Editor2D> object = std::make_shared<Editor2D>();
			object->Init(child_value);
			GetRoot()->AddChild(object);
			break;
		}
		case RECT2D: 
		{
			std::cout << "Creating a Rect2D!" << std::endl;
			std::shared_ptr<Rect2D> object = std::make_shared<Rect2D>();
			object->Init(child_value);
			GetRoot()->AddChild(object);
			break;
		}
		case RECTPLAYER:
		{
			std::cout << "Creating a Rect2DPlayer!" << std::endl;
			std::shared_ptr<RectPlayer> object = std::make_shared<RectPlayer>();
			object->Init(child_value);
			GetRoot()->AddChild(object);
			break;
		}
		case TEXT2D:
		{
			std::cout << "Creating a Text2D!" << std::endl;
			std::shared_ptr<Text2D> object = std::make_shared<Text2D>();
			object->Init(child_value);
			GetRoot()->AddChild(object);
			break;
		}
		case IMAGERENDERER2D:
		{
			std::cout << "Creating an ImageRenderer2D!" << std::endl;
			std::shared_ptr<ImageRenderer2D> object = std::make_shared<ImageRenderer2D>();
			object->Init(child_value);
			GetRoot()->AddChild(object);
			break;
		}
	}
}

void LoadHierarchy(std::string file_name) {
	std::ifstream file(file_name);

	if (!file.is_open()) {
		std::cout << "file not found!" << std::endl;
		return;
	}

	std::ostringstream oss;

	oss << file.rdbuf();

	std::string json_content = oss.str();

	rapidjson::Document document;
	document.Parse(json_content);

	if (document.HasParseError()) {
		std::cout << "Error Parsing JSON file!" << std::endl;
		return;
	}

	if (!document.IsObject() || !document.HasMember("children") || !document["children"].IsArray()) {
		std::cout << "JSON must contain a children array at root level!" << std::endl;
		return;
	}

	std::cout << "Deserializing Hierarchy!" << std::endl;

	for (const auto& child_value : document["children"].GetArray()) {
		CreateObject(child_value);
	}
}

void BaseScene() {
	current_layer = 0;
	// Root\first_child
	// Reserved for World Elements ONLY.

	std::shared_ptr<Object2D> first_child = GetWorldBranch();
	first_child->SetName("WORLD");
	GetRoot()->AddChild(first_child);

		std::shared_ptr<ImageRenderer2D> sky = std::make_shared<ImageRenderer2D>();
		sky->SetScale({ 1.0f * screen_width, 1.0f * screen_height });
		sky->LoadTexture("Assets/Background.png");
		sky->SetSaturation(1.0f);
		first_child->AddChild(sky);

		std::shared_ptr<ImageRenderer2D> clouds = GetClouds();
		clouds->SetScale({ 0.8f * screen_width, 1.0f * screen_height });
		clouds->LoadTexture("Assets/Clouds.png");
		clouds->SetTiling(1, 1);
		clouds->SetTransparency(0.7f);
		clouds->SetTexScrollSpeed(0.05f, -0.05f);
		clouds->SetSaturation(1.0f);
		first_child->AddChild(clouds);

		std::shared_ptr<ImageRenderer2D> mountain = std::make_shared<ImageRenderer2D>();
		mountain->SetScale({ 0.8f * screen_width, 1.0f * screen_height });
		mountain->SetPosition({ 0.0f, -0.2f * screen_height });
		mountain->LoadTexture("Assets/Rearground.png");
		first_child->AddChild(mountain);

		first_child->AddChild(GetMapContainer());

		GetBeanstalk()->SetScale({ 0.15f * screen_width, 1.0f * screen_height });
		GetBeanstalk()->SetPosition({ 0.0f, 0.0f });
		GetBeanstalk()->LoadTexture("Assets/Stem.png");
		GetBeanstalk()->SetTiling(1, 4);
		GetBeanstalk()->SetSaturation(0.8f);
		first_child->AddChild(GetBeanstalk());

		std::shared_ptr<RectPlayer> win_platform = std::make_shared <RectPlayer>();
		win_platform->SetName("WINPLATFORM");
		win_platform->SetScale({ 0.0f, 0.0f });
		win_platform->SetPosition({ 0.0f, 10200.0f });
		win_platform->SetColor(0x4FCF8C19);
		first_child->AddChild(win_platform);

			std::shared_ptr<ImageRenderer2D> golden_egg = std::make_shared<ImageRenderer2D>();
			golden_egg->SetScale({ 1000.f, 1000.f });
			golden_egg->LoadTexture("Assets/GoldenEgg.png");
			golden_egg->SetSpaceType(WORLD_SPACE);
			golden_egg->SetPosition({ 0.f, 100.f });
			win_platform->AddChild(golden_egg);

			std::shared_ptr<ImageRenderer2D> win_platform_img = std::make_shared<ImageRenderer2D>();
			win_platform_img->SetScale({ 1280.f, 20.f });
			win_platform_img->SetPosition({ 0.f, 0.f });
			win_platform_img->LoadTexture("Assets/FinishLine.png");
			win_platform_img->SetSpaceType(WORLD_SPACE);
			win_platform_img->SetTiling(4, 1);
			win_platform->AddChild(win_platform_img);
			

		std::shared_ptr<RectPlayer> ground = std::make_shared <RectPlayer>();
		ground->SetName("GROUND");
		ground->SetScale({ 2600.0f, 2600.0f });
		ground->SetPosition({ 0.0f, -1600.0f });
		ground->SetColor(0x00000000);
		first_child->AddChild(ground);

			std::shared_ptr<ImageRenderer2D> brick_ground = std::make_shared<ImageRenderer2D>();
			brick_ground->SetScale({ 0.5f * screen_width, 1.0f * screen_height });
			brick_ground->SetPosition({ 0.0f, 1300.0f - 0.5f * screen_height });
			brick_ground->LoadTexture("Assets/Brick_Wall.png");
			brick_ground->SetSpaceType(WORLD_SPACE);
			brick_ground->SetTiling(2, 2);
			brick_ground->SetSaturation(0.8f);
			brick_ground->SetTexScrollSpeed(0.0f, 0.0f);
			ground->AddChild(brick_ground);

		std::shared_ptr<ImageRenderer2D> image = GetBrickUIRight();
		image->SetScale({ 0.5f * screen_width, 1.0f * screen_height });
		image->SetPosition({ 0.5f * screen_width, 0.0f });
		image->LoadTexture("Assets/Brick_Wall.png");
		image->SetTiling(2, 2);
		image->SetSaturation(0.8f);
		image->SetTexScrollSpeed(0.0f, -0.5f);
		first_child->AddChild(image);

		std::shared_ptr<ImageRenderer2D> image2 = GetBrickUILeft();
		image2->SetScale({ 0.5f * screen_width, 1.0f * screen_height });
		image2->SetPosition({ -0.5f * screen_width, 0.0f });
		image2->LoadTexture("Assets/Brick_Wall.png");
		image2->SetTiling(2, 2);
		image2->SetSaturation(0.8f);
		image2->SetTexScrollSpeed(0.0f, -0.5f);
		first_child->AddChild(image2);

		std::shared_ptr<ImageRenderer2D> win_platform_ui = std::make_shared<ImageRenderer2D>();
		win_platform_ui->SetPosition({ 0.4f * screen_width, 0.38f * screen_height });
		win_platform_ui->LoadTexture("Assets/FinishLine.png");
		win_platform_ui->SetScale({ 160.f, 10.f });
		first_child->AddChild(win_platform_ui);

	// Root\second_child
	// Reserved for Players ONLY.
	std::shared_ptr<PlayerManager> second_child = GetPlayerBranch();
	second_child->SetName("PLAYERS");
	GetRoot()->AddChild(second_child);

	GetCamera()->SetPosition({ 0.0f, -2600.0f });
	GetRoot()->AddChild(GetCamera());
	//std::shared_ptr<RectPlayer> rect_player = std::make_shared <RectPlayer> ();
	//rect_player->SetName("PLAYER");
	//rect_player->SetScale({ 400.0f, 400.0f });
	//rect_player->SetPosition({ 0.0f, 0.0f });
	//rect_player->SetColor(0x5F000000);
	//second_child->AddChild(rect_player);

		GetPlayer()->SetName("1");
		GetPlayer()->SetPosition({ 0.0f, -250.0f });
		GetPlayer()->SetScale({ 50.0f, 50.0f });

			std::shared_ptr<RectPlayer> playerGroundCheck = std::make_shared <RectPlayer>();
			playerGroundCheck->SetName("GROUNDCHECK");
			playerGroundCheck->SetScale({ 50.0f, 10.0f });
			playerGroundCheck->SetPosition({ 0.0f, -30.0f });
			playerGroundCheck->SetColor(0x4F0000FF);
			playerGroundCheck->SetTransparency(0.0f);
			GetPlayer()->AddChild(playerGroundCheck);

			std::shared_ptr<RectPlayer> playerCoyote = std::make_shared <RectPlayer>();
			playerCoyote->SetName("COYOTE");
			playerCoyote->SetScale({ 50.0f, 50.0f });
			playerCoyote->SetPosition({ 0.0f, -30.0f });
			playerCoyote->SetTransparency(0.0f);
			playerCoyote->SetColor(0x4F00FF00);
			GetPlayer()->AddChild(playerCoyote);

			std::shared_ptr<RectPlayer> playerRunOffL = std::make_shared <RectPlayer>();
			playerRunOffL->SetName("RUNOFFL");
			playerRunOffL->SetScale({ 50.0f, 50.0f });
			playerRunOffL->SetPosition({ -50.0f, -30.0f });
			playerRunOffL->SetTransparency(0.0f);
			playerRunOffL->SetColor(0x4FFF00FF);
			GetPlayer()->AddChild(playerRunOffL);

			std::shared_ptr<RectPlayer> playerRunOffR = std::make_shared <RectPlayer>();
			playerRunOffR->SetName("RUNOFFR");
			playerRunOffR->SetScale({ 50.0f, 50.0f });
			playerRunOffR->SetPosition({ 50.0f, -30.0f });
			playerRunOffR->SetTransparency(0.0f);
			playerRunOffR->SetColor(0x4FFF00FF);
			GetPlayer()->AddChild(playerRunOffR);

			std::shared_ptr<SpriteRenderer> playerMesh = std::make_shared <SpriteRenderer>();
			playerMesh->SetName("MESH");
			playerMesh->SetScale({ 50.0f, 50.0f });
			playerMesh->SetPosition({ 0.0f, 0.0f });
			playerMesh->LoadTexture("Assets/JackedJack_Red_SpriteSheet1.png");
			playerMesh->LoadSecondaryMesh("Assets/JackedJack_Red_SpriteSheet2.png");
			playerMesh->SetFPS(40.0f);
			playerMesh->SetSpriteTiling(0.0625f, 0.0625f);
			playerMesh->SetDrawScale(1.8f);
			playerMesh->SetSpaceType(WORLD_SPACE);
			GetPlayer()->AddChild(playerMesh);

			std::shared_ptr<InputManager> inputManager = std::make_shared <InputManager>();
			std::weak_ptr<InputManager> weak_input_manager = inputManager;
			inputManager->SetName("Input Manager");
			inputManager->active = false;
			GetPlayer()->AddChild(inputManager);

			std::shared_ptr<Text2D> nametag = std::make_shared<Text2D>();
			nametag->SetText("PLAYER");
			nametag->SetScale({ 0.5f, 0.5f });
			nametag->SetPosition({ 0.0f, 60.0f });
			nametag->SetSpaceType(WORLD_SPACE);
			GetPlayer()->AddChild(nametag);

			std::shared_ptr<PieTimer> jump_timer = std::make_shared<PieTimer>();
			jump_timer->SetName("PIETIMER");
			jump_timer->SetScale({ 10.0f, 10.0f });
			jump_timer->SetPosition({ 45.0f, 0.0f });
			jump_timer->SetColor(0xFF000000);
			GetPlayer()->AddChild(jump_timer);

			std::shared_ptr<VerticalProgressBar> vprog = std::make_shared<VerticalProgressBar>();
			vprog->SetName("VPROG");
			vprog->SetScale({ 20.f, 5.f });
			vprog->SetPosition({ 0.45f * screen_width, -0.4f * screen_height });
			vprog->SetColor(0xFFFF0000);
			GetPlayer()->AddChild(vprog);

			std::shared_ptr<ImageRenderer2D> progress_bar = std::make_shared<ImageRenderer2D>();
			progress_bar->SetName("PROGRESSBAR");
			progress_bar->SetScale({ 32.f, 32.f });
			progress_bar->SetPosition({ 0.45f * screen_width, -0.4f * screen_height });
			progress_bar->LoadTexture("Assets/JackedJackIcon.png");
			progress_bar->SetSpaceType(SCREEN_SPACE);
			progress_bar->SetTransparency(0.f);
			GetPlayer()->AddChild(progress_bar);

		second_child->AddChild(GetPlayer());


	// Display and do collision with boss enemy ()
		std::shared_ptr<EnemyBoss> enemyBoss = std::make_shared <EnemyBoss>();
		enemyBoss->SetScale({ 100.0f, 100.0f });
		enemyBoss->SetPosition({ 0.0f, 10260.0f });	// Need to change pos (detection at start of game)
		enemyBoss->SetName("Enemy Boss");
		GetWorldBranch()->AddChild(enemyBoss);

			std::shared_ptr<SpriteRenderer> boss_sprite = std::make_shared<SpriteRenderer>();
			boss_sprite->SetName("BOSSSPRITE");
			boss_sprite->SetScale({ 100.0f, 100.0f });
			boss_sprite->SetPosition({ 0.0f, 50.0f });
			boss_sprite->LoadTexture("Assets/Boss_Walk.png");
			boss_sprite->LoadSecondaryMesh("Assets/Boss_Attack.png");
			boss_sprite->SetFPS(40.0f);
			boss_sprite->SetSpriteTiling(0.0625f, 0.0625f);
			boss_sprite->SetDrawScale(4.f);
			boss_sprite->SetSpaceType(WORLD_SPACE);
			enemyBoss->AddChild(boss_sprite);
			// End of boss enemy

	// Root\third_child
	// Reserved for UI Elements ONLY. 
	std::shared_ptr<Object2D>& third_child = GetUIBranch();
	third_child->SetName("UI");
	GetRoot()->AddChild(third_child);

	GetMainMenuUIContainer()->SetScrollSpeed(5.0f);
	third_child->AddChild(GetMainMenuUIContainer());

	GetLobbyUIContainer()->SetPosition({ 0.0f, 1.0f * screen_height });
	GetLobbyUIContainer()->MoveTo(GetLobbyUIContainer()->GetLocalPosition().y);
	GetLobbyUIContainer()->SetScrollSpeed(5.0f);
	third_child->AddChild(GetLobbyUIContainer());

	GetCreditUIContainer()->SetPosition({0.0f, -2.0f * screen_height});
	GetCreditUIContainer()->MoveTo(GetCreditUIContainer()->GetLocalPosition().y);
	GetCreditUIContainer()->SetScrollSpeed(5.0f);
	third_child->AddChild(GetCreditUIContainer());

		std::shared_ptr<Button> credit_bg = std::make_shared<Button>();
		credit_bg->SetScale({ 0.5f * screen_width, 2.0f * screen_height });
		credit_bg->LoadTexture("Assets/Sign.png");
		credit_bg->SetButtonType(BUTTON_IMAGE);
		credit_bg->SetButtonLayer(20);
		GetCreditUIContainer()->AddChild(credit_bg);

		
		std::shared_ptr<Button> credit_back = std::make_shared<Button>();
		credit_back->SetName("CREDITBACK");
		credit_back->SetScale({ 0.06f * screen_height, 0.06f * screen_height });
		credit_back->SetText("");
		credit_back->SetPosition({ -0.22f * screen_width, 0.42f * screen_height });
		credit_back->LoadTexture("Assets/Back_Button.png");
		credit_back->SetButtonType(BUTTON_IMAGE);
		GetCreditUIContainer()->AddChild(credit_back);

		Callback credit_back_to_menu = [](void* context) {
			(void)context;

			GetMainMenuUIContainer()->MoveTo(0.0f);
			GetCreditUIContainer()->MoveTo(-2.0f * screen_height);
			GetCreditUIContainer()->roll_credits = false;
		};

		credit_back->OnPressed += credit_back_to_menu;

		// Instance of Credits
		std::shared_ptr<Credits> creditsInst = std::make_shared<Credits>();

		f32 startY = 0.45f * screen_height; // Starting position at the top
		f32 yOffset = -0.03f * screen_height; // Distance between each name

		for (const auto& textString : creditsInst->credits_names) {
			std::shared_ptr<Text2D> text = std::make_shared<Text2D>();
			text->SetText(textString);

			// Check if text is HEADER text and apply custom settings
			if (textString == "CREDITS" 
				|| textString == "DEVELOPED BY"
				|| textString == "INSTRUCTORS"
				|| textString == "PRESIDENT"
				|| textString == "EXECUTIVES"
				|| textString == "SPECIAL THANKS")
			{
				text->SetScale({ 0.7f, 0.7f });  // Larger scale for headings
				text->SetTextColor(0.0f, 0.8f, 0.0f, 1.0f);  // Green color for heading
			}
			else if (textString == "All content copyrighted 2025 DigiPen (SINGAPORE), All Rights Reserved") {
				text->SetScale({ 0.4f, 0.4f });
				text->SetTextColor(1.0f, 1.0f, 1.0f, 1.0f);
			}
			else {
				text->SetScale({ 0.5f, 0.5f });  // Smaller scale for names
				text->SetTextColor(1.0f, 1.0f, 1.0f, 1.0f); // Black color for names
			}

			// Set position and add to the UI container
			text->SetPosition({ 0.0f * screen_width, startY });  // Centered horizontally

			GetCreditUIContainer()->AddChild(text);

			// Move position for next text
			startY += yOffset;
		}


	GetSettingUIContainer()->SetPosition({ 0.0f, -1.0f * screen_height });
	GetSettingUIContainer()->MoveTo(GetSettingUIContainer()->GetLocalPosition().y);
	GetSettingUIContainer()->SetScrollSpeed(5.0f);
	third_child->AddChild(GetSettingUIContainer());

	GetTutorialUIContainer()->SetPosition({ 0.0f, -1.0f * screen_height });
	GetTutorialUIContainer()->MoveTo(GetTutorialUIContainer()->GetLocalPosition().y);
	GetTutorialUIContainer()->SetScrollSpeed(5.0f);
	third_child->AddChild(GetTutorialUIContainer());

	GetMultiplayerUIContainer()->SetPosition({ 0.0f, 1.0f * screen_height });
	GetMultiplayerUIContainer()->MoveTo(GetMultiplayerUIContainer()->GetLocalPosition().y);
	GetMultiplayerUIContainer()->SetScrollSpeed(5.0f);
	third_child->AddChild(GetMultiplayerUIContainer());

	GetErrorUIContainer()->SetPosition({ 0.0f, 0.0f * screen_height });
	GetErrorUIContainer()->MoveTo(GetErrorUIContainer()->GetLocalPosition().y);
	GetErrorUIContainer()->SetScrollSpeed(5.0f);
	third_child->AddChild(GetErrorUIContainer());

	GetCurrentLobbyUI()->SetPosition({ 0.0f, 1.0f * screen_height });
	GetCurrentLobbyUI()->MoveTo(GetCurrentLobbyUI()->GetLocalPosition().y);
	GetCurrentLobbyUI()->SetScrollSpeed(5.0f);
	third_child->AddChild(GetCurrentLobbyUI());

	GetCountdownTimer()->SetScale({ 2.5f, 2.5f });
	GetCountdownTimer()->SetPosition({ 0.f, 0.f });
	GetCountdownTimer()->SetTransparency(0.f);
	third_child->AddChild(GetCountdownTimer());

	GetVictoryScreen()->SetButtonLayer(3);
	GetVictoryScreen()->SetColor(0x7F0F0F0F);
	GetVictoryScreen()->SetScale({ 1.0f * screen_width, 1.0f * screen_height });
	GetVictoryScreen()->SetText(" Win!");
	GetVictoryScreen()->SetTextScale(1.0f);
	GetVictoryScreen()->SetTransparency(0.0f);
	GetVictoryScreen()->SetPosition({ 0.0f, 0.0f });
	third_child->AddChild(GetVictoryScreen());

	GetPauseMenu()->SetPosition({ 0.f, -1.f * screen_height });
	GetPauseMenu()->MoveTo(GetPauseMenu()->GetLocalPosition().y);
	GetPauseMenu()->SetScrollSpeed(5.f);
	third_child->AddChild(GetPauseMenu());

		std::shared_ptr<Rect2D> pause_menu_bg = std::make_shared<Rect2D>();
		pause_menu_bg->SetName("PAUSEBG");
		pause_menu_bg->SetScale({ 1.f * screen_width, 1.f * screen_height });
		pause_menu_bg->SetTransparency(0.3f);
		GetPauseMenu()->AddChild(pause_menu_bg);

		std::shared_ptr<Button> pause_menu_button1 = std::make_shared<Button>();
		pause_menu_button1->SetName("RESUMEBUTTON");
		pause_menu_button1->SetScale({ 0.125f * screen_width, 0.1f * screen_height });
		pause_menu_button1->SetTextScale(0.6f);
		pause_menu_button1->SetButtonLayer(-2);
		pause_menu_button1->SetText("RESUME");
		pause_menu_button1->SetPosition({ 0.0f * screen_width , 0.1f * screen_height });
		pause_menu_button1->SetColor(0xFFCF8C19);
		pause_menu_button1->LoadTexture("Assets/Sign.png");
		pause_menu_button1->SetButtonType(BUTTON_IMAGE);
		GetPauseMenu()->AddChild(pause_menu_button1);

		std::shared_ptr<Button> pause_menu_button2 = std::make_shared<Button>();
		pause_menu_button2->SetName("MENUBUTTON");
		pause_menu_button2->SetScale({ 0.125f * screen_width, 0.1f * screen_height });
		pause_menu_button2->SetTextScale(0.6f);
		pause_menu_button2->SetButtonLayer(-2);
		pause_menu_button2->SetText("MAIN MENU");
		pause_menu_button2->SetPosition({ 0.0f * screen_width , -0.1f * screen_height });
		pause_menu_button2->SetColor(0xFFCF8C19);
		pause_menu_button2->LoadTexture("Assets/Sign.png");
		pause_menu_button2->SetButtonType(BUTTON_IMAGE);
		GetPauseMenu()->AddChild(pause_menu_button2);

		std::shared_ptr<Button> pause_menu_button3 = std::make_shared<Button>();
		pause_menu_button3->SetName("QUITBUTTON");
		pause_menu_button3->SetScale({ 0.125f * screen_width, 0.1f * screen_height });
		pause_menu_button3->SetTextScale(0.6f);
		pause_menu_button3->SetButtonLayer(-2);
		pause_menu_button3->SetText("QUIT");
		pause_menu_button3->SetPosition({ 0.0f * screen_width , -0.2f * screen_height });
		pause_menu_button3->SetColor(0xFFCF8C19);
		pause_menu_button3->LoadTexture("Assets/Sign.png");
		pause_menu_button3->SetButtonType(BUTTON_IMAGE);
		GetPauseMenu()->AddChild(pause_menu_button3);

		std::shared_ptr<Button> tutorial_button2 = std::make_shared<Button>();
		tutorial_button2->SetName("HOWTOPLAYBUTTON");
		tutorial_button2->SetScale({ 0.125f * screen_width, 0.1f * screen_height });
		tutorial_button2->SetTextScale(0.6f);
		tutorial_button2->SetText("HOW TO PLAY");
		tutorial_button2->SetPosition({ 0.0f * screen_width , -0.f * screen_height });
		tutorial_button2->SetColor(0xFFCF8C19);
		tutorial_button2->LoadTexture("Assets/Sign.png");
		tutorial_button2->SetButtonType(BUTTON_IMAGE);
		tutorial_button2->SetButtonLayer(-2);
		GetPauseMenu()->AddChild(tutorial_button2);

	Callback unpause_callback = [](void* context) {
		(void)context;

		current_layer = -3;
		GetPauseMenu()->MoveTo(-1.f * screen_height);
	};

	pause_menu_button1->OnPressed += unpause_callback;

	Callback pause_callback = [](void* context) {
		(void)context;

		if (current_layer == -2) {
			GetPauseMenu()->MoveTo(0.f);
		}
		else if (current_layer == -3) {
			GetPauseMenu()->MoveTo(-1.f * screen_height);
		}
	};

	GetPauseMenu()->on_enable += pause_callback;

	std::weak_ptr<VictoryScreen> weak_victory_screen = GetVictoryScreen();

	Callback victory_callback = [weak_victory_screen](void* context) {
		(void)context;
		current_layer = 0;

		auto children_copy = GetWorldBranch()->GetChildren();
		for (auto world_child : children_copy) {
			std::shared_ptr<RectPlayer> platform = std::dynamic_pointer_cast<RectPlayer>(world_child);

			if (platform && (platform->GetName() == "MapObject" || platform->GetName() == "SPIKE")) {
				platform->Exit();
				GetWorldBranch()->RemoveChild(platform);
				continue;
			}

			std::shared_ptr<EnemyHeaderClass> enemy = std::dynamic_pointer_cast<EnemyHeaderClass>(world_child);

			if (enemy && enemy->GetName() == "Enemy Boss") {
				enemy->active = false;
				enemy->SetPosition({ 0.0f, 10260.0f });
				continue;
			}
			else if (enemy && enemy->GetName() != "Enemy Boss") {
				enemy->Exit();
				GetWorldBranch()->RemoveChild(enemy);
				continue;
			}

			std::shared_ptr<ImageRenderer2D> powerup = std::dynamic_pointer_cast<ImageRenderer2D>(world_child);

			if (powerup && powerup->GetName() == "POWERUP") {
				powerup->Exit();
				GetWorldBranch()->RemoveChild(powerup);
				continue;
			}
		}

		auto map_children_copy = GetMapContainer()->GetChildren();

		for (auto map_child : map_children_copy) {
			std::shared_ptr<Map> map = std::dynamic_pointer_cast<Map>(map_child);

			if (map) {
				map->Exit();
				GetMapContainer()->RemoveChild(map);
			}
		}

		auto player_children_copy = GetPlayerBranch()->GetChildren();
		for (auto player_child : player_children_copy) {
			std::shared_ptr<Player> player = std::dynamic_pointer_cast<Player>(player_child);

			if (player && player != GetPlayer()) {
				player->Exit();
				GetPlayerBranch()->RemoveChild(player);
			}
		}

		GetPlayer()->SetPosition({ 0.0f, -250.0f });
		GetPlayer()->GetPlayerStateMachine()->ChangeState(&GetPlayer()->GetPlayerStateMachine()->idleState);

		weak_victory_screen.lock()->SetTransparency(0.0f);

		std::shared_ptr<ImageRenderer2D> progress_bar_player = std::dynamic_pointer_cast<ImageRenderer2D>(GetPlayer()->GetChildren().at(9));

		std::shared_ptr<VerticalProgressBar> vprog_player = std::dynamic_pointer_cast<VerticalProgressBar>(GetPlayer()->GetChildren().at(8));

		progress_bar_player->SetTransparency(0.f);
		vprog_player->SetTransparency(0.f);

		GetMainMenuUIContainer()->MoveTo(0.0f);
		GetMultiplayerUIContainer()->MoveTo(screen_height);
		GetLobbyUIContainer()->MoveTo(screen_height);
		GetCurrentLobbyUI()->MoveTo(screen_height);
		GetCamera()->SetParent(GetWorldBranch());
		GetCamera()->SetPosition({ 0.0f, -2600.0f });

		if (GetNetworkClient()->player_id >= 0) {
			GetNetworkClient()->Exit();
		}

		AEAudioStopGroup(GetAudioDriver()->music_group);

		AEAudioPlay(GetAudioDriver()->menu, GetAudioDriver()->music_group, 2.f, 1.f, 1);

		GetPauseMenu()->MoveTo(-1.f * screen_height);

		current_layer = 0;
	};

	GetVictoryScreen()->OnPressed += victory_callback;

	pause_menu_button2->OnPressed += victory_callback;

		std::shared_ptr<Button> title = std::make_shared<Button>();
		title->SetButtonLayer(20);
		title->SetName("TITLE");
		title->SetScale({ 0.4f * screen_width, 0.4f * screen_height });
		title->LoadTexture("Assets/GameTitle.png");
		title->SetPosition({ 0.0f * screen_width, 0.3f * screen_height });
		title->SetButtonType(BUTTON_IMAGE);
		GetMainMenuUIContainer()->AddChild(title);

		std::shared_ptr<Button> play_button = std::make_shared<Button>();
		play_button->SetName("PLAYBUTTON");
		play_button->SetScale({ 0.125f * screen_width, 0.1f * screen_height });
		play_button->SetTextScale(0.6f);
		play_button->SetText("PLAY");
		play_button->SetPosition({ 0.0f * screen_width , 0.1f * screen_height });
		play_button->SetColor(0xFFCF8C19);
		play_button->LoadTexture("Assets/Sign.png");
		play_button->SetButtonType(BUTTON_IMAGE);
		GetMainMenuUIContainer()->AddChild(play_button);

		std::shared_ptr<Button> multiplayer_button = std::make_shared<Button>();
		multiplayer_button->SetName("MULTIPLAYERBUTTON");
		multiplayer_button->SetScale({ 0.125f * screen_width, 0.1f * screen_height });
		multiplayer_button->SetTextScale(0.6f);
		multiplayer_button->SetText("MULTIPLAYER");
		multiplayer_button->SetPosition({ 0.0f * screen_width , 0.f * screen_height });
		multiplayer_button->SetColor(0xFFCF8C19);
		multiplayer_button->LoadTexture("Assets/Sign.png");
		multiplayer_button->SetButtonType(BUTTON_IMAGE);
		GetMainMenuUIContainer()->AddChild(multiplayer_button);

		std::shared_ptr<Button> credit_button = std::make_shared<Button>();
		credit_button->SetName("CREDITBUTTON");
		credit_button->SetScale({0.125f * screen_width, 0.1f * screen_height});
		credit_button->SetTextScale(0.6f);
		credit_button->SetText("CREDITS");
		credit_button->SetPosition({0.0f * screen_width, -0.1f * screen_height});
		credit_button->SetColor(0xFFCF8C19);
		credit_button->LoadTexture("Assets/Sign.png");
		credit_button->SetButtonType(BUTTON_IMAGE);
		GetMainMenuUIContainer()->AddChild(credit_button);

		std::shared_ptr<Button> tutorial_button = std::make_shared<Button>();
		tutorial_button->SetName("HOWTOPLAYBUTTON");
		tutorial_button->SetScale({ 0.125f * screen_width, 0.1f * screen_height });
		tutorial_button->SetTextScale(0.6f);
		tutorial_button->SetText("HOW TO PLAY");
		tutorial_button->SetPosition({ 0.0f * screen_width , -0.2f * screen_height });
		tutorial_button->SetColor(0xFFCF8C19);
		tutorial_button->LoadTexture("Assets/Sign.png");
		tutorial_button->SetButtonType(BUTTON_IMAGE);
		GetMainMenuUIContainer()->AddChild(tutorial_button);

		std::shared_ptr<Button> setting_button = std::make_shared<Button>();
		setting_button->SetName("SETTINGBUTTON");
		setting_button->SetScale({ 0.125f * screen_width, 0.1f * screen_height });
		setting_button->SetTextScale(0.6f);
		setting_button->SetText("SETTINGS");
		setting_button->SetPosition({ 0.0f * screen_width , -0.3f * screen_height });
		setting_button->SetColor(0xFFCF8C19);
		setting_button->LoadTexture("Assets/Sign.png");
		setting_button->SetButtonType(BUTTON_IMAGE);
		GetMainMenuUIContainer()->AddChild(setting_button);

		std::shared_ptr<Button> exit_button = std::make_shared<Button>();
		exit_button->SetName("EXITBUTTON");
		exit_button->SetScale({ 0.125f * screen_width, 0.1f * screen_height });
		exit_button->SetTextScale(0.6f);
		exit_button->SetText("EXIT");
		exit_button->SetPosition({ 0.0f * screen_width , -0.4f * screen_height });
		exit_button->SetColor(0xFFCF8C19);
		exit_button->LoadTexture("Assets/Sign.png");
		exit_button->SetButtonType(BUTTON_IMAGE);
		GetMainMenuUIContainer()->AddChild(exit_button);

		/*std::shared_ptr<Button> quick_play_button = std::make_shared<Button>();
		quick_play_button->SetName("QUICKPLAYBUTTON");
		quick_play_button->SetScale({ 0.125f * screen_width, 0.06f * screen_height });
		quick_play_button->SetTextScale(0.5f);
		quick_play_button->SetText("Quick Play");
		quick_play_button->SetPosition({ 0.15f * screen_width , 0.42f * screen_height });
		quick_play_button->SetColor(0xFFCF8C19);
		GetMultiplayerUIContainer()->AddChild(quick_play_button);*/

		std::shared_ptr<Button> multiplayer_return_button = std::make_shared<Button>();
		multiplayer_return_button->SetName("MULTIPLAYERBACK");
		multiplayer_return_button->SetScale({ 0.06f * screen_height, 0.06f * screen_height });
		multiplayer_return_button->SetText("");
		multiplayer_return_button->SetPosition({ -0.22f * screen_width, 0.42f * screen_height });
		multiplayer_return_button->LoadTexture("Assets/Back_Button.png");
		multiplayer_return_button->SetButtonType(BUTTON_IMAGE);
		GetMultiplayerUIContainer()->AddChild(multiplayer_return_button);


		std::shared_ptr<Text2D> name_hint = std::make_shared<Text2D>();
		name_hint->SetName("PLAYERNAME");
		name_hint->SetText("Nickname:");
		name_hint->SetScale({ 0.5f, 0.5f });
		name_hint->SetPosition({ -0.15f * screen_width, 0.42f * screen_height });
		GetMultiplayerUIContainer()->AddChild(name_hint);

		std::shared_ptr<TextBox> player_name = GetPlayerName();
		player_name->SetName("PLAYERNAMETEXTBOX");
		player_name->SetScale({ 0.15f * screen_width, 0.06f * screen_height });
		player_name->SetTextScale(0.5f);
		player_name->SetText("PLAYER");
		player_name->SetDefaultText("PLAYER");
		player_name->SetPosition({ -0.025f * screen_width , 0.42f * screen_height });
		player_name->SetColor(0xFF4F4F4F);
		GetMultiplayerUIContainer()->AddChild(player_name);
		
		std::shared_ptr<Button> create_lobby_button = std::make_shared<Button>();
		create_lobby_button->SetName("CREATELOBBYBUTTON");
		create_lobby_button->SetScale({ 0.125f * screen_width, 0.06f * screen_height });
		create_lobby_button->SetTextScale(0.5f);
		create_lobby_button->SetText("Create Lobby");
		create_lobby_button->SetPosition({ 0.15f * screen_width , -0.4f * screen_height });
		create_lobby_button->SetColor(0xFFCF8C19);
		GetMultiplayerUIContainer()->AddChild(create_lobby_button);

		std::shared_ptr<Text2D> lobby_hint = std::make_shared<Text2D>();
		lobby_hint->SetName("LOBBYNAME");
		lobby_hint->SetText("Lobby Name:");
		lobby_hint->SetScale({ 0.5f, 0.5f });
		lobby_hint->SetPosition({ -0.15f * screen_width, -0.4f * screen_height });
		GetMultiplayerUIContainer()->AddChild(lobby_hint);

		std::shared_ptr<TextBox> create_lobby_text_box = std::make_shared<TextBox>();
		std::weak_ptr<TextBox> weak_create_lobby_text_box = create_lobby_text_box;
		create_lobby_text_box->SetName("CREATELOBBYTEXTBOX");
		create_lobby_text_box->SetScale({ 0.15f * screen_width, 0.06f * screen_height });
		create_lobby_text_box->SetTextScale(0.5f);
		create_lobby_text_box->SetText("LOBBYNAME");
		create_lobby_text_box->SetDefaultText("LOBBYNAME");
		create_lobby_text_box->SetPosition({ -0.025f * screen_width , -0.4f * screen_height });
		create_lobby_text_box->SetColor(0xFF4F4F4F);
		GetMultiplayerUIContainer()->AddChild(create_lobby_text_box);

		std::shared_ptr<Rect2D> lobby_ui_container_bg = std::make_shared<Rect2D>();
		lobby_ui_container_bg->SetColor(0x4F000000);
		lobby_ui_container_bg->SetPosition({ 0.0f, 0.0125f * screen_height });
		lobby_ui_container_bg->SetScale({ 0.475f * screen_width, 0.95f * screen_height });
		GetLobbyUIContainer()->AddChild(lobby_ui_container_bg);

		std::shared_ptr<UIContainer> lobby_container = std::make_shared<UIContainer>();
		GetLobbyUIContainer()->AddChild(lobby_container);

		std::shared_ptr<ErrorButton> error_ui_button = std::make_shared<ErrorButton>();
		error_ui_button->SetButtonLayer(1);
		error_ui_button->SetColor(0x7F0F0F0F);
		error_ui_button->SetScale({ 1.0f * screen_width, 1.0f * screen_height });
		error_ui_button->SetText("Connecting...");
		error_ui_button->SetTextScale(1.0f);
		error_ui_button->SetTransparency(0.0f);
		error_ui_button->SetPosition({ 0.0f, 0.0f });
		std::weak_ptr<ErrorButton> weak_error_button = error_ui_button;
		Callback error_callback = [weak_error_button](void* context) {
			(void)context;
			weak_error_button.lock()->SetTransparency(0.0f);
			weak_error_button.lock()->ResetErrorTimer();
			current_layer = 0;

			GetMainMenuUIContainer()->MoveTo(0.0f);
			GetMultiplayerUIContainer()->MoveTo(screen_height);
			GetLobbyUIContainer()->MoveTo(screen_height);
			GetCurrentLobbyUI()->MoveTo(screen_height);
			GetCamera()->SetParent(GetWorldBranch());
			GetCamera()->SetPosition({ 0.0f, -2600.0f });

			GetNetworkClient()->Exit();
		};
		error_ui_button->OnPressed += error_callback;
		GetErrorUIContainer()->AddChild(error_ui_button);

		std::shared_ptr<Rect2D> lobby_ui_bg = std::make_shared<Rect2D>();
		lobby_ui_bg->SetColor(0x4F000000);
		lobby_ui_bg->SetPosition({ 0.0f, -0.05f * screen_height });
		lobby_ui_bg->SetScale({ 0.45f * screen_width, 0.8f * screen_height });
		GetCurrentLobbyUI()->AddChild(lobby_ui_bg);

		std::shared_ptr<UIContainer> players_ui_container = std::make_shared<UIContainer>();
		GetCurrentLobbyUI()->AddChild(players_ui_container);

		std::shared_ptr<Button> lobby_return_button = std::make_shared<Button>();
		lobby_return_button->SetName("LOBBYBACK");
		lobby_return_button->SetScale({ 0.06f * screen_height, 0.06f * screen_height });
		lobby_return_button->SetText("");
		lobby_return_button->SetPosition({ -0.22f * screen_width, 0.42f * screen_height });
		lobby_return_button->LoadTexture("Assets/Back_Button.png");
		lobby_return_button->SetButtonType(BUTTON_IMAGE);
		GetCurrentLobbyUI()->AddChild(lobby_return_button);

		std::shared_ptr<Button> ready_button = std::make_shared<Button>();
		ready_button->SetName("READYBUTTON");
		ready_button->SetScale({ 0.125f * screen_width, 0.06f * screen_height });
		ready_button->SetTextScale(0.5f);
		ready_button->SetText("READY");
		ready_button->SetPosition({ 0.0f , -0.4f * screen_height });
		ready_button->SetColor(0xFFCF8C19);
		GetCurrentLobbyUI()->AddChild(ready_button);

		std::shared_ptr<Button> settings_bg = std::make_shared<Button>();
		settings_bg->SetScale({ 0.5f * screen_width, 1.0f * screen_height });
		settings_bg->LoadTexture("Assets/Sign.png");
		settings_bg->SetButtonType(BUTTON_IMAGE);
		settings_bg->SetButtonLayer(20);
		GetSettingUIContainer()->AddChild(settings_bg);

		std::shared_ptr<InputMapping> inputmapping = std::make_shared<InputMapping>();
		std::weak_ptr<InputMapping> weak_inputmapping = inputmapping;
		inputmapping->SetScale({ 0.45f * screen_width, 0.5f * screen_height });
		GetSettingUIContainer()->AddChild(inputmapping);

		std::shared_ptr<Credits> credits_screen = std::make_shared<Credits>();
		std::weak_ptr<Credits> weak_credits = credits_screen;
		credits_screen->SetScale({ 0.45f * screen_width, 0.5f * screen_height });
		GetCreditUIContainer()->AddChild(credits_screen);

		std::shared_ptr<Button> credits_return_button = std::make_shared<Button>();
		credits_return_button->SetName("CREDITSBACK");
		credits_return_button->SetScale({ 0.06f * screen_height, 0.06f * screen_height });
		credits_return_button->SetText("");
		credits_return_button->SetPosition({ -0.22f * screen_width, 0.42f * screen_height });
		credits_return_button->LoadTexture("Assets/Back_Button.png");
		credits_return_button->SetButtonType(BUTTON_IMAGE);
		GetCreditUIContainer()->AddChild(credits_return_button);

		std::shared_ptr<Tutorial> tutorial = std::make_shared<Tutorial>();
		std::weak_ptr<Tutorial> weak_tutorial = tutorial;
		AEVec2 tutorial_position{0.0f,- 0.5f * screen_height};
		tutorial->SetPosition(tutorial_position);
		tutorial->SetScale({ 0.5f * screen_width, 2.0f * screen_height });	// set background for tutorial
		GetTutorialUIContainer()->AddChild(tutorial);


	// Root\fourth_child
	// Reserved for Editor ONLY.
	std::shared_ptr<Editor2D> fourth_child = std::make_shared<Editor2D>();
	fourth_child->SetName("EDITOR");
	GetRoot()->AddChild(fourth_child);
	editor = fourth_child;

        std::shared_ptr<Rect2D> rect_test = std::make_shared<Rect2D>();
		rect_test->SetName("HIERARCHY");
        rect_test->SetScale({ 400.0f, 900.0f });
		rect_test->SetPosition({ 0.375f * screen_width, 0.0f });
		rect_test->SetColor(0x5F000000);
        fourth_child->AddChild(rect_test);

		std::shared_ptr<Rect2D> rect_test_2 = std::make_shared<Rect2D>();
		rect_test_2->SetName("INSPECTOR");
		rect_test_2->SetScale({ 400.0f, 900.0f });
		rect_test_2->SetPosition({ -0.375f * screen_width, 0.0f });
		rect_test_2->SetColor(0x5F000000);
		fourth_child->AddChild(rect_test_2);

		std::shared_ptr<Text2D> text_test = std::make_shared<Text2D>();
		text_test->SetName("HIERARCHYNAME");
		text_test->SetText("Hierarchy");
		text_test->SetScale({ 0.5f, 0.5f });
		text_test->SetPosition({ -0.47f * screen_width, 0.45f * screen_height });
		fourth_child->AddChild(text_test);

		std::shared_ptr<Text2D> text_test_2 = std::make_shared<Text2D>();
		text_test_2->SetName("INSPECTORNAME");
		text_test_2->SetText("Inspector");
		text_test_2->SetScale({ 0.5f, 0.5f });
		text_test_2->SetPosition({ 0.3f * screen_width, 0.45f * screen_height });
		fourth_child->AddChild(text_test_2);

	GetRoot()->AddChild(GetLobbyDetails());

	GetRoot()->AddChild(GetAudioDriver());

	VictoryZone()->SetPosition({ 0.f, 13000.0f });
	GetRoot()->AddChild(VictoryZone());

	std::shared_ptr<DigipenLogo> digipen_logo = std::make_shared<DigipenLogo>();
	digipen_logo->LoadTexture("Assets/Copyright_DigiPen_BLACK.png");
	digipen_logo->SetScale({ 1.f * screen_width, screen_height });
	digipen_logo->SetTimerDuration(5.f);
	digipen_logo->SetTransparency(0.f);
	GetRoot()->AddChild(digipen_logo);

	Callback create_lobby_callback = [weak_error_button, weak_create_lobby_text_box](void* context) { 
		(void)context;

		if (AEAudioIsValidAudio(GetAudioDriver()->confirm)) {
			AEAudioPlay(GetAudioDriver()->confirm, GetAudioDriver()->sfx_group, 2.f, 1.f, 0);
		}
		else {
			std::cout << "INVALID AUDIO!" << std::endl;
		}

		GetNetworkClient()->CreateLobby(weak_create_lobby_text_box.lock()->GetText(), GetPlayerName()->GetText()); 

		weak_error_button.lock()->SetErrorText("Creating Lobby");

		current_layer = 2;
	};

	Callback exit_callback = [](void* context) { 
		(void)context;
		game_running = false; 
	};

	Callback change_to_game = [weak_input_manager](void* context) {
		(void)context;

		if (AEAudioIsValidAudio(GetAudioDriver()->confirm)) {
			AEAudioPlay(GetAudioDriver()->confirm, GetAudioDriver()->sfx_group, 2.f, 1.f, 0);
		}
		else {
			std::cout << "INVALID AUDIO!" << std::endl;
		}

		GetCamera()->SetParent(GetPlayer());

		GetMainMenuUIContainer()->MoveTo(-screen_height);

		std::shared_ptr<Map> map = std::make_shared<Map>();
		std::vector<Map::MapObject> mapObj = map->CreateSeed ( - 200, -50, -350, 10000, 250, 300, 100, 100.0f, 200.0f);
		map->mapObj = mapObj;
		map->CreateMap(map->mapObj);

		std::shared_ptr<Map> map2 = std::make_shared<Map>();
		std::vector<Map::MapObject> mapObj2 = map->CreateSeed(50, 200, -325, 10000, 250, 300, 100, 100.0f, 200.0f);
		map2->mapObj = mapObj2;
		map2->CreateMap(map2->mapObj);

		// Left map
		std::shared_ptr<Map> map3 = std::make_shared<Map>();
		std::vector<Map::MapObject> mapObj3 = map->CreateSeed(-400, -400, -375, 10000, 250, 450, 100, 100.0f, 200.0f);
		map3->mapObj = mapObj3;
		map3->CreateMap(map3->mapObj);

		// Right map
		std::shared_ptr<Map> map4 = std::make_shared<Map>();
		std::vector<Map::MapObject> mapObj4 = map->CreateSeed(400, 400, -275, 10000, 250, 450, 100, 100.0f, 200.0f);
		map4->mapObj = mapObj4;
		map4->CreateMap(map4->mapObj);

		std::shared_ptr<AIPlayer> player2 = std::make_shared<AIPlayer>();
		player2->SetName("144");
		player2->SetPosition({ 100.0f, -255.0f });
		player2->SetScale({ 50.0f, 50.0f });
		player2->AIMap(mapObj);

			std::shared_ptr<RectPlayer> playerGroundCheck2 = std::make_shared <RectPlayer>();
			playerGroundCheck2->SetName("GROUNDCHECK");
			playerGroundCheck2->SetScale({ 50.0f, 10.0f });
			playerGroundCheck2->SetPosition({ 0.0f, -30.0f });
			playerGroundCheck2->SetColor(0x4F0000FF);
			playerGroundCheck2->SetTransparency(0.0f);
			player2->AddChild(playerGroundCheck2);

			std::shared_ptr<RectPlayer> playerCoyote2 = std::make_shared <RectPlayer>();
			playerCoyote2->SetName("COYOTE");
			playerCoyote2->SetScale({ 50.0f, 50.0f });
			playerCoyote2->SetPosition({ 0.0f, -30.0f });
			playerCoyote2->SetTransparency(0.3f);
			playerCoyote2->SetColor(0x4F00FF00);
			playerCoyote2->SetTransparency(0.0f);
			player2->AddChild(playerCoyote2);

			std::shared_ptr<RectPlayer> playerRunOffL2 = std::make_shared <RectPlayer>();
			playerRunOffL2->SetName("RUNOFFL");
			playerRunOffL2->SetScale({ 50.0f, 50.0f });
			playerRunOffL2->SetPosition({ -50.0f, -30.0f });
			playerRunOffL2->SetTransparency(0.3f);
			playerRunOffL2->SetColor(0x4FFF00FF);
			playerRunOffL2->SetTransparency(0.0f);
			player2->AddChild(playerRunOffL2);

			std::shared_ptr<RectPlayer> playerRunOffR2 = std::make_shared <RectPlayer>();
			playerRunOffR2->SetName("RUNOFFR");
			playerRunOffR2->SetScale({ 50.0f, 50.0f });
			playerRunOffR2->SetPosition({ 50.0f, -30.0f });
			playerRunOffR2->SetTransparency(0.3f);
			playerRunOffR2->SetColor(0x4FFF00FF);
			playerRunOffR2->SetTransparency(0.0f);
			player2->AddChild(playerRunOffR2);

			std::shared_ptr<SpriteRenderer> playerMesh2 = std::make_shared <SpriteRenderer>();
			playerMesh2->SetName("MESH");
			playerMesh2->SetScale({ 50.0f, 50.0f });
			playerMesh2->SetPosition({ 0.0f, 0.0f });
			playerMesh2->LoadTexture("Assets/JackedJack_SpriteSheet1.png");
			playerMesh2->LoadSecondaryMesh("Assets/JackedJack_SpriteSheet2.png");
			playerMesh2->SetFPS(40.0f);
			playerMesh2->SetSpriteTiling(0.0625f, 0.0625f);
			playerMesh2->SetDrawScale(1.8f);
			playerMesh2->SetSpaceType(WORLD_SPACE);
			player2->AddChild(playerMesh2);

			std::shared_ptr<AIInputManager> inputManager2 = std::make_shared <AIInputManager>();
			std::weak_ptr<AIInputManager> weak_input_manager2 = inputManager2;
			inputManager2->active = true;
			inputManager2->SetName("AI Input Manager");
			player2->AddChild(inputManager2);

			std::shared_ptr<Text2D> nametag = std::make_shared<Text2D>();
			nametag->SetText("AIPLAYER");
			nametag->SetScale({ 0.5f, 0.5f });
			nametag->SetPosition({ 0.0f, 60.0f });
			nametag->SetSpaceType(WORLD_SPACE);
			player2->AddChild(nametag);

			std::shared_ptr<PieTimer> jump_timer = std::make_shared<PieTimer>();
			jump_timer->SetName("PIETIMER");
			jump_timer->SetScale({ 10.0f, 10.0f });
			jump_timer->SetPosition({ 45.0f, 0.0f });
			jump_timer->SetColor(0xFF000000);
			jump_timer->SetTransparency(0.0f);
			player2->AddChild(jump_timer);

			std::shared_ptr<VerticalProgressBar> vprog2 = std::make_shared<VerticalProgressBar>();
			vprog2->SetName("VPROG");
			vprog2->SetScale({ 20.f, 5.f });
			vprog2->SetPosition({ 0.4f * screen_width, -0.4f * screen_height });
			vprog2->SetColor(0xFF00FF00);
			player2->AddChild(vprog2);

			std::shared_ptr<ImageRenderer2D> progress_bar2 = std::make_shared<ImageRenderer2D>();
			progress_bar2->SetName("PROGRESSBAR");
			progress_bar2->SetScale({ 32.f, 32.f });
			progress_bar2->SetPosition({ 0.4f * screen_width, -0.4f * screen_height });
			progress_bar2->LoadTexture("Assets/JackedJackIcon.png");
			progress_bar2->SetSpaceType(SCREEN_SPACE);
			player2->AddChild(progress_bar2);

		GetPlayerBranch()->AddChild(player2);

		std::shared_ptr<AIPlayer> player3 = std::make_shared<AIPlayer>();
		player3->SetName("64");
		player3->SetPosition({ 200.0f, -255.0f });
		player3->SetScale({ 50.0f, 50.0f });
		player3->AIMap(mapObj2);

			std::shared_ptr<RectPlayer> playerGroundCheck3 = std::make_shared <RectPlayer>();
			playerGroundCheck3->SetName("GROUNDCHECK");
			playerGroundCheck3->SetScale({ 50.0f, 10.0f });
			playerGroundCheck3->SetPosition({ 0.0f, -30.0f });
			playerGroundCheck3->SetColor(0x4F0000FF);
			playerGroundCheck3->SetTransparency(0.0f);
			player3->AddChild(playerGroundCheck3);

			std::shared_ptr<RectPlayer> playerCoyote3 = std::make_shared <RectPlayer>();
			playerCoyote3->SetName("COYOTE");
			playerCoyote3->SetScale({ 50.0f, 50.0f });
			playerCoyote3->SetPosition({ 0.0f, -30.0f });
			playerCoyote3->SetTransparency(0.3f);
			playerCoyote3->SetColor(0x4F00FF00);
			playerCoyote3->SetTransparency(0.0f);
			player3->AddChild(playerCoyote3);

			std::shared_ptr<RectPlayer> playerRunOffL3 = std::make_shared <RectPlayer>();
			playerRunOffL3->SetName("RUNOFFL");
			playerRunOffL3->SetScale({ 50.0f, 50.0f });
			playerRunOffL3->SetPosition({ -50.0f, -30.0f });
			playerRunOffL3->SetTransparency(0.3f);
			playerRunOffL3->SetColor(0x4FFF00FF);
			playerRunOffL3->SetTransparency(0.0f);
			player3->AddChild(playerRunOffL3);

			std::shared_ptr<RectPlayer> playerRunOffR3 = std::make_shared <RectPlayer>();
			playerRunOffR3->SetName("RUNOFFR");
			playerRunOffR3->SetScale({ 50.0f, 50.0f });
			playerRunOffR3->SetPosition({ 50.0f, -30.0f });
			playerRunOffR3->SetTransparency(0.3f);
			playerRunOffR3->SetColor(0x4FFF00FF);
			playerRunOffR3->SetTransparency(0.0f);
			player3->AddChild(playerRunOffR3);

			std::shared_ptr<SpriteRenderer> playerMesh3 = std::make_shared <SpriteRenderer>();
			playerMesh3->SetName("MESH");
			playerMesh3->SetScale({ 50.0f, 50.0f });
			playerMesh3->SetPosition({ 0.0f, 0.0f });
			playerMesh3->LoadTexture("Assets/JackedJack_Blue_SpriteSheet1.png");
			playerMesh3->LoadSecondaryMesh("Assets/JackedJack_Blue_SpriteSheet2.png");
			playerMesh3->SetFPS(40.0f);
			playerMesh3->SetSpriteTiling(0.0625f, 0.0625f);
			playerMesh3->SetDrawScale(1.8f);
			playerMesh3->SetSpaceType(WORLD_SPACE);
			player3->AddChild(playerMesh3);

			std::shared_ptr<AIInputManager> inputManager3 = std::make_shared <AIInputManager>();
			std::weak_ptr<AIInputManager> weak_input_manager3 = inputManager3;
			inputManager3->active = true;
			inputManager3->SetName("AI Input Manager");
			player3->AddChild(inputManager3);

			std::shared_ptr<Text2D> nametag2 = std::make_shared<Text2D>();
			nametag2->SetText("AIPLAYER");
			nametag2->SetScale({ 0.5f, 0.5f });
			nametag2->SetPosition({ 0.0f, 60.0f });
			nametag2->SetSpaceType(WORLD_SPACE);
			player3->AddChild(nametag2);

			std::shared_ptr<PieTimer> jump_timer2 = std::make_shared<PieTimer>();
			jump_timer2->SetName("PIETIMER");
			jump_timer2->SetScale({ 10.0f, 10.0f });
			jump_timer2->SetPosition({ 45.0f, 0.0f });
			jump_timer2->SetColor(0xFF000000);
			jump_timer2->SetTransparency(0.0f);
			player3->AddChild(jump_timer2);

			std::shared_ptr<VerticalProgressBar> vprog3 = std::make_shared<VerticalProgressBar>();
			vprog3->SetName("VPROG");
			vprog3->SetScale({ 20.f, 5.f });
			vprog3->SetPosition({ 0.35f * screen_width, -0.4f * screen_height });
			vprog3->SetColor(0xFF0000FF);
			player3->AddChild(vprog3);

			std::shared_ptr<ImageRenderer2D> progress_bar3 = std::make_shared<ImageRenderer2D>();
			progress_bar3->SetName("PROGRESSBAR");
			progress_bar3->SetScale({ 32.f, 32.f });
			progress_bar3->SetPosition({ 0.35f * screen_width, -0.4f * screen_height });
			progress_bar3->LoadTexture("Assets/JackedJackIcon.png");
			progress_bar3->SetSpaceType(SCREEN_SPACE);
			player3->AddChild(progress_bar3);

		GetPlayerBranch()->AddChild(player3);

		GetPlayer()->GetPlayerStateMachine()->active = true;

		std::shared_ptr<FinishLine> finish_line = std::make_shared<FinishLine>();
		finish_line->SetName("FINISH");
		finish_line->SetScale({ screen_width, 2600.f });
		finish_line->SetPosition({ 0.f, 11510.f });
		GetWorldBranch()->AddChild(finish_line);

		weak_input_manager.lock()->active = true;

		GetCountdownTimer()->SetCountdown(5.f);
		GetCountdownTimer()->StartCountdown();

		AEAudioStopGroup(GetAudioDriver()->music_group);

		AEAudioPlay(GetAudioDriver()->game, GetAudioDriver()->music_group, 2.f, 1.f, 1);

		current_layer = -3;
	};

	Callback change_to_inputmapping = [weak_inputmapping](void* context) {
		(void)context;
		weak_inputmapping.lock()->SettingState = true;

		if (AEAudioIsValidAudio(GetAudioDriver()->confirm)) {
			AEAudioPlay(GetAudioDriver()->confirm, GetAudioDriver()->sfx_group, 2.f, 1.f, 0);
		}
		else {
			std::cout << "INVALID AUDIO!" << std::endl;
		}

		GetSettingUIContainer()->MoveTo(0.0f);
		GetMainMenuUIContainer()->MoveTo(screen_height);
	};
	
	Callback change_to_credits = [weak_credits](void* context) {
		(void)context;
		weak_credits.lock()->CreditsState = true;

		if (AEAudioIsValidAudio(GetAudioDriver()->confirm)) {
			AEAudioPlay(GetAudioDriver()->confirm, GetAudioDriver()->sfx_group, 2.f, 1.f, 0);
		}
		else {
			std::cout << "INVALID AUDIO!" << std::endl;
		}

		GetCreditUIContainer()->MoveTo(0.0f);
		GetCreditUIContainer()->roll_credits = true;
		GetMainMenuUIContainer()->MoveTo(screen_height);
	};

	Callback change_to_tutorial = [weak_tutorial](void* context) {
		(void)context;
		weak_tutorial.lock()->TutorialState = true;

		if (AEAudioIsValidAudio(GetAudioDriver()->confirm)) {
			AEAudioPlay(GetAudioDriver()->confirm, GetAudioDriver()->sfx_group, 2.f, 1.f, 0);
		}
		else {
			std::cout << "INVALID AUDIO!" << std::endl;
		}

		if (current_layer == -2) {
			GetTutorialUIContainer()->MoveTo(0.f);
			GetPauseMenu()->MoveTo(-1.f * screen_height);
		}
		else {
			GetTutorialUIContainer()->MoveTo(0.0f);
			GetMainMenuUIContainer()->MoveTo(screen_height);
		}
	};


	Callback change_to_multiplayer = [weak_error_button](void* context) { 
		(void)context;

		GetNetworkClient()->StartClient(context);

		if (AEAudioIsValidAudio(GetAudioDriver()->confirm)) {
			AEAudioPlay(GetAudioDriver()->confirm, GetAudioDriver()->sfx_group, 2.f, 1.f, 0);
		}
		else {
			std::cout << "INVALID AUDIO!" << std::endl;
		}

		weak_error_button.lock()->SetErrorText("Connecting");

		current_layer = 2;
	};

	Callback back_to_menu = [](void* context) {
		(void)context;

		if (AEAudioIsValidAudio(GetAudioDriver()->cancel)) {
			AEAudioPlay(GetAudioDriver()->cancel, GetAudioDriver()->sfx_group, 2.f, 1.f, 0);
		}
		else {
			std::cout << "INVALID AUDIO!" << std::endl;
		}

		GetMainMenuUIContainer()->MoveTo(0.0f);
		GetMultiplayerUIContainer()->MoveTo(screen_height);
		GetLobbyUIContainer()->MoveTo(screen_height);

		GetNetworkClient()->Exit();

		GetCamera()->SetParent(GetWorldBranch());
		GetCamera()->SetPosition({ 0.0f, -2600.0f });
	};

	Callback leave_lobby = [weak_error_button](void* context) {
		(void)context;

		if (AEAudioIsValidAudio(GetAudioDriver()->cancel)) {
			AEAudioPlay(GetAudioDriver()->cancel, GetAudioDriver()->sfx_group, 2.f, 1.f, 0);
		}
		else {
			std::cout << "INVALID AUDIO!" << std::endl;
		}

		GetCurrentLobbyUI()->MoveTo(screen_height);
		GetMultiplayerUIContainer()->MoveTo(0.0f);
		GetLobbyUIContainer()->MoveTo(0.0f);

		GetNetworkClient()->LeaveLobby();

		weak_error_button.lock()->SetErrorText("Leaving Lobby");

		current_layer = 2;
	};

	Callback on_ready = [weak_error_button](void* context) {
		(void)context;

		if (AEAudioIsValidAudio(GetAudioDriver()->confirm)) {
			AEAudioPlay(GetAudioDriver()->confirm, GetAudioDriver()->sfx_group, 2.f, 1.f, 0);
		}
		else {
			std::cout << "INVALID AUDIO!" << std::endl;
		}

		GetNetworkClient()->ReadyPlayer();

		weak_error_button.lock()->SetErrorText("Readying");

		current_layer = 2;
	};

	ready_button->OnPressed += on_ready;

	lobby_return_button->OnPressed += leave_lobby;

	multiplayer_return_button->OnPressed += back_to_menu;

	credits_return_button->OnPressed += back_to_menu;

	create_lobby_button->OnPressed += create_lobby_callback;

	play_button->OnPressed += change_to_game;
	multiplayer_button->OnPressed += change_to_multiplayer;
	exit_button->OnPressed += exit_callback;
	pause_menu_button3->OnPressed += exit_callback;
	setting_button->OnPressed += change_to_inputmapping;
	credit_button->OnPressed += change_to_credits;
	tutorial_button->OnPressed += change_to_tutorial;
	tutorial_button2->OnPressed += change_to_tutorial;
}



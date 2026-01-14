
/* Start Header ************************************************************************/
/*!
\file		PlayerManager.cpp
\author		Tai Xiao Xuan
\par		xiaoxuan.tai@digipen.edu
\date		09/03/2025
\brief		This file implements a PlayerManager class, that handles interfacing with the NetworkClient to spawn server players.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#include "GameDefn.hpp"
#include "PlayerMovementStateMachine.hpp"

void PlayerManager::Update() {

	if (on_player_added) {
		std::shared_ptr<Text2D> ref_nametag = std::dynamic_pointer_cast<Text2D>(GetPlayer()->GetChildren().at(6));
		
		current_layer = -3;

		if (ref_nametag) {
			ref_nametag->SetText(GetPlayerName()->GetText());
		}

		std::shared_ptr<FinishLine> finish_line = std::make_shared<FinishLine>();
		finish_line->SetName("FINISH");
		finish_line->SetScale({ screen_width, 2600.f });
		finish_line->SetPosition({ 0.f, 11510.f });
		GetWorldBranch()->AddChild(finish_line);
		finish_line->Update();

		GetPlayer()->GetPlayerStateMachine()->active = true;

		int i{};

		for (auto& details : GetLobbyDetails()->details) {
			std::stringstream stream;
			stream << details.player_id;

			if (details.player_id == GetNetworkClient()->player_id) {
				std::stringstream ss;
				ss << GetNetworkClient()->player_id;

				GetPlayer()->SetName(ss.str());
				continue;
			}

			if (details.player_id <= -1) {
				continue;
			}

			std::shared_ptr<ServerPlayer> player2 = std::make_shared<ServerPlayer>();
			player2->SetName(stream.str());
			player2->SetPosition({ 0.0f, -250.0f });
			player2->SetScale({ 50.0f, 50.0f });

			std::shared_ptr<RectPlayer> playerGroundCheck2 = std::make_shared <RectPlayer>();
			playerGroundCheck2->SetName("GROUNDCHECK");
			playerGroundCheck2->SetScale({ 50.0f, 10.0f });
			playerGroundCheck2->SetPosition({ 0.0f, -30.0f });
			playerGroundCheck2->SetColor(0x4F0000FF);
			playerGroundCheck2->SetTransparency(0.f);
			player2->AddChild(playerGroundCheck2);

			std::shared_ptr<RectPlayer> playerCoyote2 = std::make_shared <RectPlayer>();
			playerCoyote2->SetName("COYOTE");
			playerCoyote2->SetScale({ 50.0f, 50.0f });
			playerCoyote2->SetPosition({ 0.0f, -30.0f });
			playerCoyote2->SetTransparency(0.0f);
			playerCoyote2->SetColor(0x4F00FF00);
			player2->AddChild(playerCoyote2);

			std::shared_ptr<RectPlayer> playerRunOffL2 = std::make_shared <RectPlayer>();
			playerRunOffL2->SetName("RUNOFFL");
			playerRunOffL2->SetScale({ 50.0f, 50.0f });
			playerRunOffL2->SetPosition({ -50.0f, -30.0f });
			playerRunOffL2->SetTransparency(0.0f);
			playerRunOffL2->SetColor(0x4FFF00FF);
			player2->AddChild(playerRunOffL2);

			std::shared_ptr<RectPlayer> playerRunOffR2 = std::make_shared <RectPlayer>();
			playerRunOffR2->SetName("RUNOFFR");
			playerRunOffR2->SetScale({ 50.0f, 50.0f });
			playerRunOffR2->SetPosition({ 50.0f, -30.0f });
			playerRunOffR2->SetTransparency(0.0f);
			playerRunOffR2->SetColor(0x4FFF00FF);
			player2->AddChild(playerRunOffR2);

			std::shared_ptr<SpriteRenderer> playerMesh3 = std::make_shared <SpriteRenderer>();
			playerMesh3->SetName("MESH");
			playerMesh3->SetScale({ 50.0f, 50.0f });
			playerMesh3->SetPosition({ 0.0f, 0.0f });
			playerMesh3->LoadTexture("Assets/JackedJack_SpriteSheet1.png");
			playerMesh3->LoadSecondaryMesh("Assets/JackedJack_SpriteSheet2.png");
			playerMesh3->SetFPS(40.0f);
			playerMesh3->SetSpriteTiling(0.0625f, 0.0625f);
			playerMesh3->SetDrawScale(1.8f);
			playerMesh3->SetSpaceType(WORLD_SPACE);
			player2->AddChild(playerMesh3);

			std::shared_ptr<ServerInputManager> server_input = std::make_shared<ServerInputManager>();
			server_input->SetName("INPUT");
			player2->AddChild(server_input);

			std::shared_ptr<Text2D> nametag = std::make_shared<Text2D>();
			nametag->SetText(details.player_name);
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
			vprog2->SetPosition({ 0.4f * screen_width + i * (0.05f * screen_width), -0.4f * screen_height });
			vprog2->SetColor(0xFF00FF00);
			player2->AddChild(vprog2);

			std::shared_ptr<ImageRenderer2D> progress_bar2 = std::make_shared<ImageRenderer2D>();
			progress_bar2->SetName("PROGRESSBAR");
			progress_bar2->SetScale({ 32.f, 32.f });
			progress_bar2->SetPosition({ 0.4f * screen_width + i * (0.05f * screen_width), -0.4f * screen_height });
			progress_bar2->LoadTexture("Assets/JackedJackIcon.png");
			progress_bar2->SetSpaceType(SCREEN_SPACE);
			player2->AddChild(progress_bar2);

			GetPlayerBranch()->AddChild(player2);
			server_input->active = true;

			i++;
		}

		std::shared_ptr<InputManager> input_manager = std::dynamic_pointer_cast<InputManager>(GetPlayer()->GetChildren().at(5));

		if (input_manager) {
			input_manager->active = true;
		}

		on_player_added = false;

		AEAudioStopGroup(GetAudioDriver()->music_group);

		AEAudioPlay(GetAudioDriver()->game, GetAudioDriver()->music_group, 2.f, 1.f, 1);
	}

	Object2D::Update();
}
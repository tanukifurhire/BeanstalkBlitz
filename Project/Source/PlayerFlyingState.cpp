/* Start Header ************************************************************************/
/*!
\file		PlayerFlyingState.cpp
\author		Tai Xiao Xuan
\par		xiaoxuan.tai@digipen.edu
\date		06/04/2025
\brief		This file implements a FlyingState class, that handles player Flying logic. From Powerups

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#include "PlayerFlyingState.hpp"
#include "PlayerMovementStateMachine.hpp"
#include "Player.hpp"
#include "GameDefn.hpp"

void PlayerFlyingState::Enter() {
	state_machine->player.lock()->SetSpeed(state_machine->player.lock()->GetMaxSpeed());
	
	state_machine->player.lock()->SetJumpForce(state_machine->player.lock()->GetMaxJumpForce());

	std::shared_ptr<SpriteRenderer> sprite = std::dynamic_pointer_cast<SpriteRenderer>(state_machine->player.lock()->GetChildren().at(4));

	if (sprite) {
		sprite->SetState(&sprite->jumpState);
	}

	if (state_machine->player.lock() == GetPlayer()) {
		AEAudioPlay(GetAudioDriver()->boosting, GetAudioDriver()->sfx_group, 2.f, 1.f, 0);
	}

	PlayerAirborneState::Enter();
}

void PlayerFlyingState::HandleInput() {

	state_machine->player.lock()->velocity.x += state_machine->player.lock()->player_input.x * state_machine->player.lock()->GetSpeed() * deltaTime;

	f32 jumpForce = state_machine->player.lock()->GetJumpForce();

	state_machine->player.lock()->velocity.y += AEClamp(jumpForce * deltaTime, 0.0f, 2000.0f * deltaTime);

	if (time_in_state >= 0.75f * time_max) {
		jumpForce -= 5000.0f * deltaTime;
	}

	state_machine->player.lock()->SetJumpForce(jumpForce);

	PlayerAirborneState::HandleInput();
}

void PlayerFlyingState::Update() {

	AABB left_wall{ {-screen_width, -99999.f},{world_boundary_x_min, 99999.f} };
	AABB right_wall{ {world_boundary_x_max, -99999.f},{screen_width, 99999.f} };

	f32 tFirst{ 0.0f };

	AEVec2 offset = state_machine->player.lock()->GetLocalPosition();

	AEVec2 calculated_position{ 0.0f, 0.0f };

	AEVec2Add(&calculated_position, &offset, &state_machine->player.lock()->velocity);

	AABB calculated_bounding_box{ {calculated_position.x - state_machine->player.lock()->GetScale().x / 2, calculated_position.y - state_machine->player.lock()->GetScale().y / 2},{calculated_position.x + state_machine->player.lock()->GetScale().x / 2, calculated_position.y + state_machine->player.lock()->GetScale().y / 2} };

	if (CollisionIntersection_RectRect(state_machine->player.lock()->bounding_box, state_machine->player.lock()->velocity, left_wall, { 0.0f, 0.0f }, tFirst, deltaTime)) {
		if (!CollisionIntersection_RectRect(calculated_bounding_box, { 0.0f, 0.0f }, left_wall, { 0.0f, 0.0f }, tFirst, deltaTime)) {
			offset.x += state_machine->player.lock()->velocity.x * deltaTime;

			state_machine->player.lock()->SetPosition(offset);
		}
		else {
			offset.x += state_machine->player.lock()->velocity.x * tFirst;

			state_machine->player.lock()->SetPosition(offset);
		}
	}
	else if (CollisionIntersection_RectRect(state_machine->player.lock()->bounding_box, state_machine->player.lock()->velocity, right_wall, { 0.0f, 0.0f }, tFirst, deltaTime)) {
		if (!CollisionIntersection_RectRect(calculated_bounding_box, { 0.0f, 0.0f }, right_wall, { 0.0f, 0.0f }, tFirst, deltaTime)) {
			offset.x += state_machine->player.lock()->velocity.x * deltaTime;

			state_machine->player.lock()->SetPosition(offset);
		}
		else {
			offset.x += state_machine->player.lock()->velocity.x * tFirst;

			state_machine->player.lock()->SetPosition(offset);
		}
	}
	else {
		offset.x += state_machine->player.lock()->velocity.x * deltaTime;

		state_machine->player.lock()->SetPosition(offset);
	}

	time_in_state += deltaTime;

	if (time_in_state >= time_max) {
		time_in_state = time_max;

		state_machine->ChangeState(&state_machine->fallingState);
	}

	f32 jumpForce = state_machine->player.lock()->GetJumpForce();

	if (jumpForce <= 50.0f) {
		state_machine->player.lock()->SetJumpForce(0.0f);
		state_machine->ChangeState(&state_machine->fallingState);
	}

	offset.y += state_machine->player.lock()->velocity.y;

	state_machine->player.lock()->SetPosition(offset);

	for (auto& child : GetWorldBranch()->GetChildren()) {
		std::shared_ptr<FinishLine> finishLine = std::dynamic_pointer_cast<FinishLine>(child);

		if (finishLine) {
			std::shared_ptr<InputManager> input = std::dynamic_pointer_cast<InputManager>(state_machine->player.lock()->GetChildren().at(5));

			std::shared_ptr<RectPlayer> ground_check = std::dynamic_pointer_cast<RectPlayer>(state_machine->player.lock()->GetChildren().at(0));

			if (input && ground_check) {
				f32 collision_tFirst{};

				if (input->active) {
					std::shared_ptr<ImageRenderer2D> progress_bar = std::dynamic_pointer_cast<ImageRenderer2D>(state_machine->player.lock()->GetChildren().at(9));

					std::shared_ptr<VerticalProgressBar> vprog = std::dynamic_pointer_cast<VerticalProgressBar>(state_machine->player.lock()->GetChildren().at(8));

					if (progress_bar && vprog) {
						progress_bar->SetTransparency(1.f);
						vprog->SetTransparency(1.f);
						f32 y_pos = (ground_check->GetPosition().y / 10200.f) * (0.8f * screen_height);
						progress_bar->SetPosition({ progress_bar->GetLocalPosition().x, -0.4f * screen_height + y_pos });
						vprog->SetScale({ 20.f, y_pos });
					}
				}
				else {
					std::shared_ptr<ImageRenderer2D> progress_bar = std::dynamic_pointer_cast<ImageRenderer2D>(state_machine->player.lock()->GetChildren().at(9));

					std::shared_ptr<VerticalProgressBar> vprog = std::dynamic_pointer_cast<VerticalProgressBar>(state_machine->player.lock()->GetChildren().at(8));

					if (progress_bar && vprog) {
						progress_bar->SetTransparency(0.f);

						vprog->SetTransparency(0.f);
					}
				}

				if (CollisionIntersection_RectRect(ground_check->bounding_box, state_machine->player.lock()->velocity, finishLine->bounding_box, finishLine->velocity, collision_tFirst, deltaTime)) {
					// This player has finished!

					input->active = false;

					state_machine->active = false;

					state_machine->ChangeState(&state_machine->victoryState);

					GetWorldBranch()->RemoveChild(finishLine);
				}
			}
		}
	}
}

void PlayerFlyingState::PostUpdate() {
	AEMtx33 scale_matrix{ 0.0f };
	AEMtx33Identity(&scale_matrix);
	AEMtx33Scale(&scale_matrix, 150.f, 150.f);

	AEMtx33 rotation_matrix{ 0.0f };
	AEMtx33Identity(&rotation_matrix);
	AEMtx33Rot(&rotation_matrix, 0.f * (PI / 180.0f));

	AEMtx33 translate{ 0.0f };
	AEMtx33Identity(&translate);

	AEMtx33Trans(&translate, state_machine->player.lock()->GetLocalPosition().x - camera_x - 75.f, state_machine->player.lock()->GetLocalPosition().y - camera_y - 75.f);

	AEMtx33 transform = { 0 };
	AEMtx33Concat(&transform, &rotation_matrix, &scale_matrix);
	AEMtx33Concat(&transform, &translate, &transform);

	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);

	AEGfxSetColorToMultiply(1.f, 1.f, 1.f, 1.0f);

	AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);

	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(1.f);

	AEGfxTextureSet(default_tex, 0.f, 0.f);

	AEGfxSetTransform(transform.m);

	AEGfxMeshDraw(default_mesh, AE_GFX_MDM_TRIANGLES);

	std::cout << "Flying!" << std::endl;
}

void PlayerFlyingState::Exit() {
	if (state_machine->player.lock() == GetPlayer()) {
		AEAudioStopGroup(GetAudioDriver()->sfx_group);
	}

	state_machine->player.lock()->SetJumpForce(0.f);

	time_in_state = 0.f;

	PlayerAirborneState::Exit();
}

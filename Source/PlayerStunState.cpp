/* Start Header ************************************************************************/
/*!
\file		PlayerStunState.cpp
\author		Tai Xiao Xuan
\par		xiaoxuan.tai@digipen.edu
\date		09/03/2025
\brief		This file implements a FallingState class, that handles player Falling logic.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#include "PlayerStunState.hpp"
#include "PlayerMovementStateMachine.hpp"
#include "Utils.hpp"
#include "GameDefn.hpp"

void PlayerStunState::Enter() {
	state_name = "StunState";

	state_machine->player.lock()->SetSpeed(state_machine->player.lock()->GetMaxSpeed());

	state_machine->player.lock()->SetJumpForce(0.0f);

	state_duration = state_duration_max;

	if (state_machine->player.lock() == GetPlayer()) {
		AEAudioPlay(GetAudioDriver()->hit, GetAudioDriver()->sfx_group, 2.f, 1.f, 0);
	}

	PlayerAirborneState::Enter();
}

void PlayerStunState::HandleInput() {

	f32 jumpForce = state_machine->player.lock()->GetJumpForce();

	jumpForce -= 5000.0f * deltaTime;

	state_machine->player.lock()->SetJumpForce(jumpForce);

	state_machine->player.lock()->velocity.y += AEClamp(jumpForce, -2000.0f, 0.0f);

	//PlayerAirborneState::HandleInput();
}

void PlayerStunState::Update() {

	PlayerAirborneState::Update();

	std::shared_ptr<RectPlayer> ground_check = std::dynamic_pointer_cast<RectPlayer>(state_machine->player.lock()->GetChildren().at(0));

	AEVec2 ground_check_position = ground_check->GetPosition();

	std::shared_ptr<Object2D> world = std::dynamic_pointer_cast<Object2D>(GetRoot()->GetChildren().at(0));

	bool is_grounded = false;

	// AABB Collision
	if (world && ground_check) {
		for (const auto& child : world->GetChildren()) {
			std::shared_ptr<RectPlayer> platform = std::dynamic_pointer_cast<RectPlayer>(child);

			if (platform && platform->GetName() == "GROUND") {
				f32 tFirst{ 0.0f };
				if (CollisionIntersection_RectRect(ground_check->bounding_box, state_machine->player.lock()->velocity, platform->bounding_box, platform->velocity, tFirst, deltaTime)) {
					is_grounded = true;

					AEVec2 offset = state_machine->player.lock()->GetLocalPosition();

					offset.y += state_machine->player.lock()->velocity.y * tFirst;

					state_machine->player.lock()->SetPosition(offset);

					break;
				}

			}
		}
	}

	state_duration -= deltaTime;

	if (state_duration >= 0.5f) {
		std::shared_ptr<SpriteRenderer> sprite = std::dynamic_pointer_cast<SpriteRenderer>(state_machine->player.lock()->GetChildren().at(4));

		if (sprite) {
			sprite->SetState(&sprite->stunState);
		}
		return;
	}

	if (is_grounded) {
		state_machine->ChangeState(&state_machine->idleState);
	}
	else {
		AEVec2 offset = state_machine->player.lock()->GetLocalPosition();

		offset.y += state_machine->player.lock()->velocity.y * deltaTime;

		state_machine->player.lock()->SetPosition(offset);
	}
	
	if (state_duration < 0.0f) {
		state_machine->ChangeState(&state_machine->fallingState);
	}
}

void PlayerStunState::PostUpdate() {

	if (stun_type && state_duration >= 0.6f) {
		AEMtx33 scale_matrix{ 0.0f };
		AEMtx33Identity(&scale_matrix);
		AEMtx33Scale(&scale_matrix, ((state_duration_max - state_duration) / .15f) * 100.f, ((state_duration_max - state_duration) / .15f) * 100.f);

		AEMtx33 rotation_matrix{ 0.0f };
		AEMtx33Identity(&rotation_matrix);
		AEMtx33Rot(&rotation_matrix, 0.f * (PI / 180.0f));

		AEMtx33 translate{ 0.0f };
		AEMtx33Identity(&translate);

		AEMtx33Trans(&translate, pos_x - camera_x - ((state_duration_max - state_duration) / .15f) * 50.f, pos_y - camera_y - ((state_duration_max - state_duration) / .15f) * 50.f);

		AEMtx33 transform = { 0 };
		AEMtx33Concat(&transform, &rotation_matrix, &scale_matrix);
		AEMtx33Concat(&transform, &translate, &transform);

		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);

		AEGfxSetColorToMultiply(1.f, 1.f, 1.f, 1.0f);

		AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);

		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		AEGfxSetTransparency(1.f);

		AEGfxTextureSet(stun_tex, 0.f, 0.f);

		AEGfxSetTransform(transform.m);

		AEGfxMeshDraw(default_mesh, AE_GFX_MDM_TRIANGLES);
	}

	PlayerAirborneState::PostUpdate();
}

void PlayerStunState::Exit() {
	if (state_machine->player.lock() == GetPlayer()) {
		AEAudioStopGroup(GetAudioDriver()->sfx_group);
	}

	PlayerAirborneState::Exit();
}
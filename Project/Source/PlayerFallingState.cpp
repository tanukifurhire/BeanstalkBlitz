/* Start Header ************************************************************************/
/*!
\file		PlayerFallingState.cpp
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

#include "PlayerFallingState.hpp"
#include "PlayerMovementStateMachine.hpp"
#include "Utils.hpp"

void PlayerFallingState::Enter() {
	state_name = "FallingState";

	state_machine->player.lock()->SetSpeed(state_machine->player.lock()->GetMaxSpeed());
	state_machine->player.lock()->SetJumpForce(0.0f);
	if (state_machine->player.lock()->GetJumpForce() > 0.0f) {
		
	}

	std::shared_ptr<InputManager> input = std::dynamic_pointer_cast<InputManager>(state_machine->player.lock()->GetChildren().at(5));
	if (input)
	{
		on_jump_triggered = [this](void* context) { (void)context;
			this->OnJumpTriggered();
		};

		input->on_jump_triggered += on_jump_triggered;
	}

	std::shared_ptr<SpriteRenderer> sprite = std::dynamic_pointer_cast<SpriteRenderer>(state_machine->player.lock()->GetChildren().at(4));

	if (sprite) {
		sprite->SetState(&sprite->fallState);
	}

	PlayerAirborneState::Enter();
}

void PlayerFallingState::OnJumpTriggered() {
	std::shared_ptr<RectPlayer> coyote = std::dynamic_pointer_cast<RectPlayer>(state_machine->player.lock()->GetChildren().at(1));

	std::shared_ptr<RectPlayer> runOffL = std::dynamic_pointer_cast<RectPlayer>(state_machine->player.lock()->GetChildren().at(2));

	std::shared_ptr<RectPlayer> runOffR = std::dynamic_pointer_cast<RectPlayer>(state_machine->player.lock()->GetChildren().at(3));

	AEVec2 coyote_position = coyote->GetPosition();

	AEVec2 runOffL_position = runOffL->GetPosition();

	AEVec2 runOffR_position = runOffR->GetPosition();

	std::shared_ptr<Object2D> world = std::dynamic_pointer_cast<Object2D>(GetRoot()->GetChildren().at(0));

	bool is_coyote = false;

	bool is_runoff_L = false;

	bool is_runoff_R = false;

	if (world && coyote) {
		for (const auto& child : world->GetChildren()) {
			std::shared_ptr<RectPlayer> platform = std::dynamic_pointer_cast<RectPlayer>(child);

			if (platform) {
				AEVec2 platform_position = platform->GetPosition();

				if (CalculateRectRectCollision(coyote_position, coyote->GetScale().x, coyote->GetScale().y, platform_position, platform->GetScale().x, platform->GetScale().y)) {
					is_coyote = true;

					break;
				}

				if (CalculateRectRectCollision(runOffL_position, runOffL->GetScale().x, runOffL->GetScale().y, platform_position, platform->GetScale().x, platform->GetScale().y)) {
					is_runoff_L = true;

					break;
				}

				if (CalculateRectRectCollision(runOffR_position, runOffR->GetScale().x, runOffR->GetScale().y, platform_position, platform->GetScale().x, platform->GetScale().y)) {
					is_runoff_R = true;

					break;
				}
			}
		}
	}

	if (is_coyote) {
		state_machine->ChangeState(&state_machine->jumpState);

		state_machine->player.lock()->SetJumpForce(state_machine->player.lock()->GetMaxJumpForce());

		return;
	}

	if (is_runoff_L && (state_machine->previous_state != &state_machine->jumpState)) {
		state_machine->ChangeState(&state_machine->jumpState);

		state_machine->player.lock()->SetJumpForce(state_machine->player.lock()->GetMaxJumpForce());

		return;
	}

	if (is_runoff_R && (state_machine->previous_state != &state_machine->jumpState)) {
		state_machine->ChangeState(&state_machine->jumpState);

		state_machine->player.lock()->SetJumpForce(state_machine->player.lock()->GetMaxJumpForce());

		return;
	}
}

void PlayerFallingState::HandleInput() {

	f32 jumpForce = state_machine->player.lock()->GetJumpForce();

	jumpForce -= 5000.0f * deltaTime;

	state_machine->player.lock()->SetJumpForce(jumpForce);

	state_machine->player.lock()->velocity.y += AEClamp(jumpForce, -2000.0f, 0.0f);

	PlayerAirborneState::HandleInput();
}

void PlayerFallingState::Update() {

	PlayerAirborneState::Update();

	std::shared_ptr<RectPlayer> ground_check = std::dynamic_pointer_cast<RectPlayer>(state_machine->player.lock()->GetChildren().at(0));

	AEVec2 ground_check_position = ground_check->GetPosition();

	std::shared_ptr<Object2D> world = std::dynamic_pointer_cast<Object2D>(GetRoot()->GetChildren().at(0));

	bool is_grounded = false;

	// AABB Collision
	if (world && ground_check) {
		for (const auto& child : world->GetChildren()) {
			std::shared_ptr<RectPlayer> platform = std::dynamic_pointer_cast<RectPlayer>(child);

			if (platform) {

				f32 tFirst { 0.0f };
				if (CollisionIntersection_RectRect(ground_check->bounding_box , state_machine->player.lock()->velocity, platform->bounding_box, platform->velocity, tFirst, deltaTime)) {
					is_grounded = true;

					AEVec2 offset = state_machine->player.lock()->GetLocalPosition();

					offset.y += state_machine->player.lock()->velocity.y * tFirst;

					state_machine->player.lock()->SetPosition(offset);

					break;
				}

			}
		}
	}

	if (is_grounded) {
		state_machine->ChangeState(&state_machine->idleState);

		if (state_machine->player.lock() == GetPlayer()) {
			AEAudioPlay(GetAudioDriver()->step_leaf, GetAudioDriver()->sfx_group, 2.f, 1.f, 0);
		}
	}
	else {
		AEVec2 offset = state_machine->player.lock()->GetLocalPosition();

		offset.y += state_machine->player.lock()->velocity.y * deltaTime;

		state_machine->player.lock()->SetPosition(offset);
	}
}

void PlayerFallingState::PostUpdate() {

	PlayerAirborneState::PostUpdate();
}

void PlayerFallingState::Exit() {

	std::shared_ptr<InputManager> input = std::dynamic_pointer_cast<InputManager>(state_machine->player.lock()->GetChildren().at(5));
	if (input)
	{

		input->on_jump_triggered -= on_jump_triggered;
	}

	PlayerAirborneState::Exit();
}
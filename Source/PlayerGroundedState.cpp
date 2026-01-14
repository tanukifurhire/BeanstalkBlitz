/* Start Header ************************************************************************/
/*!
\file		PlayerGroundedState.cpp
\author		Tai Xiao Xuan
\par		xiaoxuan.tai@digipen.edu
\date		09/03/2025
\brief		This file implements a Grounded class, that handles player Grounded logic.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#include "PlayerGroundedState.hpp"
#include "RectPlayer.hpp"
#include "Player.hpp"
#include "PlayerMovementStateMachine.hpp"
#include "Utils.hpp"

void PlayerGroundedState::Enter() {

	if (state_machine->player.lock()->coyote) {
		state_machine->player.lock()->SetJumpForce(state_machine->player.lock()->GetMaxJumpForce());

		state_machine->ChangeState(&state_machine->jumpState);

		state_machine->player.lock()->coyote = false;
	}

	std::shared_ptr<InputManager> input = std::dynamic_pointer_cast<InputManager>(state_machine->player.lock()->GetChildren().at(5));
	if (input)
	{
		on_jump_pressed = [this](void* context) { (void)context;
			this->OnJump();
		};

		input->on_jump_pressed += on_jump_pressed;
	}

	PlayerMovementState::Enter();
}

void PlayerGroundedState::OnJump() {
	if (state_machine->current_state != &state_machine->jumpChargeState) {
		state_machine->ChangeState(&state_machine->jumpChargeState);
	}
}

void PlayerGroundedState::HandleInput() {

	PlayerMovementState::HandleInput();
}

void PlayerGroundedState::Update() {

	std::shared_ptr<RectPlayer> ground_check = std::dynamic_pointer_cast<RectPlayer>(state_machine->player.lock()->GetChildren().at(0));

	AEVec2 ground_check_position = ground_check->GetPosition();

	std::shared_ptr<Object2D> world = std::dynamic_pointer_cast<Object2D>(GetRoot()->GetChildren().at(0));

	bool is_grounded = false;

	// I WILL CHANGE TO AABB
	if (world && ground_check) {
		for (const auto& child : world->GetChildren()) {
			std::shared_ptr<RectPlayer> platform = std::dynamic_pointer_cast<RectPlayer>(child);

			if (platform) {


				f32 tFirst{ 0.f };
				if (CollisionIntersection_RectRect(ground_check->bounding_box, state_machine->player.lock()->velocity, platform->bounding_box, platform->velocity, tFirst, deltaTime)) {
					is_grounded = true;
					
					state_machine->player.lock()->SetPosition({ state_machine->player.lock()->GetLocalPosition().x, platform->GetLocalPosition().y + platform->GetScale().y / 2 - ground_check->GetLocalPosition().y + ground_check->GetScale().y / 2 });

					break;
				}

			}
		}
	}

	if (is_grounded == false) {
		std::cout << "Fall!" << std::endl;

		state_machine->ChangeState(&state_machine->fallingState);

		return;
	}

	PlayerMovementState::Update();
}

void PlayerGroundedState::PostUpdate() {

	PlayerMovementState::PostUpdate();
}

void PlayerGroundedState::Exit() {

	std::shared_ptr<InputManager> input = std::dynamic_pointer_cast<InputManager>(state_machine->player.lock()->GetChildren().at(5));
	if (input)
	{
		input->on_jump_pressed -= on_jump_pressed;
	}

	PlayerMovementState::Exit();
}


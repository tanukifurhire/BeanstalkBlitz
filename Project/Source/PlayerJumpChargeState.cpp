/* Start Header ************************************************************************/
/*!
\file		PlayerJumpChargeState.cpp
\author		Tai Xiao Xuan
\par		xiaoxuan.tai@digipen.edu
\date		09/03/2025
\brief		This file implements a JumpChargeState class, that handles player Jump Charging Logic, much like Jump King.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#include "PlayerJumpChargeState.hpp"
#include "PlayerMovementStateMachine.hpp"

void PlayerJumpChargeState::Enter() {
	state_name = "JumpChargeState";

	state_machine->player.lock()->SetJumpForce(state_machine->player.lock()->GetMaxJumpForce());

	std::shared_ptr<InputManager> input = std::dynamic_pointer_cast<InputManager>(state_machine->player.lock()->GetChildren().at(5));
	if (input)
	{
		on_jump_released = [this](void* context) { (void)context;
			std::cout << "Jump Release Event Received!" << std::endl;
			this->OnJumpReleased();
		};

		input->on_jump_released += on_jump_released;
	}

	std::shared_ptr<PieTimer> timer = std::dynamic_pointer_cast<PieTimer>(state_machine->player.lock()->GetChildren().at(7));

	timer->TimerProgress() = 0.0f;

	std::shared_ptr<SpriteRenderer> sprite = std::dynamic_pointer_cast<SpriteRenderer>(state_machine->player.lock()->GetChildren().at(4));

	if (sprite) {
		sprite->SetState(&sprite->chargeState);
	}

	if (state_machine->player.lock() == GetPlayer()) {
		AEAudioPlay(GetAudioDriver()->charging, GetAudioDriver()->sfx_group, 2.f, 1.f, 0);
	}

	//PlayerGroundedState::Enter();
}

void PlayerJumpChargeState::OnJumpReleased() {
	state_machine->ChangeState(&state_machine->jumpState);
}

void PlayerJumpChargeState::HandleInput() {
	f32 jump_force = state_machine->player.lock()->GetJumpForce();

	jump_force += state_machine->player.lock()->GetJumpChargeSpeed() * deltaTime;

	state_machine->player.lock()->SetJumpForce(jump_force);

	time_in_state += deltaTime;

	std::shared_ptr<PieTimer> timer = std::dynamic_pointer_cast<PieTimer>(state_machine->player.lock()->GetChildren().at(7));

	timer->TimerProgress() = time_in_state / force_change_state;

	if (time_in_state >= force_change_state) {
		dynamic_cast<PlayerStunState*>(&state_machine->stunState)->stun_type = SELF_STUN;
		state_machine->ChangeState(&state_machine->stunState);
		return;
	}

	if (time_in_state < threshold_to_move) {
		PlayerGroundedState::HandleInput();
	}
}

void PlayerJumpChargeState::Update() {

	PlayerGroundedState::Update();
}

void PlayerJumpChargeState::PostUpdate() {

	PlayerGroundedState::PostUpdate();
}

void PlayerJumpChargeState::Exit() {
	time_in_state = 0.0f;

	std::shared_ptr<PieTimer> timer = std::dynamic_pointer_cast<PieTimer>(state_machine->player.lock()->GetChildren().at(7));

	timer->TimerProgress() = 0.0f;

	std::shared_ptr<InputManager> input = std::dynamic_pointer_cast<InputManager>(state_machine->player.lock()->GetChildren().at(5));
	if (input)
	{
		input->on_jump_released -= on_jump_released;
	}

	if (state_machine->player.lock() == GetPlayer()) {
		AEAudioStopGroup(GetAudioDriver()->sfx_group);
	}

	//PlayerGroundedState::Exit();
}
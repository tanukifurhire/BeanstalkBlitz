/* Start Header ************************************************************************/
/*!
\file		PlayerMovingState.cpp
\author		Tai Xiao Xuan
\par		xiaoxuan.tai@digipen.edu
\date		09/03/2025
\brief		This file implements a MovingState class, that handles player horizontal movement logic.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#include "PlayerMovingState.hpp"
#include "PlayerMovementStateMachine.hpp"

void PlayerMovingState::Enter() {
	state_name = "MovingState";

	state_machine->player.lock()->SetSpeed(state_machine->player.lock()->GetMaxSpeed());

	std::shared_ptr<SpriteRenderer> sprite = std::dynamic_pointer_cast<SpriteRenderer>(state_machine->player.lock()->GetChildren().at(4));

	if (sprite) {
		sprite->SetState(&sprite->moveState);
	}

	if (state_machine->player.lock() == GetPlayer()) {
		AEAudioPlay(GetAudioDriver()->run, GetAudioDriver()->sfx_group, 2.f, 1.f, 1);
	}

	PlayerGroundedState::Enter();
}

void PlayerMovingState::HandleInput() {

	PlayerGroundedState::HandleInput();

	if (state_machine->player.lock()->player_input.x == 0.0f) {
		state_machine->ChangeState(&state_machine->idleState);
	}
}

void PlayerMovingState::Update() {

	PlayerGroundedState::Update();
}

void PlayerMovingState::PostUpdate() {

	PlayerGroundedState::PostUpdate();
}

void PlayerMovingState::Exit() {
	if (state_machine->player.lock() == GetPlayer()) {
		AEAudioStopGroup(GetAudioDriver()->sfx_group);
	}

	PlayerGroundedState::Exit();
}

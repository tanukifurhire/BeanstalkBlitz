/* Start Header ************************************************************************/
/*!
\file		PlayerIdleState.cpp
\author		Tai Xiao Xuan
\par		xiaoxuan.tai@digipen.edu
\date		09/03/2025
\brief		This file implements a IdleState class, that handles player Idle logic.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#include "PlayerIdleState.hpp"
#include "PlayerMovementStateMachine.hpp"

void PlayerIdleState::Enter() {
	state_name = "IdleState";

	state_machine->player.lock()->SetSpeed(0.0f);

	std::shared_ptr<SpriteRenderer> sprite = std::dynamic_pointer_cast<SpriteRenderer>(state_machine->player.lock()->GetChildren().at(4));

	if (sprite) {
		sprite->SetState(&sprite->idleState);
	}

	PlayerGroundedState::Enter();
}

void PlayerIdleState::HandleInput() {

	PlayerGroundedState::HandleInput();

	if (state_machine->player.lock()->player_input.x > 0.0f || state_machine->player.lock()->player_input.x < 0.0f) {
		std::cout << "Change State To Move!" << std::endl;
		state_machine->ChangeState(&state_machine->movingState);
	}
}

void PlayerIdleState::Update() {

	PlayerGroundedState::Update();
}

void PlayerIdleState::PostUpdate() {

	PlayerGroundedState::PostUpdate();
}

void PlayerIdleState::Exit() {

	PlayerGroundedState::Exit();
}

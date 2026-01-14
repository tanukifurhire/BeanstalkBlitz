/* Start Header ************************************************************************/
/*!
\file		PlayerJumpChargeState.hpp
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

#pragma once
#include "PlayerGroundedState.hpp"

class PlayerJumpChargeState : public PlayerGroundedState {
public:
	PlayerJumpChargeState(PlayerMovementStateMachine* state_machine_val) : PlayerGroundedState(state_machine_val) {}

	void Enter() override;

	void HandleInput() override;

	void Update() override;

	void PostUpdate() override;

	void OnJumpReleased();

	void Exit() override;

	Callback on_jump_released;

protected:
	float time_in_state = 0.0f;
	float threshold_to_move = 0.2f;

	float force_change_state = 1.0f;
};
/* Start Header ************************************************************************/
/*!
\file		PlayerGroundedState.hpp
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

#pragma once
#include "PlayerMovementState.hpp"

class PlayerGroundedState : public PlayerMovementState {
public:
	PlayerGroundedState(PlayerMovementStateMachine* state_machine_val) : PlayerMovementState(state_machine_val) {}

	void Enter() override;

	void HandleInput() override;

	void Update() override;

	void PostUpdate() override;

	void OnJump();

	void Exit() override;

	Callback on_jump_pressed;

protected:

};
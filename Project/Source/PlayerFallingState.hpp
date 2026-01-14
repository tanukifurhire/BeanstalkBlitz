/* Start Header ************************************************************************/
/*!
\file		PlayerFallingState.hpp
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

#pragma once
#include "PlayerAirborneState.hpp"

class PlayerFallingState : public PlayerAirborneState {
public:
	PlayerFallingState(PlayerMovementStateMachine* state_machine_val) : PlayerAirborneState(state_machine_val) {}

	void Enter() override;

	void HandleInput() override;

	void Update() override;

	void PostUpdate() override;

	void OnJumpTriggered();

	void Exit() override;

	Callback on_jump_triggered;

protected:

};
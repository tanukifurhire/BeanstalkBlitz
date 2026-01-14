/* Start Header ************************************************************************/
/*!
\file		PlayerIdleState.hpp
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

#pragma once
#include "PlayerGroundedState.hpp"

class PlayerIdleState : public PlayerGroundedState {
public:
	PlayerIdleState(PlayerMovementStateMachine* state_machine_val) : PlayerGroundedState(state_machine_val) {}

	void Enter() override;

	void HandleInput() override;

	void Update() override;

	void PostUpdate() override;

	void Exit() override;

protected:

};
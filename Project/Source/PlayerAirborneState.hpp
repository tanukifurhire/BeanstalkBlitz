/* Start Header ************************************************************************/
/*!
\file		PlayerAirborneState.hpp
\author		Tai Xiao Xuan
\par		xiaoxuan.tai@digipen.edu
\date		09/03/2025
\brief		This file implements a Airborne class, part of the hierarchical state machine implementation.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#pragma once
#include "PlayerMovementState.hpp"

class PlayerAirborneState : public PlayerMovementState {
public: 
	PlayerAirborneState(PlayerMovementStateMachine* state_machine_val) : PlayerMovementState(state_machine_val) {}

	void Enter() override;

	void HandleInput() override;

	void Update() override;

	void PostUpdate() override;

	void Exit() override;

protected:

};
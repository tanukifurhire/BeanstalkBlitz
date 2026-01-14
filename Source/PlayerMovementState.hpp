
/* Start Header ************************************************************************/
/*!
\file		PlayerMovementState.hpp
\author		Tai Xiao Xuan
\par		xiaoxuan.tai@digipen.edu
\date		09/03/2025
\brief		This file implements a MovementState class, that handles the base horizontal movement of the Player.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#pragma once
#include "State.hpp"
#include <functional>

using Callback = std::function<void(void*)>;

class PlayerMovementState : public State {
public:
	PlayerMovementState(PlayerMovementStateMachine* state_machine_val) : State(state_machine_val) {}

	void Enter() override;

	void HandleInput() override;

	void Update() override;

	void PostUpdate() override;

	void OnLeft();

	void OnRight();

	void Exit() override;

	Callback on_left;

	Callback on_right;

protected:

};
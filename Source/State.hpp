/* Start Header ************************************************************************/
/*!
\file		State.hpp
\author		Tai Xiao Xuan
\par		xiaoxuan.tai@digipen.edu
\date		09/03/2025
\brief		This file implements a basic interface for other States to inherit from.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#pragma once
#include <memory>
#include <string>

class PlayerMovementStateMachine;

class State {
public:

	State(PlayerMovementStateMachine* state_machine);

	virtual void Enter();

	virtual void HandleInput();

	virtual void Update();

	virtual void PostUpdate();

	virtual void Exit();

protected:
	PlayerMovementStateMachine* state_machine;
	std::string state_name = "State";
};
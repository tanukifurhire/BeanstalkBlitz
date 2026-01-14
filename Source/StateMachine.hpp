/* Start Header ************************************************************************/
/*!
\file		StateMachine.hpp
\author		Tai Xiao Xuan
\par		xiaoxuan.tai@digipen.edu
\date		09/03/2025
\brief		This file implements a base StateMachine logic for changing and handling different States.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#pragma once
#include "GameDefn.hpp"
#include <memory>

class State;

class StateMachine {
public:

	StateMachine();

	State* current_state = nullptr;

	State* previous_state = nullptr;

	virtual void Enter();

	virtual void HandleInput();

	virtual void Update();

	virtual void PostUpdate();

	virtual void Exit();

	virtual void ChangeState(State* new_state);

	bool active = false;

protected:

};
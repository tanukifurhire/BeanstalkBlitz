/* Start Header ************************************************************************/
/*!
\file		StateMachine.cpp
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

#include "StateMachine.hpp"
#include "State.hpp"

StateMachine::StateMachine() {};

void StateMachine::Enter() {
	current_state->Enter();
}

void StateMachine::HandleInput() {
	current_state->HandleInput();
}

void StateMachine::Update() {
	current_state->Update();
}

void StateMachine::PostUpdate() {
	current_state->PostUpdate();
}

void StateMachine::Exit() {
	current_state->Exit();
}

void StateMachine::ChangeState(State* new_state) {
	
	if (current_state) {
		current_state->Exit();

		previous_state = current_state;
	}

	current_state = new_state;

	current_state->Enter();
}
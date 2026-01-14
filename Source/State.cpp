/* Start Header ************************************************************************/
/*!
\file		State.cpp
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

#include "State.hpp"
#include "PlayerMovementStateMachine.hpp"
#include <iostream>

State::State(PlayerMovementStateMachine* state_machine_val) : state_machine(state_machine_val) {}

void State::Enter() {
	std::cout << state_name << " Entered!" << std::endl;
}

void State::HandleInput() {

}

void State::Update() {

}

void State::PostUpdate() {

}

void State::Exit() {

}

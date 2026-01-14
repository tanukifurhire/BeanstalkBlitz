/* Start Header ************************************************************************/
/*!
\file		PlayerAirborneState.cpp
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

#include "PlayerAirborneState.hpp"
#include "PlayerMovementStateMachine.hpp"

void PlayerAirborneState::Enter() {

	PlayerMovementState::Enter();
}

void PlayerAirborneState::HandleInput() {

	PlayerMovementState::HandleInput();
}

void PlayerAirborneState::Update() {

	PlayerMovementState::Update();
}

void PlayerAirborneState::PostUpdate() {

	PlayerMovementState::PostUpdate();
}

void PlayerAirborneState::Exit() {

	PlayerMovementState::Exit();
}

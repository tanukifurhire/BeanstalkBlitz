/* Start Header ************************************************************************/
/*!
\file		PlayerVictoryState.hpp
\author		Tai Xiao Xuan
\par		xiaoxuan.tai@digipen.edu
\date		30/03/2025
\brief		This file declares a VictoryState class, that handles player Victory logic.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#pragma once
#include "PlayerAirborneState.hpp"

class PlayerVictoryState : public PlayerAirborneState {
public:
	PlayerVictoryState(PlayerMovementStateMachine* state_machine_val) : PlayerAirborneState(state_machine_val) {}

	void Enter() override;

	void HandleInput() override;

	void Update() override;

	void PostUpdate() override;

	void Exit() override;

protected:

};
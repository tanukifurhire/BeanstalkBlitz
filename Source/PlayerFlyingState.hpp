/* Start Header ************************************************************************/
/*!
\file		PlayerFlyingState.hpp
\author		Tai Xiao Xuan
\par		xiaoxuan.tai@digipen.edu
\date		06/04/2025
\brief		This file implements a FlyingState class, that handles player Flying logic. From Powerups

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#pragma once
#include "PlayerAirborneState.hpp"

class PlayerFlyingState : public PlayerAirborneState {
public:
	PlayerFlyingState(PlayerMovementStateMachine* state_machine_val) : PlayerAirborneState(state_machine_val) {}

	void Enter() override;

	void HandleInput() override;

	void Update() override;

	void PostUpdate() override;

	void Exit() override;

protected: 
	float time_in_state{ 0.f };
	float time_max{ 2.f };
};
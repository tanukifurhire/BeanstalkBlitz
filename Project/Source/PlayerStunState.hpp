/* Start Header ************************************************************************/
/*!
\file		PlayerStunState.hpp
\author		Tai Xiao Xuan
\par		xiaoxuan.tai@digipen.edu
\date		09/03/2025
\brief		This file implements a StunState class, that handles player Stunned logic.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#pragma once
#include "PlayerAirborneState.hpp"

enum StunType {
	SELF_STUN,
	ENEMY_STUN
};

class PlayerStunState : public PlayerAirborneState {
public:

	PlayerStunState(PlayerMovementStateMachine* state_machine_val) : PlayerAirborneState(state_machine_val) {}

	void Enter() override;

	void HandleInput() override;

	void Update() override;

	void PostUpdate() override;

	void Exit() override;

	float pos_x{};
	float pos_y{};

	StunType stun_type{ SELF_STUN };

protected:
	float state_duration{};
	const float state_duration_max{0.8f};
};
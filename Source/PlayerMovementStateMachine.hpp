/* Start Header ************************************************************************/
/*!
\file		PlayerMovementStateMachine.hpp
\author		Tai Xiao Xuan
\par		xiaoxuan.tai@digipen.edu
\date		09/03/2025
\brief		This file implements a PlayerMovementStateMachine class, inheriting from StateMachine and
			storing all the concrete states of the Player. 

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#pragma once
#include "StateMachine.hpp"
#include "PlayerIdleState.hpp"
#include "PlayerMovingState.hpp"
#include "PlayerFallingState.hpp"
#include "PlayerJumpState.hpp"
#include "PlayerJumpChargeState.hpp"
#include "PlayerStunState.hpp"
#include "PlayerVictoryState.hpp"
#include "PlayerFlyingState.hpp"

class PlayerMovementStateMachine : public StateMachine {
public:

	PlayerMovementStateMachine() {}

	PlayerIdleState idleState = PlayerIdleState(this);

	PlayerMovingState movingState = PlayerMovingState(this);

	PlayerJumpState jumpState = PlayerJumpState(this);

	PlayerFallingState fallingState = PlayerFallingState(this);

	PlayerJumpChargeState jumpChargeState = PlayerJumpChargeState(this);

	PlayerStunState stunState = PlayerStunState(this);

	PlayerVictoryState victoryState = PlayerVictoryState(this);

	PlayerFlyingState flyingState = PlayerFlyingState(this);

	std::weak_ptr<Player> player;

protected:

};
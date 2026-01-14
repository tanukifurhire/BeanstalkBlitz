/* Start Header ************************************************************************/
/*!
\file		Player.hpp
\author		Tai Xiao Xuan
\par		xiaoxuan.tai@digipen.edu
\date		09/03/2025
\brief		This file implements Player class Functions that handles Player logic, movement and collisions

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#pragma once
#include "Object2D.hpp"

class PlayerMovementStateMachine;

class Player : public Object2D {
public:
    Player(std::shared_ptr<Object> parent = nullptr);

    AEVec2 player_input{ 0.0f, 0.0f };

    const f32 GetSpeed() {
        return movementSpeed;
    }

    const f32 GetMaxSpeed() {
        return maxMovementSpeed;
    }

    void SetSpeed(f32 speed_val) {
        movementSpeed = speed_val;
    }

    const f32 GetJumpForce() {
        return jumpForce;
    }

    const f32 GetJumpChargeSpeed() {
        return jumpChargeSpeed;
    }

    const f32 GetMaxJumpForce() {
        return maxJumpForce;
    }

    void SetJumpForce(f32 jump_val) {
        jumpForce = jump_val;
    }

    PlayerMovementStateMachine* GetPlayerStateMachine() const {
        return movementStateMachine;
    }

    void Enter() override;

    void PreUpdate() override;

    void Update() override;

    void PostUpdate() override;

    void Exit() override;

    bool doubleJumped = false;
    bool coyote = false;

protected:

    f32 movementSpeed{ 0.0f };
    f32 maxMovementSpeed{ 500.0f };

    f32 jumpForce{ 0.0f };

    f32 maxJumpForce{ 1900.0f };
    f32 jumpChargeSpeed{ 2000.0f };

    PlayerMovementStateMachine* movementStateMachine = nullptr;
};

class ServerPlayer : public Player {
public: 
    ServerPlayer(std::shared_ptr<Object> parent = nullptr) : Player(std::move(parent)) {}

    void Update() override;
};
/* Start Header ************************************************************************/
/*!
\file		EnemyHeaderClass.hpp
\author		Tan Chun Kwang Xavier
\par		chunkwangxavier.tan@digipen.edu
\date		09/03/2025
\brief		This file implements

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#pragma once
#include "AEEngine.h"
#include "Object2D.hpp"

class EnemyHeaderClass : public Object2D
{
public:
    EnemyHeaderClass(std::shared_ptr<Object> parent = nullptr);

    virtual void Enter();

    virtual void PreUpdate();

    virtual void Update();

    virtual void PostUpdate();

    virtual void Exit();

    bool active{ false };

protected:
    // Definition of struct of Bounding Boxes (From GIT)
    struct AABB
    {
        AEVec2	min{};  // Bottom left corner
        AEVec2	max{};  // Top right corner
    };

    // Used By Enemy Type 3
    enum initSide {
        LEFT,
        RIGHT
    };

    // Some variables that each different enemy types will use
    std::string printString{};  // Debugging string (removable)
    std::string printString2{};  // Debugging string (removable)
    f32 movementSpeed{};        // Speed of enemy movement
    AABB enemyBounds = {};      // Declaration of bounding box for enemies

    // Debug function to check if collision works (Removable)
    //void PrintCollision(std::string);

    // Functions here include (They are declared in order of being used):
    // 1. Update the current Enemy's Bounding Box
    // 2. Retrieve the player(s) Bounding Boxes
    // 3. Check for collision with players (overridable; subject to change)
    // 4. Funnction to do all collision checking with enemies
    void UpdateEnemyBoundingBox();  // 1.
    AABB GetPlayerBounds(AEVec2 const&, AEVec2 const&); // 2.
    // Somewhere in the function... 3.
    void EnemyPlayerCollision();  // 4.
    void EnemyPlayerCollision(const AABB&, const AEVec2&);  // 4b. (Overload Ver.)
    // Overload version will use a specified AABB collsion and do collision checking

    void UpdateVelocity();
};
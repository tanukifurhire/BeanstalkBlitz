/* Start Header ************************************************************************/
/*!
\file		EnemyBasic1.hpp
\author		Tan Chun Kwang Xavier
\par		chunkwangxavier.tan@digipen.edu
\date		09/03/2025
\brief		This file implements the first enemy type of the game where
            the movement of this enemy is to move around in a circle pattern

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#pragma once
#include "AEEngine.h"
#include "GameDefn.hpp"

class EnemyBasic1 : public EnemyHeaderClass
{
public:
    EnemyBasic1(std::shared_ptr<Object> parent = nullptr);

    void Enter() override;

    void PreUpdate() override;

    void Update() override;

    void PostUpdate() override;

    void Exit() override;

protected:
    f32 radius = 100.0f; // Radius of circular movement
    f32 iterator{}; // Counter to allow continuous circular movement

    void SetNextPosition();
    void UpdateBeeVelocity();
};
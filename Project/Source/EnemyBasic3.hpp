/* Start Header ************************************************************************/
/*!
\file		EnemyBasic3.hpp
\author		Tan Chun Kwang Xavier
\par		chunkwangxavier.tan@digipen.edu
\date		09/03/2025
\brief		This file implements the third enemy type of the game where
            the enemy is of a shooting type that will shoot in a set direction

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#pragma once
#include "AEEngine.h"
#include "GameDefn.hpp"

class EnemyBasic3 : public EnemyHeaderClass
{
public:
    EnemyBasic3(std::shared_ptr<Object> parent = nullptr);

    void Enter() override;

    void PreUpdate() override;

    void Update() override;

    void PostUpdate() override;

    void Exit() override;

    int GetOffSide();

protected:
    // This defines the maximum positioning of where the enemy can spawn
    const std::pair<float, float> maxMapBounds{ -400.0f, 400.0f };

    AEGfxTexture* eggLeftText{};
    AEGfxTexture* eggRightText{};

    struct Bullet {
        // Mesh to draw the bullet
        AEGfxVertexList* bulletMesh = nullptr;

        AEVec2 startFixedPos;   // Fixed position of where bullet should respawn
        AEVec2 pos;             // Current pos of bullet
        AEVec2 bulletScale;     // Scaling of bullet
        AABB bulletAABB;        // Bounding collision box of bullet

        // Max bounds of how far the bullet can travel
        // first: when enemy spawns on the RIGHT
        // second: when enemy spawns on the LEFT
        std::pair<float, float> bulletLimitBounds;

        f32 bulletSpeed;        // Speed of the bullet movement
    };

    // Inherited Enum from EnemyHeaderClass
    initSide offSide{};

    Bullet* bullet = new Bullet{};
    u32 bulletColor = 0xFFFFB07C;

    void DrawSquare();
    void DrawCircles(int);              // Prepares the mesh for the bullet

    //------Initializers for bullet-----//
    void CheckOffSidePosition();
    void CheckStartingPos();
    void SetBulletLimits();
    void SetBulletScaling();
    //------Initializers for bullet-----//

    void MoveBullet();
    void CheckBulletBounds();
    void UpdateBulletBoundingBox();
    void DrawBullet();

    // Debug stats
    void PrintBulletStats();
};
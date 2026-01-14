/* Start Header ************************************************************************/
/*!
\file		Utils.hpp
\author		Xavier Seet Long Chuan
\par		x.seet@digipen.edu
\date		09/03/2025
\brief		This file declares the Utility functions and structures such as RectRectCollision and AABB Static/Dynamic Collisions, and Bounding Boxes for AABB struct


Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#pragma once
#include "AEengine.h"
#include <iostream>

struct AABB
{
	AEVec2	min;
	AEVec2	max;
};

// Static Collision Check
bool CollisionIntersection_RectRect_Static(const AABB& object1, const AABB& object2);

// Dynamic Collision Check
bool CollisionIntersection_RectRect_Dynamic(const AABB& object1, const AEVec2& object1_vel, const AABB& object2, const AEVec2& object2_vel, float& firstTimeOfCollision, float& gravity_dt);

// Collision check for static or dynamic
inline bool CollisionIntersection_RectRect(const AABB& object1, const AEVec2& vel_object1, const AABB& object2, const AEVec2& vel_object2, float& firstTimeOfCollision, float& gravity_dt)
{
    bool staticCollision = false;
    staticCollision = CollisionIntersection_RectRect_Static(object1, object2);
    if (staticCollision)
    {
        return true;
    }

    return CollisionIntersection_RectRect_Dynamic(object1, vel_object1, object2, vel_object2, firstTimeOfCollision, gravity_dt);
}

typedef struct AEVec2 AEVec2;

typedef float f32;

bool CalculateRectRectCollision(const AEVec2& position1, f32 scale1_x, f32 scale1_y,
    const AEVec2& position2, f32 scale2_x, f32 scale2_y);

f32 lerp(f32 start, f32 end, f32 alpha);


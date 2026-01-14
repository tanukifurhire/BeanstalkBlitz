/* Start Header ************************************************************************/
/*!
\file		Utils.cpp
\author		Xavier Seet Long Chuan
\par		x.seet@digipen.edu
\date		09/03/2025
\brief		This file implements the Utility functions such as RectRectCollision and AABB Static/Dynamic Collisions
            

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#include "Utils.hpp"
#include "AEEngine.h"

bool CalculateRectRectCollision(const AEVec2& position1, f32 scale1_x, f32 scale1_y,
    const AEVec2& position2, f32 scale2_x, f32 scale2_y) {
    // Calculate the edges of the first rectangle
    f32 left1 = position1.x - scale1_x / 2;
    f32 right1 = position1.x + scale1_x / 2;
    f32 top1 = position1.y + scale1_y / 2;
    f32 bottom1 = position1.y - scale1_y / 2;

    // Calculate the edges of the second rectangle
    f32 left2 = position2.x - scale2_x / 2;
    f32 right2 = position2.x + scale2_x / 2;
    f32 top2 = position2.y + scale2_y / 2;
    f32 bottom2 = position2.y - scale2_y / 2;

    // Check for overlap along both axes
    bool overlapX = (right1 >= left2) && (left1 <= right2);
    bool overlapY = (top1 >= bottom2) && (bottom1 <= top2);

    // If both overlap, there's a collision
    return overlapX && overlapY;
}


//------------------ Static Collision --------------------------//
bool CollisionIntersection_RectRect_Static(const AABB& object1, const AABB& object2)
{
    // Check if there is no overlap on the x-axis
    if (object1.max.x < object2.min.x || object1.min.x > object2.max.x)
        return false;

    // Check if there is no overlap on the y-axis
    if(object1.max.y < object2.min.y || object1.min.y > object2.max.y)
        return false;

    // If there is overlap on both axes, return true
    return true;
}
//--------------------------------------------------------------//


//------------------ Dynamic Collision --------------------------//
bool CollisionIntersection_RectRect_Dynamic(const AABB& object1, const AEVec2& object1_vel, 
    const AABB& object2, const AEVec2& object2_vel, 
    float& firstTimeOfCollision, float& gravity_dt) 
{
    AEVec2 relative_velocity;
    AEVec2 vel1_copy = object1_vel;
    AEVec2 vel2_copy = object2_vel;
    AEVec2Sub(&relative_velocity, &vel2_copy, &vel1_copy);

    float tFirst = 0;
    float tLast = static_cast<float>(gravity_dt);

    // Handle X-AXIS collision
    if (relative_velocity.x < 0.0f) { // object moving away
        // Case 1
        if (object1.min.x > object2.max.x)
            return false;
        // Case 4
        if (object1.max.x < object2.min.x)
            tFirst = fmaxf((object1.max.x - object2.min.x) / relative_velocity.x, tFirst);
        if (object1.min.x < object2.max.x)
            tLast = fminf((object1.min.x - object2.max.x) / relative_velocity.x, tLast);
    }
    else if (relative_velocity.x > 0.0f) { // object moving towards
        // Case 2
        if (object1.min.x > object2.max.x) // done
            tFirst = fmaxf((object1.min.x - object2.max.x) / relative_velocity.x, tFirst);
        if (object1.max.x > object2.min.x)
            tLast = fminf((object1.max.x - object2.min.x) / relative_velocity.x, tLast);
        // Case 3
        if (object1.max.x < object2.min.x)
            return false; // No collision
    }
    else { // if (relative_velocity == 0)
        // Case 5
        if (object1.max.x < object2.min.x || object1.min.x > object2.max.x) return false;
    }

    // Handle Y_AXIS collision
    if (relative_velocity.y < 0.0f) { // object moving away
        // Case 1
        if (object1.min.y > object2.max.y) {
            return false;
        }
        // Case 4
        if (object1.max.y < object2.min.y)
            tFirst = fmaxf((object1.max.y - object2.min.y) / relative_velocity.y, tFirst);
        if (object1.min.y < object2.max.y)
            tLast = fminf((object1.min.y - object2.max.y) / relative_velocity.y, tLast);
    }
    else if (relative_velocity.y > 0.0f) { // object moving towards
        // Case 2
        if (object1.min.y > object2.max.y) {
            tFirst = fmaxf((object1.min.y - object2.max.y) / relative_velocity.y, tFirst);
        }
        if (object1.max.y > object2.min.y) {
            tLast = fminf((object1.max.y - object2.min.y) / relative_velocity.y, tLast);
        }

        // Case 3
        if (object1.max.y < object2.min.y) {
            return false; // No collision
        }
    }
    else { // if (relative_velocity == 0)
        // Case 5
        if (object1.max.y < object2.min.y || object1.min.y > object2.max.y) {
            return false;
        }
    }

    // (Case 6) Check if tFirst > tLast, meaning no collision
    if (tFirst > tLast) {
        return false;
    }

    // Set the first time of collision
    firstTimeOfCollision = tFirst;
    std::cout << "Dynamic!" << std::endl;
    return true;
}
//---------------------------------------------------------------//


f32 lerp(f32 start, f32 end, f32 alpha) {
    return start + (end - start) * alpha;
}
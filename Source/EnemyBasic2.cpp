/* Start Header ************************************************************************/
/*!
\file		EnemyBasic2.cpp
\author		Tan Chun Kwang Xavier
\par		chunkwangxavier.tan@digipen.edu
\date		09/03/2025
\brief		This file implements the functions for the movement and collision of the
			enemy in the game world

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#include "EnemyBasic2.hpp"

EnemyBasic2::EnemyBasic2(std::shared_ptr<Object> parent) : EnemyHeaderClass(std::move(parent)) {}

void EnemyBasic2::Enter()
{
	//printString = "Enemy 2";
	RandomizeLimits();
	Object2D::Enter();
}

void EnemyBasic2::PreUpdate()
{
	Object2D::PreUpdate();
}

void EnemyBasic2::Update()
{
	UpdateVelocity();
	if (active) {
		SetNextPosition();
		UpdateEnemyBoundingBox();
		EnemyPlayerCollision();
	}

	Object2D::Update();
}

void EnemyBasic2::PostUpdate()
{
	Object2D::PostUpdate();
}

void EnemyBasic2::Exit()
{
	Object2D::Exit();
}

// Function to allow movement of enemy within the set bounds based off of its
// movement speed
void EnemyBasic2::SetNextPosition() {
	if (offset.x >= limitLeftRight.second) {
		reverseMove = true;
	}
	else if (offset.x <= limitLeftRight.first) {
		reverseMove = false;
	}

	if (!reverseMove) {
		offset.x += movementSpeed * deltaTime;
	}
	else {
		offset.x -= movementSpeed * deltaTime;
	}
}

// Added function to randomize movement of enemies
// Provides extra "fun" factor
void EnemyBasic2::RandomizeLimits() {
	// Randomize Leftmost and Rightmost bounds of enemy
	// Random selector between range of 1 to the maximum bounds of the map

	// Set the left most bounds
	float rdLeft = static_cast<float>(std::rand() % (maxBoundX - 1 + 1) + 1);
	rdLeft = -rdLeft;

	// Set the right most bounds
	float rdRight = static_cast<float>(std::rand() % (maxBoundX - 1 + 1) + 1);

	// Apply to the enemy
	limitLeftRight = { rdLeft, rdRight };
}
/* Start Header ************************************************************************/
/*!
\file		EnemyBasic1.cpp
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

#include "EnemyBasic1.hpp"

EnemyBasic1::EnemyBasic1(std::shared_ptr<Object> parent) : EnemyHeaderClass(std::move(parent)) {}

void EnemyBasic1::Enter()
{
	//printString = "Enemy 1";
	movementSpeed = 2.0f;
	Object2D::Enter();
}

void EnemyBasic1::PreUpdate()
{
	Object2D::PreUpdate();
}

void EnemyBasic1::Update()
{
	UpdateBeeVelocity();
	if (active) {
		iterator += deltaTime;
		SetNextPosition();
		if (iterator <= 0.0f) {
			iterator = 0.0f;
		}
		else if (iterator > 2 * PI) {
			iterator = 0.0f;
		}

		UpdateEnemyBoundingBox();
		EnemyPlayerCollision();
	}

	Object2D::Update();
}

void EnemyBasic1::PostUpdate()
{
	Object2D::PostUpdate();
}

void EnemyBasic1::Exit()
{
	Object2D::Exit();
}


void EnemyBasic1::SetNextPosition() {
	offset.x = cosf(movementSpeed * iterator) * radius;
	offset.y = sinf(movementSpeed * iterator) * radius;
}

void EnemyBasic1::UpdateBeeVelocity() {
	velocity = { movementSpeed * deltaTime + velocity.x, movementSpeed * deltaTime + velocity.y };
}
/* Start Header ************************************************************************/
/*!
\file		EnemyBasic2.hpp
\author		Tan Chun Kwang Xavier
\par		chunkwangxavier.tan@digipen.edu
\date		09/03/2025
\brief		This file implements the second enemy type of the game where
            the movement of this enemy is to move back and forth on a horizontal plane

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#pragma once
#include "AEEngine.h"
#include "GameDefn.hpp"

class EnemyBasic2 : public EnemyHeaderClass
{
public:
	EnemyBasic2(std::shared_ptr<Object> parent = nullptr);

	void Enter() override;

	void PreUpdate() override;

	void Update() override;

	void PostUpdate() override;

	void Exit() override;

protected:
	const int maxBoundX = 400;	// Max bounds of the map
	const f32 movementSpeed = 400.0f;	// Speed at which to move enemy

	std::pair<float, float> limitLeftRight = { 0.0f, 0.0f };	// Max bounds of left and right

	bool reverseMove = false;	// Boolean to check left right movement

	//---- Initializer for enemy limits ----//
	void RandomizeLimits();

	//---- Movement of enemy ----//
	void SetNextPosition();
};
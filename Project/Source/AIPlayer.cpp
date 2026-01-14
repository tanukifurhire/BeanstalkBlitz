/* Start Header ************************************************************************/
/*!
\file		AIPlayer.cpp
\author		Vu Phan Hung
\par		phanhung.vu@digipen.edu
\date		09/03/2025
\brief		This file implements 

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#include "AIPlayer.hpp"
#include "PlayerMovementStateMachine.hpp"
#include "Object2D.hpp"
#include "random"
#include "Map.hpp"
#include "InputManager.hpp"

AIPlayer::AIPlayer(std::shared_ptr<Object> parent) : Player(std::move(parent)) {}

void AIPlayer::AIMap(std::vector<Map::MapObject> map)
{
	mapObject = map;
}

void AIPlayer::Enter()
{
	movementStateMachine = new PlayerMovementStateMachine();

	movementStateMachine->player = std::dynamic_pointer_cast<Player>(shared_from_this());

	movementStateMachine->ChangeState(&movementStateMachine->idleState);

	movementStateMachine->active = true;

	input = std::dynamic_pointer_cast<InputManager>(GetChildren().at(5));

	Object2D::Enter();
}

void AIPlayer::PreUpdate()
{
	//AI own input

	////random input//
	//std::random_device dev;
	//std::mt19937 rng(dev());
	//std::uniform_int_distribution<std::mt19937::result_type> input(0, 4); // distribution in range [1, 6]
	//AIMovement(input(rng));

	//introduce "noise" here
	Object2D::PreUpdate();

	AIProcessMovement();

	movementStateMachine->HandleInput();
}

void AIPlayer::Update()
{
	movementStateMachine->Update();

	Object2D::Update();

	player_input = { 0.0f, 0.0f };
}

void AIPlayer::PostUpdate()
{
	//std::cout << "POSTUpdate";

	movementStateMachine->PostUpdate();

	Object2D::PostUpdate();
}

void AIPlayer::Exit()
{

	Player::Exit();
}

//void AIPlayer::AIMovement(int input)     //choose its input
//{
//	switch (input)
//	{
//	case 0:	//AEInputCheckCurr(AEVK_A)
//		player_input.x -= 1.0f;
//		break;
//	case 1:	//AEInputCheckCurr(AEVK_D)
//		player_input.x += 1.0f;
//		break;
//	case 2:	//AEInputCheckCurr(AEVK_SPACE)
//
//		//offset.x += player_input.x * GetSpeed() * deltaTime;
//
//		//if (jumpForce <= 50.0f) {
//		//	SetJumpForce(0.0f);
//		//	return;
//		//}
//
//		//offset.y += AEClamp(jumpForce * deltaTime, 0.0f, 2000.0f * deltaTime);
//
//		//jumpForce -= 5000.0f * deltaTime;
//
//		//SetJumpForce(jumpForce);
//
//		//SetPosition(offset);
//		break;
//	case 3:	//maybe the power up thingy
//
//		break;
//	case 4:	//maybe something else
//
//		break;
//	}
//}

void AIPlayer::AIProcessMovement()
{
//	//keep track of AI platform first before providing any input
////the AI made it pass the current platform
//	while (this->offset.y > next.y)
//	{
//		std::cout << "PASSING A PLATFORM";
//		nextPlatform++;	//go to the next platform
//	}
//	//the AI drop down
//	while (this->offset.y < next.y)
//	{
//		std::cout << "THE AI Drop Down";
//		nextPlatform--;
//	}
//
//	if (nextPlatform >= mapObject.size() - 1)
//		return;
//
//	Map::MapObject curr = mapObject.at(nextPlatform);		//current platform
//	Map::MapObject next = mapObject.at(nextPlatform + 1);	//next platform
//
//
//	//std::cout << "Currnet platform x and y " << curr.x << ' ' << curr.y << '\n';
//	//std::cout << "NEXT platform x and y " << next.x << ' ' << next.y << '\n';
//
//	//one AI idea would be to make it always move to the edge of platform before jumping up
//
//	//AEVec2 offset = GetLocalPosition();
//
//	//player world position compare to platform world position to find which edge to go to
//	//if current platform is to the left of next platform
//	if (this->offset.x < next.x)
//	{
//		//move to the right of the current platform
//		while (this->offset.x + (0.7f) < curr.x)
//		{
//			std::cout << "GOING RIGHT OF CURRENT PLATFORM\n";
//			//AIMovement(1);
//			//offset.x += player_input.x * GetSpeed() * deltaTime;
//		}
//
//		player_input = { 0.0f, 0.0f };
//		//AIMovement(2);
//
//	}
//	//if current platform is to the right of the next platform
//	else
//	{
//		//move to the left of the current platform
//		while (this->offset.x + (0.7f) > curr.x)
//		{
//			std::cout << "GOING LEFT OF CURRENT PLATFORM\n";
//
//			//AIMovement(0);
//			//offset.x += player_input.x * GetSpeed() * deltaTime;
//		}
//
//		player_input = { 0.0f, 0.0f };
//		//AIMovement(2);
//	}
}
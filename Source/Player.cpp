/* Start Header ************************************************************************/
/*!
\file		Player.cpp
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

#include "Player.hpp"
#include "GameDefn.hpp"
#include "PlayerMovementStateMachine.hpp"

Player::Player(std::shared_ptr<Object> parent) : Object2D(std::move(parent)), movementStateMachine() {}

// Init
void Player::Enter() {

	movementStateMachine = new PlayerMovementStateMachine();

	movementStateMachine->player = std::dynamic_pointer_cast<Player>(shared_from_this());

	movementStateMachine->ChangeState(&movementStateMachine->idleState);

	movementStateMachine->active = true;

    Object2D::Enter();
}

// Input
void Player::PreUpdate() {
	Object2D::PreUpdate();
	velocity.x = 0.0f;
	velocity.y = 0.0f;

	movementStateMachine->HandleInput();
}

// Physics / logic
void Player::Update() {
	movementStateMachine->Update();

	// DO NOT REMOVE
	Object2D::Update();

	player_input = { 0.0f, 0.0f };
}

void ServerPlayer::Update() {

	Object2D::Update();

	player_input = { 0.0f, 0.0f };
}

// Graphics
void Player::PostUpdate() {

	movementStateMachine->PostUpdate();

	Object2D::PostUpdate();
}

// Free
void Player::Exit() {
	movementStateMachine->Exit();

	if (movementStateMachine) {
		delete movementStateMachine;
	}

    Object2D::Exit();
}

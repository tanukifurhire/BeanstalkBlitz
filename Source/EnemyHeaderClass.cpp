/* Start Header ************************************************************************/
/*!
\file		EnemyHeaderClass.cpp
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

#include "GameDefn.hpp"
#include "PlayerMovementStateMachine.hpp"

EnemyHeaderClass::EnemyHeaderClass(std::shared_ptr<Object> parent) : Object2D(std::move(parent)) {}

void EnemyHeaderClass::Enter()
{
	// empty on purpose, overridable

	// but here is a bunny if you insist
	/*
	// (\(\			\|/
	// ( -.-)		-o-
	// o_(")(")		/|\

	*/
	// look at him, just chilling in the sun

	Object2D::Enter();
}

void EnemyHeaderClass::PreUpdate()
{
	// empty on purpose, overridable

	// but here is a bunny if you insist
	/*
	// (\(\			\|/
	// ( -.-)		-o-
	// o_(")(")		/|\

	*/
	// look at him, just chilling in the sun

	Object2D::PreUpdate();
}

void EnemyHeaderClass::Update()
{
	// empty on purpose, overridable

	// but here is a bunny if you insist
	/*
	// (\(\			\|/
	// ( -.-)		-o-
	// o_(")(")		/|\

	*/
	// look at him, just chilling in the sun

	Object2D::Update();
}

void EnemyHeaderClass::PostUpdate()
{
	// empty on purpose, overridable

	// but here is a bunny if you insist
	/*
	// (\(\			\|/
	// ( -.-)		-o-
	// o_(")(")		/|\

	*/
	// look at him, just chilling in the sun

	Object2D::PostUpdate();
}

void EnemyHeaderClass::Exit()
{
	// empty on purpose, overridable

	// but here is a bunny if you insist
	/*
	// (\(\			\|/
	// ( -.-)		-o-
	// o_(")(")		/|\

	*/
	// look at him, just chilling in the sun

	Object2D::Exit();
}

// Updates the bounding box of the enemy
void EnemyHeaderClass::UpdateEnemyBoundingBox() {
	enemyBounds.min.x = -0.5f * GetScale().x + GetPosition().x;
	enemyBounds.min.y = -0.5f * GetScale().y + GetPosition().y;
	enemyBounds.max.x = +0.5f * GetScale().x + GetPosition().x;
	enemyBounds.max.y = +0.5f * GetScale().y + GetPosition().y;
}

// Retrives the bounding box of the player
EnemyHeaderClass::AABB EnemyHeaderClass::GetPlayerBounds(AEVec2 const& playerPos, AEVec2 const& playerScale) {
	AABB playerBounds{};

	playerBounds.min.x = -0.5f * playerScale.x + playerPos.x;
	playerBounds.min.y = -0.5f * playerScale.y + playerPos.y;
	playerBounds.max.x = +0.5f * playerScale.x + playerPos.x;
	playerBounds.max.y = +0.5f * playerScale.y + playerPos.y;

	return playerBounds;
}

void EnemyHeaderClass::EnemyPlayerCollision() {
	std::shared_ptr<Object2D> players = std::dynamic_pointer_cast<Object2D>(GetRoot()->GetChildren().at(1));

	AEVec2 playerCoords{}, playerScale{};
	AABB playerBounds{};

	for (std::shared_ptr<Object> child : players->GetChildren()) {
		std::shared_ptr<Player> player = std::dynamic_pointer_cast<Player>(child);

		if (std::dynamic_pointer_cast<AIPlayer>(child)) {
			continue;
		}

		if (player) {
			f32 tFirst{};
			if (CollisionIntersection_RectRect(player->bounding_box, player->velocity, bounding_box, velocity, tFirst, deltaTime)) {
				if (player->GetPlayerStateMachine()->current_state != &player->GetPlayerStateMachine()->stunState) {
					if (player->GetPlayerStateMachine()->active && player->GetPlayerStateMachine()->current_state != &player->GetPlayerStateMachine()->flyingState) {
						dynamic_cast<PlayerStunState*>(&player->GetPlayerStateMachine()->stunState)->pos_x = player->GetLocalPosition().x;
						dynamic_cast<PlayerStunState*>(&player->GetPlayerStateMachine()->stunState)->pos_y = player->GetLocalPosition().y + (player->velocity.y < 0.f ? -player->GetScale().y / 2.f : player->GetScale().y / 2.f);
						dynamic_cast<PlayerStunState*>(&player->GetPlayerStateMachine()->stunState)->stun_type = ENEMY_STUN;
						
						player->GetPlayerStateMachine()->ChangeState(&player->GetPlayerStateMachine()->stunState);
					}
				}
			}
		}
	}
}

void EnemyHeaderClass::EnemyPlayerCollision(const AABB& incomingAABB, const AEVec2& incomingVel) {
	std::shared_ptr<Object2D> players = std::dynamic_pointer_cast<Object2D>(GetRoot()->GetChildren().at(1));

	AEVec2 playerCoords{}, playerScale{};
	AABB playerBounds{};

	auto box = bounding_box;
	bounding_box.max = incomingAABB.max;
	bounding_box.min = incomingAABB.min;

	for (std::shared_ptr<Object> child : players->GetChildren()) {
		std::shared_ptr<Player> player = std::dynamic_pointer_cast<Player>(child);

		if (std::dynamic_pointer_cast<AIPlayer>(child)) {
			continue;
		}

		if (player) {
			f32 tFirst{};
			if (CollisionIntersection_RectRect(player->bounding_box, player->velocity, bounding_box, incomingVel, tFirst, deltaTime)) {
				if (player->GetPlayerStateMachine()->current_state != &player->GetPlayerStateMachine()->stunState) {
					if (player->GetPlayerStateMachine()->active && player->GetPlayerStateMachine()->current_state != &player->GetPlayerStateMachine()->flyingState) {
						dynamic_cast<PlayerStunState*>(&player->GetPlayerStateMachine()->stunState)->pos_x = player->GetLocalPosition().x;
						dynamic_cast<PlayerStunState*>(&player->GetPlayerStateMachine()->stunState)->pos_y = player->GetLocalPosition().y + (player->velocity.y < 0.f ? -player->GetScale().y / 2.f : player->GetScale().y / 2.f);
						dynamic_cast<PlayerStunState*>(&player->GetPlayerStateMachine()->stunState)->stun_type = ENEMY_STUN;

						player->GetPlayerStateMachine()->ChangeState(&player->GetPlayerStateMachine()->stunState);
					}
				}
			}
		}
	}

	bounding_box = box;
}

void EnemyHeaderClass::UpdateVelocity() {
	velocity = { movementSpeed * deltaTime + velocity.x, 0.0f };
}
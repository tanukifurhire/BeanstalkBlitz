/* Start Header ************************************************************************/
/*!
\file		EnemyBoss.cpp
\author		Tan Chun Kwang Xavier
\par		chunkwangxavier.tan@digipen.edu
\date		09/03/2025
\brief		This file implements the functions for the spawning, dropping and collision
			of the projectiles with the players in the game world together with the boss
			behaviour

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#include "EnemyBoss.hpp"

EnemyBoss::EnemyBoss(std::shared_ptr<Object> parent) : EnemyHeaderClass(std::move(parent)) {}

void EnemyBoss::Enter()
{
	AEGfxMeshStart();

	DrawSquare();

	boulderMesh = AEGfxMeshEnd();


	for (int i{}; i < BOULDER_MAX_ARR; ++i) {
		ResetBoulderProperties(boulders[i]);
	}

	pTex = AEGfxTextureLoad("Assets/Boulder.png");

	active = false;

	//printString = "Boss";
	//printString2 = "Boulder";
	Object2D::Enter();
}

void EnemyBoss::PreUpdate()
{
	if (active) {
		CountNumPlayers();
	}
	//std::cout << "Total Number of players: " << numPlayers << '\n';
	Object2D::PreUpdate();
}

void EnemyBoss::Update()
{
	if (active) {
		//std::cout << "Boss Active!" << std::endl;
		BossStateManager(currBossState);
		BoulderDropping();
		UpdateBoulderVelocity();
		UpdateBouldersBoundingBox();
		BoulderPlayerCollision();
		DespawnBoulder();
		/*if (!idleBoss) {
			SetBossIdle();
		}*/
		TransitionState();
	}
	else {
		timer = 2.f;
		currBossState = SEARCHING;
	}
	//PrintBoulderDetails();
	Object2D::Update();
}

void EnemyBoss::PostUpdate()
{
	Object2D::PostUpdate();
	DrawBoulders();
}

void EnemyBoss::Exit()
{
	AEGfxTextureUnload(pTex);
	AEGfxMeshFree(boulderMesh);
	Object2D::Exit();
}

// This function will create a square mesh
void EnemyBoss::DrawSquare() {
	AEGfxTriAdd(
		-1.0f, 1.0f, 0xFF000000, 0.0f, 0.0f,
		-1.0f, -1.0f, 0xFF000000, 0.0f, 1.0f,
		1.0f, -1.0f, 0xFF000000, 1.0f, 1.0f
	);
	AEGfxTriAdd(
		-1.0f, 1.0f, 0xFF000000, 0.0f, 0.0f,
		1.0f, 1.0f, 0xFF000000, 1.0f, 0.0f,
		1.0f, -1.0f, 0xFF000000, 1.0f, 1.0f
	);
}

// Counting of total number of players:
void EnemyBoss::CountNumPlayers() {	
	int totalNumPlayers{};

	// For each player in the game,	
	for (std::shared_ptr<Object> child : GetPlayerBranch()->GetChildren()) {
		std::shared_ptr<Player> player = std::dynamic_pointer_cast<Player>(child);

		// Count player
		if (player) {
			++totalNumPlayers;
		}
	}

	numPlayers = totalNumPlayers;
}


void EnemyBoss::ResetBoulderProperties(Boulder& boulder) {
	boulder.isActive = false;
	boulder.allowFall = false;
	boulder.pos = offset;
	boulder.boulderScale = BOULDER_SCALLING;
	boulder.liftSpeed = BOULDER_LIFT_SPEED;
	boulder.dropSpeed = BOULDER_FALL_SPEED;
	boulder.dropVelocity.x = 0.f;
	boulder.dropVelocity.y = 0.f;
}

void EnemyBoss::UpdateBoulderVelocity() {
	for (int i{}; i < BOULDER_MAX_ARR; ++i) {
		if (!boulders[i].isActive) {
			continue;
		}
		boulders[i].dropVelocity.y = BOULDER_LIFT_SPEED * deltaTime + boulders[i].dropVelocity.y;
	}
}

// Updates the bounding box of the each boulder that is currently active in the game
void EnemyBoss::UpdateBouldersBoundingBox() {
	for (int i{}; i < BOULDER_MAX_ARR; ++i) {
		if (!boulders[i].isActive) {
			continue;
		}
		boulders[i].boulderAABB.min.x = -0.5f * boulders[i].boulderScale.x + boulders[i].pos.x;
		boulders[i].boulderAABB.min.y = -0.5f * boulders[i].boulderScale.y + boulders[i].pos.y;
		boulders[i].boulderAABB.max.x = +0.5f * boulders[i].boulderScale.x + boulders[i].pos.x;
		boulders[i].boulderAABB.max.y = +0.5f * boulders[i].boulderScale.y + boulders[i].pos.y;
	}
}

void EnemyBoss::BoulderPlayerCollision() {
	for (int i{}; i < BOULDER_MAX_ARR; ++i) {
		if (!boulders[i].isActive) {
			continue;
		}
		EnemyPlayerCollision(boulders[i].boulderAABB, boulders[i].dropVelocity);
	}
}

// Choosing a random player
bool EnemyBoss::SelectRandomPlayer() {
	// If there are no players, ignore
	if (numPlayers <= 0) {
		return false;
	}
	
	rd_num = 0;
	int selected_id{ -1 };

	int i = 0;

	f32 y_pos = -300;

	for (auto& child : GetPlayerBranch()->GetChildren()) {
		std::shared_ptr<Player> player = std::dynamic_pointer_cast<Player>(child);

		if (player) {
			if (y_pos < player->GetLocalPosition().y) {
				y_pos = player->GetLocalPosition().y;

				if (GetNetworkClient()->player_id >= 0) {
					std::stringstream ss;

					if (i == 0) {
						ss << GetNetworkClient()->player_id;
					}
					else {
						ss << player->GetName();
					}

					int id{};

					ss >> id;

					selected_id = id;
				}

				rd_num = i;
			}
			else if (AEIsF32Equal(player->GetLocalPosition().y, y_pos)) {
				if (GetNetworkClient()->player_id >= 0) {
					std::stringstream ss;

					ss << player->GetName();

					int id{};

					ss >> id;

					if (id < selected_id) {
						selected_id = id;
						rd_num = i;
					}

					std::cout << "Equal! Current Selected: " << selected_id << " New Selected: " << id << std::endl;
				}
			}

			i++;
		}
	}

	if (y_pos == -300) {
		return false;
	}

	std::cout << "Player {" << selected_id << "} selected!!!\n";
	return true;
}


f32 EnemyBoss::RetrivePlayerXPos() {
	f32 xpos{};
	int i{ 0 };

	for (std::shared_ptr<Object> child : GetPlayerBranch()->GetChildren()) {
		std::shared_ptr<Player> player = std::dynamic_pointer_cast<Player>(child);

		if (player) {
			if (i == rd_num) {
				xpos = player->GetPosition().x;
			}
			++i;
		}
	}

	return xpos;
}

bool EnemyBoss::BossMovement() {
	if ((offset.x >= targetPosX - 0.5f) && (offset.x <= targetPosX + 0.5f)) {
		return true;
	}

	std::shared_ptr<SpriteRenderer> sprite = std::dynamic_pointer_cast<SpriteRenderer>(GetChildren().at(0));

	// If boss is to the left of the player (Relative X position)
	if (offset.x < targetPosX) {
		sprite->SetScale({ sprite->GetScale().x < 0 ? -sprite->GetScale().x : sprite->GetScale().x, sprite->GetScale().y });
		offset.x += bossMovementSpeed * deltaTime;
	}
	// If boss is to the right of the player (Relative X position)
	else if (offset.x > targetPosX) {
		sprite->SetScale({ sprite->GetScale().x > 0 ? -sprite->GetScale().x : sprite->GetScale().x, sprite->GetScale().y });
		offset.x -= bossMovementSpeed * deltaTime;
	}

	return false;
}

// Returns the index of a newly refreshed boulder object
int EnemyBoss::SpawnBoulder() {
	int i{};
	for (; i < BOULDER_MAX_ARR; ++i) {
		if (!boulders[i].isActive) {
			ResetBoulderProperties(boulders[i]);
			boulders[i].isActive = true;
			break;
		}
	}
	return i;
}

bool EnemyBoss::LiftBoulder(Boulder& currBoulder) {
	if ((currBoulder.pos.y - offset.y) >= liftMaxHeight) {
		return true;
	}

	// Do lifting here
	currBoulder.pos.y += BOULDER_LIFT_SPEED * deltaTime;
	return false;
}

bool EnemyBoss::ReleaseBoulder(Boulder& currBoulder) {
	currBoulder.allowFall = true;
	return true;
}

void EnemyBoss::BoulderDropping() {
	for (int i{}; i < BOULDER_MAX_ARR; ++i) {
		if (!boulders[i].isActive || !boulders[i].allowFall) {
			continue;
		}
		boulders[i].pos.y -= boulders[i].dropVelocity.y * deltaTime;
	}
}

// INVALID CODE (REMOVABLE)
void EnemyBoss::SetBossIdle() {
	bool availBoulders{ false };
	for (int i{}; i < BOULDER_MAX_ARR; ++i) {
		if (boulders[i].isActive) {
			continue;
		}
		availBoulders = true;
	}

	if (!availBoulders) {
		currBossState = IDLE;
		nextBossState = IDLE;
	}
	else {
		currBossState = SEARCHING;
	}
}

void EnemyBoss::DrawBoulders() {
	for (int i{}; i < BOULDER_MAX_ARR; ++i) {
		if (!boulders[i].isActive) {
			continue;
		}

		if (!boulders[i].allowFall) {
			continue;
		}

		AEMtx33 boulderTransform;
		AEMtx33Identity(&boulderTransform);

		AEMtx33 boulderScale;
		AEMtx33Scale(&boulderScale, boulders[i].boulderScale.x, boulders[i].boulderScale.y);

		AEMtx33 boulderRotation{ 0.0f };
		AEMtx33Identity(&boulderRotation);
		AEMtx33Rot(&boulderRotation, 0.0f);

		AEMtx33 boulderTranslate;

		AEMtx33Trans(&boulderTranslate, boulders[i].pos.x - camera_x, boulders[i].pos.y - camera_y);

		AEMtx33Concat(&boulderTransform, &boulderRotation, &boulderScale);
		AEMtx33Concat(&boulderTransform, &boulderTranslate, &boulderTransform);

		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);

		//AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);

		//AEGfxSetColorToAdd(1.0f, 0.69f, 0.027f, 1.0f);

		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		AEGfxSetTransparency(1.0f);

		AEGfxTextureSet(pTex, 0.f, 0.f);

		AEGfxSetTransform(boulderTransform.m);

		AEGfxMeshDraw(boulderMesh, AE_GFX_MDM_TRIANGLES);
	}
}

// Removes the currently selected boulder from the screen
void EnemyBoss::DestroyBoulder(Boulder& currBoulder) {
	currBoulder.isActive = false;
}

// Checker to determine when to remove the boulder
void EnemyBoss::DespawnBoulder() {
	for (int i{}; i < BOULDER_MAX_ARR; ++i) {
		if (!boulders[i].isActive) {
			continue;
		}

		if ((offset.y - boulders[i].pos.y) >= BOULDER_FALL_DIST) {
			DestroyBoulder(boulders[i]);
		}
	}
}

void EnemyBoss::BossStateManager(BossStates& currState) {
	std::shared_ptr<SpriteRenderer> sprite = std::dynamic_pointer_cast<SpriteRenderer>(GetChildren().at(0));

	switch (currState) {
		case IDLE:
			//std::cout << "Idling...\n";
			// empty on purpose, overridable
			
			// but here is a bunny if you insist
			/*
			 
			// (\(\			\|/
			// ( -.-)		-o-
			// o_(")(")		/|\

			*/
			// look at him, just chilling in the sun
			break;
		case SEARCHING:
			//std::cout << "Now selecting player...\n";
			timer -= deltaTime;
			
			if (timer < 0.f) {
				if (SelectRandomPlayer()) {
					searchPlayer = true;
					nextBossState = MOVING;
					sprite->SetState(&sprite->bossWalk);
				}
				timer = 2.f;
			}

			break;
		case MOVING:
			//std::cout << "Now moving to player position...\n";
			targetPosX = RetrivePlayerXPos();
			if (BossMovement()) {
				newlySpawnedBoulder = SpawnBoulder();
				nextBossState = LIFTING;
				bossMove = true;
				sprite->SetState(&sprite->bossLift);
			}
			break;
		case LIFTING:
			//std::cout << "Now lifting up boulder...\n";
			timer -= deltaTime;
			if (timer <= 0.f) {
				timer = 2.f;
				liftBoulder = true;
				nextBossState = DROPPING;
				sprite->SetState(&sprite->bossAttack);
			}
			break;
		case DROPPING:
			//std::cout << "Now dropping boulder: " << newlySpawnedBoulder << "...\n";
			if (ReleaseBoulder(boulders[newlySpawnedBoulder])) {
				dropBoulder = true;
				nextBossState = SEARCHING;
			}
			break;
	}
}

// Changing of boss states:
void EnemyBoss::TransitionState() {
	// Based on a series of checks,
	if (searchPlayer || bossMove || liftBoulder || dropBoulder) {
		// Change the current state of boss
		currBossState = nextBossState;
		// Reset all boolean flags
		searchPlayer = bossMove = liftBoulder = dropBoulder = false;
	}
}

void EnemyBoss::PrintBoulderDetails() {
	std::cout << "<-------------------------------------->\n";
	for (int i{}; i < BOULDER_MAX_ARR; ++i) {
		std::cout << "Boulder " << i << ":\n";
		std::cout << "isActive: " << boulders[i].isActive << ":\n";
		std::cout << "pos x: " << boulders[i].pos.x << ":\n";
		std::cout << "pos y: " << boulders[i].pos.y << ":\n";
	}
	std::cout << "<-------------------------------------->\n";
}

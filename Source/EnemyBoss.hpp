/* Start Header ************************************************************************/
/*!
\file		EnemyBoss.hpp
\author		Tan Chun Kwang Xavier
\par		chunkwangxavier.tan@digipen.edu
\date		09/03/2025
\brief		This file implements the final enemy type of the game where
            the enemy is a boss which will drop projectiles down onto the players

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#pragma once
#include "AEEngine.h"
#include "GameDefn.hpp"

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// NOTE: THIS ENEMY CAN ONLY BE CREATED AFTER ALL PLAYERS HAVE BEEN CREATED
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

class EnemyBoss : public EnemyHeaderClass
{
public:
	EnemyBoss(std::shared_ptr<Object> parent = nullptr);

	void Enter() override;

	void PreUpdate() override;

	void Update() override;

	void PostUpdate() override;

	void Exit() override;

protected:
	AEGfxTexture *pTex = nullptr;

	f32 timer{ 2.f };

	// Constant variables used by the boulders
	static const int BOULDER_MAX_ARR = 5;
	const f32 BOULDER_LIFT_SPEED = 40.0f;
	const f32 BOULDER_FALL_SPEED = 10000.0f;
	const f32 BOULDER_FALL_DIST = 10400.0f;
	const AEVec2 BOULDER_SCALLING = { 30.0f, 30.0f };

	// Constant variables used by the boss
	const f32 bossMovementSpeed = 50.0f;
	const f32 liftMaxHeight = 75.0f;  // Height to lift boulder to
	//const f32 SOMETHINGGOESHERE = SOMEVALUE;

	// Rock that the final boss will toss down at players
	struct Boulder {
		bool isActive{ false };		// Bool to check if current Boulder is 'alive'
		bool allowFall{ false };	// Bool to check if boulder is allowed to 'fall'

		// Color of the Boulder
		u32 boulderColor = 0xFFFFB07C;
		
		AEVec2 pos{};				// Position of Boulder
		AEVec2 boulderScale{};		// Scaling of Boulder
		AABB boulderAABB{};			// Bounding collision box of Boulder

		f32 liftSpeed{};			// Speed of the boulder being lifted
		f32 dropSpeed{};			// Speed of the falling boulder
		AEVec2 dropVelocity{};
	};

	// States at which the boss can be in
	enum BossStates {
		IDLE,
		SEARCHING,		// Look for new player
		MOVING,			// Move to same X pos as selected player
		LIFTING,		// Lift up newly spawned boulder
		DROPPING		// Drop the spawned boulder
	};

	// Mesh to draw the Boulder
	AEGfxVertexList* boulderMesh = nullptr;
	// Creating an array of 'BOULDER_MAX_ARR' boulders
	Boulder boulders[BOULDER_MAX_ARR]{};

	int numPlayers{ 0 };		// Total number of players in the game
	int rd_num{ 0 };			// Random number selector
	BossStates currBossState{SEARCHING};	// Current boss state
	BossStates nextBossState{};				// Next boss state
	int newlySpawnedBoulder{};	// Index of a newly created boulder

	//<-------- Initializers -------->//
	void DrawSquare();				// Creating of mesh to be used by the boulders
	void CountNumPlayers();			// Checks for the total number of players in the game
	void ResetBoulderProperties(Boulder&); // Initializing variables of boulders

	//<-------- Bounding Box Stuff -------->//
	// Updating of bounding boxes of all currently Active boulders in the game
	void UpdateBouldersBoundingBox();
	void UpdateBoulderVelocity();
	void BoulderPlayerCollision();


	//<-------- Movement Stuff -------->//
	// Boolean checkers
	//bool idleBoss{ false };
	bool searchPlayer{ false };
	bool bossMove{ false };
	bool liftBoulder{ false };
	bool dropBoulder{ false };
	f32 targetPosX{};

	//<-------- Movement Stuff : SUBSECTION (Boss States) -------->//
	// 1.get number of players in the game
	//    - use random selection to choose a player
	// 2. stop when x pos of player and x pos of boss matches
	// 3. spawn 1 boulder
	// 4. lift up boulder
	// 5. drop boulder
	bool SelectRandomPlayer();
	f32 RetrivePlayerXPos();
	bool BossMovement();
	int SpawnBoulder();
	bool LiftBoulder(Boulder&);
	bool ReleaseBoulder(Boulder&);
	void BoulderDropping();
	void SetBossIdle();

	//<-------- Drawing Stuff -------->//
	void DrawBoulders();			// Graphics

	//<-------- Despawning Stuff -------->//
	void DestroyBoulder(Boulder&);	// Checker to determine when to remove a boulder
	void DespawnBoulder();			// Boulder remover

	//<-------- Boss State Manager -------->//
	void BossStateManager(BossStates&);		// Manager to determine what to do during each state of boss
	void TransitionState();					// This is to facilitate changing of boss states

	//<-------- Debugging -------->//
	void PrintBoulderDetails();
};
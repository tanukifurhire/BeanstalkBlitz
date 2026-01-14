/* Start Header ************************************************************************/
/*!
\file		EnemyBasic3.cpp
\author		Tan Chun Kwang Xavier
\par		chunkwangxavier.tan@digipen.edu
\date		09/03/2025
\brief		This file implements the functions for the spawning, shooting and collision
			of the enemy with the players in the game world

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#include "EnemyBasic3.hpp"

EnemyBasic3::EnemyBasic3(std::shared_ptr<Object> parent) : EnemyHeaderClass(std::move(parent)) {}

// Init
void EnemyBasic3::Enter()
{
	AEGfxMeshStart();

	DrawSquare();

	bullet->bulletMesh = AEGfxMeshEnd();
	eggLeftText = AEGfxTextureLoad("Assets/eggLeft.png");
	eggRightText = AEGfxTextureLoad("Assets/eggRight.png");
	CheckOffSidePosition();	// This function should fix enemy shooting the wrong way
	CheckStartingPos();
	bullet->startFixedPos = bullet->pos;
	bullet->bulletSpeed = 50.0f;
	SetBulletLimits();
	SetBulletScaling();
	PrintBulletStats();
	//std::cout << "Offset: (x: " << offset.x << ", y: " << offset.y << ")\n";
	Object2D::Enter();
}

// Inputs
void EnemyBasic3::PreUpdate()
{
	//printString = "Enemy 3";
	//printString2 = "Enemy 3 bullet";
	Object2D::PreUpdate();
}

// Physics / Logic
void EnemyBasic3::Update()
{
	if (active) {
		// Movement of bullet functions
		MoveBullet();
		CheckBulletBounds();

		// Updating of bounding boxes of bullet and enemy
		UpdateEnemyBoundingBox();
		UpdateBulletBoundingBox();

		// Checking collision with player of enemy and bullet:
		EnemyPlayerCollision();
		EnemyPlayerCollision(bullet->bulletAABB, {});
	}

	Object2D::Update();
}
// Graphics
void EnemyBasic3::PostUpdate()
{
	// Rendering of bullet
	DrawBullet();
	Object2D::PostUpdate();
}

// Free
void EnemyBasic3::Exit()
{
	// Freeing of memory (sugoi important)
	AEGfxMeshFree(bullet->bulletMesh);
	if (eggLeftText) {
		AEGfxTextureUnload(eggLeftText);
	}
	if (eggRightText) {
		AEGfxTextureUnload(eggRightText);
	}
	delete bullet;
	Object2D::Exit();
}

int EnemyBasic3::GetOffSide() {
	return (offSide == LEFT) ? 0 : 1;
}

// This function will create a circle mesh
// it takes in the number of triangles required to draw the circle
void EnemyBasic3::DrawCircles(int numberTriangles) {

	// this is to convert the angle to get points on the triangles to radian
	f32 subSections = 1.0f / static_cast<f32>(numberTriangles);

	// draw the circle based on the number of triangles taken in by the function
	for (int i{ 0 }; i < numberTriangles; ++i) {
		AEGfxTriAdd(
			0.0f, 0.0f, bulletColor, 0.0f, 0.0f,
			cosf(i * subSections * 2 * PI), sinf(i * subSections * 2 * PI), bulletColor, 0.0f, 0.0f,
			cosf((i + 1) * subSections * 2 * PI), sinf((i + 1) * subSections * 2 * PI), bulletColor, 0.0f, 0.0f
		);
	}
}

// This function will create a square mesh
void EnemyBasic3::DrawSquare() {
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

// Initializing of information of where the enemy had spawn on the map
void EnemyBasic3::CheckOffSidePosition() {
	// Enemy spawns on the far left of the map
	if (offset.x >= maxMapBounds.first - 3.0f && offset.x <= maxMapBounds.first + 3.0f) {
		offSide = LEFT;
	}
	// Enemy spawns on the far right of the map
	else if (offset.x <= maxMapBounds.second + 3.0f && offset.x >= maxMapBounds.second - 3.0f) {
		offSide = RIGHT;
	}
}

// This function will determine which side the bullet will fire out towards
void EnemyBasic3::CheckBulletBounds() {
	if (offSide == LEFT) {
		if (bullet->pos.x >= bullet->bulletLimitBounds.second) {
			bullet->pos = bullet->startFixedPos;
		}
	}
	else if (offSide == RIGHT) {
		if (bullet->pos.x <= bullet->bulletLimitBounds.first) {
			bullet->pos = bullet->startFixedPos;
		}
	}
}

// Initialize bullet position
void EnemyBasic3::CheckStartingPos() {
	if (offSide == LEFT) {
		bullet->pos.x = offset.x + (scale.x / 2.0f);
		bullet->pos.y = offset.y;
	}
	else if (offSide == RIGHT) {
		bullet->pos.x = offset.x - (scale.x / 2.0f);
		bullet->pos.y = offset.y;
	}
}

// Setting of how far the bullet can travel off to from the enemy
void EnemyBasic3::SetBulletLimits() {
	bullet->bulletLimitBounds.first = bullet->startFixedPos.x - 100.0f;
	bullet->bulletLimitBounds.second = bullet->startFixedPos.x + 100.0f;
}

// Scales the bullet proportionally to the enemy size
void EnemyBasic3::SetBulletScaling() {
	bullet->bulletScale.x = GetScale().x / 3.0f;
	bullet->bulletScale.y = GetScale().y / 3.0f;
}

// Shoots out the bullets based on which side is initialized
void EnemyBasic3::MoveBullet() {
	if (offSide == LEFT) {
		bullet->pos.x += bullet->bulletSpeed * deltaTime;
	}
	else if (offSide == RIGHT) {
		bullet->pos.x -= bullet->bulletSpeed * deltaTime;
	}
}

// Updates the bounding box of the enemy
void EnemyBasic3::UpdateBulletBoundingBox() {
	bullet->bulletAABB.min.x = -0.5f * bullet->bulletScale.x + bullet->pos.x;
	bullet->bulletAABB.min.y = -0.5f * bullet->bulletScale.y + bullet->pos.y;
	bullet->bulletAABB.max.x = +0.5f * bullet->bulletScale.x + bullet->pos.x;
	bullet->bulletAABB.max.y = +0.5f * bullet->bulletScale.y + bullet->pos.y;
}

// Drawing of bullet into the game world
void EnemyBasic3::DrawBullet() {
	AEMtx33 bulletTransform;
	AEMtx33Identity(&bulletTransform);

	AEMtx33 bulletScale;
	AEMtx33Scale(&bulletScale, bullet->bulletScale.x, bullet->bulletScale.y);

	AEMtx33 bulletRotation{ 0.0f };
	AEMtx33Identity(&bulletRotation);
	AEMtx33Rot(&bulletRotation, 0.0f);

	AEMtx33 bulletTranslate;

	AEMtx33Trans(&bulletTranslate, bullet->pos.x - camera_x, bullet->pos.y - camera_y);

	AEMtx33Concat(&bulletTransform, &bulletRotation, &bulletScale);
	AEMtx33Concat(&bulletTransform, &bulletTranslate, &bulletTransform);

	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);

	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);

	AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);

	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(1.0f);

	if (offSide == LEFT) {
		AEGfxTextureSet(eggLeftText, 0.0f, 0.0f);
	}
	else if (offSide == RIGHT) {
		AEGfxTextureSet(eggRightText, 0.0f, 0.0f);
	}

	AEGfxSetTransform(bulletTransform.m);

	AEGfxMeshDraw(bullet->bulletMesh, AE_GFX_MDM_TRIANGLES);
}

void EnemyBasic3::PrintBulletStats() {
	std::cout << "Bullet stats:\n"
		<< "Bullet pos: (x: " << bullet->pos.x << ", y: " << bullet->pos.y << ")\n"
		<< "Bullet startFixedPos: (x: " << bullet->startFixedPos.x << ", y: " << bullet->startFixedPos.y << ")\n"
		<< "Bullet bulletScale: (x: " << bullet->bulletScale.x << ", y: " << bullet->bulletScale.y << ")\n"
		<< "Bullet bulletLimitBounds: (first: " << bullet->bulletLimitBounds.first << ", second: " << bullet->bulletLimitBounds.second << ")\n"
		<< "Bullet speed: " << bullet->bulletSpeed << "\n";
}
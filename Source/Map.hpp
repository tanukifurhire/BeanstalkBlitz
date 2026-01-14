/* Start Header ************************************************************************/
/*!
\file		Map.hpp
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

#pragma once
#include "Object2D.hpp"

enum EnemyType {
	ENEMY_NONE,
	ENEMY_TYPE_1,
	ENEMY_TYPE_2,
	ENEMY_TYPE_3,
	ENEMY_POWERUP
};

enum PlatformType {
	PLATFORM_NORMAL,
	PLATFORM_SPIKE
};

class Map : public Object2D {
public:
	struct MapObject {
		int x;
		int y;
		AEVec2 s;

		EnemyType enemy_type;

		PlatformType platform_type;

		MapObject(int xCoord, int yCoord, AEVec2 scale, EnemyType enemy_type_val = ENEMY_NONE, PlatformType platform_type_val = PLATFORM_NORMAL ): x(xCoord), y(yCoord), s(scale), enemy_type(enemy_type_val), platform_type(platform_type_val) {}
	};

    void Enter() override;

    void PreUpdate() override;

    void Update() override;

    void PostUpdate() override;

    void Exit() override;

	std::vector<MapObject> CreateSeed(int xMin, int xMax, int yMin, int yMax,int minStep, int maxStep, int numObjects, float objectSizeX, float objectSizeY);

	std::vector<MapObject> CreateSeed(int xMin, int xMax, int yMin, int yMax, int minStep, int maxStep, int numObjects, float objectSizeX, float objectSizeY, unsigned int seed);

	void CreateMap(const std::vector<MapObject>& map);

	std::vector<MapObject> mapObj;

protected:
	int _xMin{};
	int _xMax{};
	int _numObjects{};
	int _yMin{};
	int _yMax{};
	int _minStep = 250;
	int _maxStep = 300;
};
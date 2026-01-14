/* Start Header ************************************************************************/
/*!
\file		AIPlayer.hpp
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
#include "Player.hpp"
#include "Map.hpp"
#include "InputManager.hpp"

class PlayerMovementStateMachine;

class AIPlayer : public Player
{
public :
	AIPlayer(std::shared_ptr<Object> parent = nullptr);

    void AIMap(std::vector<Map::MapObject> map);

    void Enter() override;

    void PreUpdate() override;

    void Update() override;

    void PostUpdate() override;

    void Exit() override;

    std::vector<Map::MapObject> mapObject;
    int nextPlatform{};
    Map::MapObject curr{ 0,0,{0,0} };
    Map::MapObject next{ 0,0,{0,0} };

protected:

    std::shared_ptr<InputManager> input;

    void AIProcessMovement();
};
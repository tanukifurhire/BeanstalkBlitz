
/* Start Header ************************************************************************/
/*!
\file		PlayerManager.hpp
\author		Tai Xiao Xuan
\par		xiaoxuan.tai@digipen.edu
\date		09/03/2025
\brief		This file implements a PlayerManager class, that handles interfacing with the NetworkClient to spawn server players.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#pragma once
#include "Object2D.hpp"

class PlayerManager : public Object2D {
public:
	PlayerManager(std::shared_ptr<Object> parent = nullptr) : Object2D(std::move(parent)) {}

	void Update() override;

	bool on_player_added = false;


};
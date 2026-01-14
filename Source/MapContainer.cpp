/* Start Header ************************************************************************/
/*!
\file		MapContainer.cpp
\author		Tai Xiao Xuan
\par		xiaoxuan.tai@digipen.edu
\date		09/03/2025
\brief		This file implements a separate class that manages the Map spawning in Multiplayer Mode.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#include "GameDefn.hpp"

void MapContainer::Update() {
	if (on_create_map) {
		std::srand(GetNetworkClient()->new_seed);

		for (auto& child : children) {
			std::shared_ptr<Map> map = std::dynamic_pointer_cast<Map>(child);

			if (map) {
				map->CreateMap(map->mapObj);
			}
		}

		on_create_map = false;
	}

	Object2D::Update();
}
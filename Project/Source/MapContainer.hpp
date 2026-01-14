/* Start Header ************************************************************************/
/*!
\file		MapContainer.hpp
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

#pragma once
#include "Object2D.hpp"

class MapContainer : public Object2D {
public:
	MapContainer(std::shared_ptr<Object> parent = nullptr) : Object2D(std::move(parent)) {}

	bool on_create_map = false;

	void Update() override;

protected:

};
/* Start Header ************************************************************************/
/*!
\file		UIContainer.cpp
\author		Tai Xiao Xuan
\par		xiaoxuan.tai@digipen.edu
\date		09/03/2025
\brief		This file implements a base UIContainer class, that allows UI objects to be moved around as a whole, 
			and lerp from one position to another easily.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#include "GameDefn.hpp"
#include "Utils.hpp"

void UIContainer::Update() {
	f32 offset_y = offset.y;

	if (!AEIsF32Equal(offset.y, target)) {
		
		offset.y = lerp(offset.y, target, scroll_speed * deltaTime);
	}

	if (!GetCamera()->is_player_parent) {
		GetBrickUILeft()->SetTexScrollSpeed(0.0f, 0.2f * (offset.y - offset_y));
		GetBrickUIRight()->SetTexScrollSpeed(0.0f, 0.2f * (offset.y - offset_y));
	}

	Object2D::Update();
}
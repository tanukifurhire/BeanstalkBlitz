/* Start Header ************************************************************************/
/*!
\file		UIContainer.hpp
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

#pragma once
#include "Object2D.hpp"

class UIContainer : public Object2D {
public:
	UIContainer(std::shared_ptr<Object> parent = nullptr) : Object2D(std::move(parent)) {}

	void Update() override;

	void MoveTo(f32 target_val) {
		std::cout << name << " Moving to " << target_val << std::endl;
		target = target_val;
	}

	void SetScrollSpeed(f32 speed) {
		scroll_speed = speed;
	}

protected:
	f32 target{};
	f32 scroll_speed{};
};
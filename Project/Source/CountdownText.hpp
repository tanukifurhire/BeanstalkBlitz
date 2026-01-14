/* Start Header ************************************************************************/
/*!
\file		CountdownText.hpp
\author		Tai Xiao Xuan
\par		xiaoxuan.tai@digipen.edu
\date		30/03/2025
\brief		This file declares a countdown text UI class

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#pragma once
#include "Text2D.hpp"

class CountdownText : public Text2D {
public:
	CountdownText(std::shared_ptr<Object> parent = nullptr) : Text2D(std::move(parent)) {}

	void Update() override;

	void SetCountdown(f32 count) {
		countdown = count;
	}

	void StartCountdown() {
		start = true;
	}

protected:
	f32 countdown{0.f};
	bool start{ false };
};
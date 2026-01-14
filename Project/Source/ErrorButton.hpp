/* Start Header ************************************************************************/
/*!
\file		ErrorButton.hpp
\author		Tai Xiao Xuan
\par		xiaoxuan.tai@digipen.edu
\date		09/03/2025
\brief		This file Implements functions for an Error Button that masks all screen UI objects,
			useful for when the Client is attempting to communicate with the server.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#pragma once
#include "Button.hpp"

class ErrorButton : public Button {
public:
	ErrorButton(std::shared_ptr<Object> parent = nullptr) : Button(std::move(parent)) {}

	void Update() override;

	void PostUpdate() override;

	void ResetErrorTimer() {
		error_timer = error_timer_max;
	}

	void SetErrorText(std::string text) {
		error_text = text;
	}

protected:
	f32 error_timer = 10.0f;
	const f32 error_timer_max = 10.0f;

	f32 load_anim_timer = 2.0f;
	const f32 load_anim_timer_max = 2.0f;

	std::string error_text = "Connecting";
};
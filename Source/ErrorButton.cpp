/* Start Header ************************************************************************/
/*!
\file		ErrorButton.cpp
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

#include "GameDefn.hpp"

void ErrorButton::Update() {

	if (current_layer == 2) {

		text_field = error_text;

		if (load_anim_timer <= 0.0f) {
			load_anim_timer = load_anim_timer_max;
		}

		if (load_anim_timer <= 0.5f) {
			text_field += ".";
		}

		if (load_anim_timer <= 1.0f) {
			text_field += ".";
		}

		if (load_anim_timer <= 1.5f) {
			text_field += ".";
		}

		load_anim_timer -= deltaTime;

		transparency = 1.0f;

		error_timer -= deltaTime;

		if (error_timer <= 0.0f) {
			text_field = "Connection Error!";
			current_layer = 1;
			error_timer = error_timer_max;
			load_anim_timer = load_anim_timer_max;
		}
	}

	Button::Update();
}

void ErrorButton::PostUpdate() {

	Button::PostUpdate();

	f32 print_size_x{}, print_size_y{};

	AEGfxGetPrintSize(pFont, "CLICK ANYWHERE TO RETURN TO MENU", text_scale / 2.f, &print_size_x, &print_size_y);

	AEGfxPrint(pFont, "CLICK ANYWHERE TO RETURN TO MENU", 0.f * screen_width / (screen_width / 2.0f) - (print_size_x / 2.f), -0.35f * screen_height / (screen_height / 2.0f) - (print_size_y / 2.f), text_scale / 2.f, 1.0f, 1.0f, 1.0f, transparency);
}
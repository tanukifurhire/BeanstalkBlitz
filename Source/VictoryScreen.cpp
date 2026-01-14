/* Start Header ************************************************************************/
/*!
\file	    VictoryScreen.cpp
\author		Tai Xiao Xuan
\par		xiaoxuan.tai@digipen.edu
\date		30/03/2025
\brief		This file implements a Victory Screen UI class that shows which player won.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#include "GameDefn.hpp"

void VictoryScreen::Update() {
	if (current_layer == 3) {
		//SetTransparency(1.f);

		std::stringstream ss;

		text_field = error_text + " Win!";
	}

	Button::Update();
}

void VictoryScreen::PostUpdate() {

	Button::PostUpdate();

	f32 print_size_x{}, print_size_y{};

	AEGfxGetPrintSize(pFont, "CLICK ANYWHERE TO RETURN TO MENU", text_scale / 2.f, &print_size_x, &print_size_y);

	AEGfxPrint(pFont, "CLICK ANYWHERE TO RETURN TO MENU", 0.f * screen_width / (screen_width / 2.0f) - (print_size_x / 2.f), -0.35f * screen_height / (screen_height / 2.0f) - (print_size_y / 2.f), text_scale / 2.f, 1.0f, 1.0f, 1.0f, transparency);
}
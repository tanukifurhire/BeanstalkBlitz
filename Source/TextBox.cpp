/* Start Header ************************************************************************/
/*!
\file		TextBox.cpp
\author		Tai Xiao Xuan
\par		xiaoxuan.tai@digipen.edu
\date		09/03/2025
\brief		This file implements a base TextBox class, that allows for editable text fields at runtime.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#include "GameDefn.hpp"

void TextBox::Update() {
	AEVec2 cursor_pos{};

	cursor_pos.x = static_cast<f32>(cursor_pos_world_x);
	cursor_pos.y = static_cast<f32>(cursor_pos_world_y);

	if (current_layer == layer && AECalcDistPointToRect(&cursor_pos, &position, local_scale.x, local_scale.y) <= 0.0f) {
		if (AEInputCheckTriggered(AEVK_LBUTTON)) {
			button_state = BUTTON_PRESSED;
			OnPressed.Emit(nullptr);
		}
	}
	else if (AECalcDistPointToRect(&cursor_pos, &position, local_scale.x, local_scale.y) > 0.0f) {
		if (AEInputCheckTriggered(AEVK_LBUTTON)) {
			button_state = BUTTON_NONE;

			if (text_field.empty()) {
				text_field = default_text;
			}
		}
	}

	if (button_state == BUTTON_PRESSED) {
		for (u8 key = AEVK_0; key <= AEVK_Z; key++) {
			if (AEInputCheckTriggered(key)) {
				if (text_field.length() <= 12) {
					text_field += static_cast<char>(key);
				}
			}
		}

		if (AEInputCheckTriggered(AEVK_BACK)) {
			if (text_field.length() > 0) {
				text_field.pop_back();
			}
		}

		if (AEInputCheckTriggered(AEVK_RETURN)) {
			button_state = BUTTON_NONE;
			if (text_field.empty()) {
				text_field = default_text;
			}
		}
	}

	Rect2D::Update();
}

void TextBox::PostUpdate() {
	AEMtx33 scale_matrix{ 0.0f };
	AEMtx33Identity(&scale_matrix);
	AEMtx33Scale(&scale_matrix, local_scale.x, local_scale.y);

	AEMtx33 rotation_matrix{ 0.0f };
	AEMtx33Identity(&rotation_matrix);
	AEMtx33Rot(&rotation_matrix, rotation * (PI / 180.0f));

	AEMtx33 translate{ 0.0f };
	AEMtx33Identity(&translate);
	AEMtx33Trans(&translate, position.x, position.y);

	AEMtx33 transform = { 0 };
	AEMtx33Concat(&transform, &rotation_matrix, &scale_matrix);
	AEMtx33Concat(&transform, &translate, &transform);

	AEGfxSetRenderMode(AE_GFX_RM_COLOR);

	switch (button_state) {
	case BUTTON_NONE:
		AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
		break;
	case BUTTON_HOVERED:
		AEGfxSetColorToMultiply(0.8f, 0.8f, 0.8f, 1.0f);
		break;
	case BUTTON_PRESSED:
		AEGfxSetColorToMultiply(0.6f, 0.6f, 0.6f, 1.0f);
		break;
	}

	AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);

	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(transparency);

	AEGfxSetTransform(transform.m);

	AEGfxMeshDraw(aeMesh, AE_GFX_MDM_TRIANGLES);

	f32 print_size_x{}, print_size_y{};

	AEGfxGetPrintSize(pFont, text_field.c_str(), text_scale, &print_size_x, &print_size_y);

	AEGfxPrint(pFont, text_field.c_str(), (position.x - (scale.x / 2.2f)) / (screen_width / 2.0f) , position.y / (screen_height / 2.0f) - (print_size_y / 2), text_scale, 1.0f, 1.0f, 1.0f, transparency);

	Object2D::PostUpdate();
}
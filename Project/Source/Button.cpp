/* Start Header ************************************************************************/
/*!
\file		Button.cpp
\author		Tai Xiao Xuan
\par		xiaoxuan.tai@digipen.edu
\date		09/03/2025
\brief		This file Implements functions for a Basic Text Button/Image Button.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#include "GameDefn.hpp"

void Button::Enter() {
    pFont = AEGfxCreateFont(font_file.c_str(), font_size);

    Rect2D::Enter();
}

void Button::Update() {
	AEVec2 cursor_pos{};

	cursor_pos.x = static_cast<f32>(cursor_pos_world_x);
	cursor_pos.y = static_cast<f32>(cursor_pos_world_y);

	if (current_layer == layer && AECalcDistPointToRect(&cursor_pos, &position, local_scale.x, local_scale.y) <= 0.0f) {
		if (AEInputCheckTriggered(AEVK_LBUTTON)) {
			button_state = BUTTON_PRESSED;
            OnPressed.Emit(nullptr);
		}
		else {
			button_state = BUTTON_HOVERED;
		}
	}
    else {
        button_state = BUTTON_NONE;
    }

	Rect2D::Update();
}

void Button::PostUpdate() {
    AEMtx33 scale_matrix{ 0.0f };
    AEMtx33Identity(&scale_matrix);
    switch (button_state) {
    case BUTTON_NONE:
        AEMtx33Scale(&scale_matrix, local_scale.x, local_scale.y);
        break;
    case BUTTON_HOVERED:
        AEMtx33Scale(&scale_matrix, local_scale.x * 1.05f, local_scale.y * 1.05f);
        break;
    case BUTTON_PRESSED:
        AEMtx33Scale(&scale_matrix, local_scale.x, local_scale.y);
        break;
    }

    AEMtx33 rotation_matrix{ 0.0f };
    AEMtx33Identity(&rotation_matrix);
    AEMtx33Rot(&rotation_matrix, rotation * (PI / 180.0f));

    AEMtx33 translate{ 0.0f };
    AEMtx33Identity(&translate);
    AEMtx33Trans(&translate, position.x, position.y);

    AEMtx33 transform = { 0 };
    AEMtx33Concat(&transform, &rotation_matrix, &scale_matrix);
    AEMtx33Concat(&transform, &translate, &transform);

    switch (button_type) {
    case BUTTON_RECT:
        AEGfxSetRenderMode(AE_GFX_RM_COLOR);
        break;
    case BUTTON_IMAGE:
        AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
        AEGfxTextureSet(pTex, 0.0f, 0.0f);
        break;
    }

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

    switch (button_state) {
    case BUTTON_NONE:
        AEGfxPrint(pFont, text_field.c_str(), position.x / (screen_width / 2.0f) - (print_size_x / 2), position.y / (screen_height / 2.0f) - (print_size_y / 2), text_scale, 1.0f, 1.0f, 1.0f, transparency);
        break;
    case BUTTON_HOVERED:
        AEGfxPrint(pFont, text_field.c_str(), position.x / (screen_width / 2.0f) - (print_size_x / 2), position.y / (screen_height / 2.0f) - (print_size_y / 2), text_scale * 1.05f, 1.0f, 1.0f, 1.0f, transparency);
        break;
    case BUTTON_PRESSED:
        AEGfxPrint(pFont, text_field.c_str(), position.x / (screen_width / 2.0f) - (print_size_x / 2), position.y / (screen_height / 2.0f) - (print_size_y / 2), text_scale, 1.0f, 1.0f, 1.0f, transparency);
        break;
    }

    Object2D::PostUpdate();
}

void Button::Exit() {
    if (pTex) {
        AEGfxTextureUnload(pTex);
    }

    Rect2D::Exit();
}
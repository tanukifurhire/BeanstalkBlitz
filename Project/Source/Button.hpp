/* Start Header ************************************************************************/
/*!
\file		Button.hpp
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

#pragma once
#include "Rect2D.hpp"
#include "Event.hpp"

enum ButtonState {
	BUTTON_NONE,
	BUTTON_HOVERED,
	BUTTON_PRESSED
};

enum ButtonType {
	BUTTON_RECT,
	BUTTON_IMAGE
};

class Button : public Rect2D {
public:
	Button(std::shared_ptr<Object> parent = nullptr) : Rect2D(std::move(parent)) {}

	std::string GetText() {
		return text_field;
	}

	void SetText(std::string text) {
		text_field = text;
	}

	f32 GetTextScale() {
		return text_scale;
	}

	void SetTextScale(f32 text_scale_val) {
		text_scale = text_scale_val;
	}

	void SetButtonLayer(s8 layer_val) {
		layer = layer_val;
	}

	void LoadTexture(const char* pFileName) {
		pTex = AEGfxTextureLoad(pFileName);
	}

	void SetButtonType(ButtonType button_type_val) {
		button_type = button_type_val;
	}

	void Enter() override;

	void Update() override;

	void PostUpdate() override;

	void Exit() override;

	Event OnPressed;

	s32 data{};

protected:
	AEGfxTexture* pTex = nullptr;
	std::string tex_file = "Assets/Brick_Wall.png";

	ButtonState button_state{ BUTTON_NONE };
	ButtonType button_type{ BUTTON_RECT };

	f32 text_scale{};

	std::string text_field{ "Hello World!" };
	int font_size = 50;

	std::string font_file = "Assets/SourceSans3-Bold.ttf";

	s8 pFont = 0;

	s8 layer = 0;
};
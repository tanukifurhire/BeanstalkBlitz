/* Start Header ************************************************************************/
/*!
\file		Text2D.cpp
\author		Tai Xiao Xuan
\par		xiaoxuan.tai@digipen.edu
\date		09/03/2025
\brief		This file implements a base Text Rendering class, that can print text on the screen.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#include "pch.hpp"
#include "GameDefn.hpp"
#include <iostream>

void Text2D::Enter() {
	pFont = AEGfxCreateFont(font_file.c_str(), font_size);
	
	Object2D::Enter();
}

void Text2D::Init(const rapidjson::Value& child_value) {

	if (!child_value.HasMember("text_field")) {
		std::cout << "Rect2D has no member 'text_field'!" << std::endl;
		return;
	}

	if (!child_value.HasMember("font")) {
		std::cout << "Rect2D has no member 'font'!" << std::endl;
		return;
	}

	text_field = child_value["text_field"].GetString();

	font_file = child_value["font"].GetString();
	
	Object2D::Init(child_value);
}

void Text2D::PreUpdate() {

	Object2D::PreUpdate();
}

void Text2D::Update() {

	Object2D::Update();
}

void Text2D::PostUpdate() {

	f32 print_size_x{}, print_size_y{};

	AEGfxGetPrintSize(pFont, text_field.c_str(), scale.x, &print_size_x, &print_size_y);

	switch (space_type) {
	case SCREEN_SPACE:
		AEGfxPrint(pFont, text_field.c_str(), position.x / (static_cast<f32>(AEGfxGetWindowWidth() / 2.0f)) - print_size_x / 2.0f, position.y / (static_cast<f32>(AEGfxGetWindowHeight()) / 2.0f) - print_size_y / 2.0f, scale.y, red, green, blue, transparency);
		break;
	case WORLD_SPACE:
		AEGfxPrint(pFont, text_field.c_str(), (position.x - camera_x) / (static_cast<f32>(AEGfxGetWindowWidth() / 2.0f)) - print_size_x / 2.0f, (position.y - camera_y) / (static_cast<f32>(AEGfxGetWindowHeight()) / 2.0f) - print_size_y / 2.0f, scale.y, red, green, blue, transparency);
		break;
	}

	Object2D::PostUpdate();
}

void Text2D::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const {
	writer.StartObject();
	writer.Key("type");
	writer.Int(TEXT2D);

	writer.Key("name");
	writer.String(name.c_str());

	writer.Key("offset");
	writer.StartObject();
	writer.Key("x");
	writer.Double(offset.x);
	writer.Key("y");
	writer.Double(offset.y);
	writer.EndObject();

	writer.Key("scale");
	writer.StartObject();
	writer.Key("x");
	writer.Double(local_scale.x);
	writer.Key("y");
	writer.Double(local_scale.y);
	writer.EndObject();

	writer.Key("rotation");
	writer.Double(local_rotation);

	writer.Key("text_field");
	writer.String(text_field.c_str());

	writer.Key("font");
	writer.String(font_file.c_str());

	writer.Key("children");
	writer.StartArray();

	for (const auto& child : children) {
		child->Serialize(writer);
	}

	writer.EndArray();
	writer.EndObject();
}

void Text2D::Exit() {
	AEGfxDestroyFont(pFont);

	Object2D::Exit();
}


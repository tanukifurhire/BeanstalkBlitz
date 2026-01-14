/* Start Header ************************************************************************/
/*!
\file		Object.cpp
\author		Tai Xiao Xuan
\par		xiaoxuan.tai@digipen.edu
\date		09/03/2025
\brief		This file implements the Base Object Functions that all Scene Members must inherit from.
			Includes SetParent, Init, Serialize, Deserialize, Enter, PreUpdate, Update, PostUpdate, InspectName and Exit.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#include "pch.hpp"
#include "GameDefn.hpp"

Object::Object(std::shared_ptr<Object> parent) : parent(std::move(parent)) {}

void Object::SetParent(const std::shared_ptr<Object>& new_parent) {
	const std::shared_ptr<Object>& old_parent = parent.lock();

	if (old_parent == new_parent) {
		std::cout << "Same Parent!" << std::endl;
		return;
	}

	if (new_parent == shared_from_this()) {
		std::cout << "Cannot parent to itself!" << std::endl;
		return;
	}

	std::cout << "Change Parent!";

	if (new_parent) {
		for (std::shared_ptr<Object> parent_object = new_parent->parent.lock(); parent_object != 0; parent_object = parent_object->GetParent()) {
			if (parent_object == shared_from_this()) {
				std::cout << "Attempted to parent to a child!" << std::endl;
				return;
			}
		}

		new_parent->AddChild(shared_from_this());
	}
	else {
		std::cout << "No new parent!" << std::endl;
	}

	if (old_parent) {
		old_parent->RemoveChild(shared_from_this());
	}
}

void Object::Init(const rapidjson::Value& child_value) {
	if (!child_value.HasMember("name")) {
		std::cout << "Object has no member 'name'!" << std::endl;
		return;
	}

	if (!child_value.HasMember("children")) {
		std::cout << "Object has no member 'children'!" << std::endl;
		return;
	}

	name = child_value["name"].GetString();

	for (const auto& child : child_value["children"].GetArray()) {

		switch (child["type"].GetInt()) {
			case OBJECT:
			{
				std::cout << "Creating an Object!" << std::endl;
				std::shared_ptr<Object> object = std::make_shared<Object>();
				object->Init(child);
				AddChild(object);
				break;
			}
			case OBJECT2D:
			{
				std::cout << "Creating an Object2D!" << std::endl;
				std::shared_ptr<Object2D> object = std::make_shared<Object2D>();
				object->Init(child);
				AddChild(object);
				break;
			}
			case EDITOR2D:
			{
				std::cout << "Creating an Editor2D!" << std::endl;
				std::shared_ptr<Editor2D> object = std::make_shared<Editor2D>();
				object->Init(child);
				AddChild(object);
				break;
			}
			case RECT2D:
			{
				std::cout << "Creating a Rect2D!" << std::endl;
				std::shared_ptr<Rect2D> object = std::make_shared<Rect2D>();
				object->Init(child);
				AddChild(object);
				break;
			}
			case RECTPLAYER:
			{
				std::cout << "Creating a Rect2DPlayer!" << std::endl;
				std::shared_ptr<RectPlayer> object = std::make_shared<RectPlayer>();
				object->Init(child);
				AddChild(object);
				break;
			}
			case CAMERA2D:
			{
				std::cout << "Creating a Camera2D!" << std::endl;
				std::shared_ptr<Camera2D> object = std::make_shared<Camera2D>();
				object->Init(child);
				AddChild(object);
				break;
			}
			case TEXT2D:
			{
				std::cout << "Creating a Text2D!" << std::endl;
				std::shared_ptr<Text2D> object = std::make_shared<Text2D>();
				object->Init(child);
				AddChild(object);
				break;
			}
			case IMAGERENDERER2D:
			{
				std::cout << "Creating an ImageRenderer2D!" << std::endl;
				std::shared_ptr<ImageRenderer2D> object = std::make_shared<ImageRenderer2D>();
				object->Init(child);
				AddChild(object);
				break;
			}
		}
	}
}

void Object::Enter() {
	already_initialised = true;
}

void Object::PreUpdate() {
	auto children_copy = children;

	for (const auto& child : children_copy) {
		child->PreUpdate();
	}
}

void Object::Update() {

	auto children_copy = children;

	for (const auto& child : children_copy) {
		child->Update();
	}
}

void Object::PostUpdate() {
	AEGfxSetBackgroundColor(0.6f, 0.6f, 0.6f);

	auto children_copy = children;

	for (const auto& child : children_copy) {
		child->PostUpdate();
	}
}

void Object::EditorUpdate(f32 x_pos, f32& y_pos, f32 x_scale, f32 y_scale) {
	bool hovered = false;
	bool pressed = false;

	f32 y_position = y_pos;

	AEVec2 cursor_pos{ static_cast<f32>(cursor_pos_world_x), static_cast<f32>(cursor_pos_world_y) };
	AEVec2 rect_pos{ x_pos + x_scale / 2.0f, y_pos + y_scale / 4.0f };

	f32 dist_to_rect = AECalcDistPointToRect(&cursor_pos, &rect_pos, x_scale, y_scale);

	if (dist_to_rect <= 0.0f) {
		if (AEInputCheckTriggered(AEVK_LBUTTON)) {
			if (parent.lock() != GetRoot()) {
				editor_picked_object = shared_from_this();
			}
			editor_inspected_object = shared_from_this();
		}
		else {
			if (editor_picked_object.lock() != shared_from_this()) {
				editor_hovered_object = shared_from_this();
				hovered = true;
			}
		}
	}

	if (editor_picked_object.lock() == shared_from_this()) {
		pressed = true;
	
		if (AEInputCheckReleased(AEVK_LBUTTON)) {
			if (editor_hovered_object.lock()) {
				if (parent.lock() != GetRoot()) {
					SetParent(editor_hovered_object.lock());
				}
				else {
					std::cout << "Parent is Root!" << std::endl;
				}
			}
			else {
				std::cout << "Hovered Object doesn't exist!" << std::endl;
			}

			editor_picked_object.reset();
		}
	}

	if (editor_inspected_object.lock() == shared_from_this()) {
		EditorInspectorUpdate();
		is_inspected_object_updated = false;
	}

	if (editor_picked_object.lock() == shared_from_this()) {
		y_position = cursor_pos.y - y_scale / 4;
	}

	f32 transparency = 0.7f;

	if (hovered) {
		transparency = 1.0f;
	}
	if (pressed) {
		transparency = 0.3f;
	}

	DrawRect(x_pos, y_position - y_scale / 4, x_scale, y_scale, 0.0f, default_mesh, 0.2f, 0.2f, 0.2f, transparency, 1.0f);
	
	f32 width, height;
	AEGfxGetPrintSize(default_font, name.c_str(), 0.4f, &width, &height);
	AEGfxPrint(default_font, name.c_str(), x_pos / (screen_width / 2.0f), y_position / (screen_height / 2.0f), 0.4f, 1.0f, 1.0f, 1.0f, transparency);

	auto children_copy = children;

	f32 new_pos_x = x_pos + (0.0125f * screen_width);
	f32 new_scale_x = x_scale - (0.0125f * screen_width);

	for (const auto& child : children_copy) {
		y_pos -= (0.03f * screen_height);
		child->EditorUpdate(new_pos_x, y_pos, new_scale_x, y_scale);
	}
}

f32 Object::EditorInspectorUpdate() {
	f32 y_pos = (0.35f * screen_height);

	InspectName((0.275f * screen_width), y_pos, (0.1875f * screen_width), (0.027f * screen_height));

	return y_pos;
}

// { "name": name , "chilren": [ { ... }, { ... } ] }
void Object::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const {
	writer.StartObject();
	writer.Key("type");
	writer.Int(OBJECT);

	writer.Key("name");
	writer.String(name.c_str());

	writer.Key("children");
	writer.StartArray();

	for (const auto& child : children) {
		child->Serialize(writer);
	}

	writer.EndArray();
	writer.EndObject();
}

void Object::InspectName(f32 x_pos, f32& y_pos, f32 x_scale, f32 y_scale) {

	AEVec2 cursor_pos{ static_cast<f32>(cursor_pos_world_x), static_cast<f32>(cursor_pos_world_y) };

	AEVec2 rect_pos{ x_pos + x_scale / 2.0f, y_pos + y_scale / 4.0f };

	f32 transparency = 0.5f;

	bool hovered = false;

	f32 dist_to_rect = AECalcDistPointToRect(&cursor_pos, &rect_pos, x_scale, y_scale);

	if (dist_to_rect <= 0.0f) {
		if (AEInputCheckTriggered(AEVK_LBUTTON)) {
			inspected_object = NAME;
			is_inspected_object_updated = true;
			inspected_field = name;
		}
		else {
			hovered = true;
		}
	}

	if (hovered) {
		transparency = 0.7f;
	}
	
	if (inspected_object == NAME) {
		transparency = 1.0f;
		DrawRect(x_pos, y_pos - y_scale / 4, x_scale, y_scale, 0.0f, default_mesh, 0.2f, 0.2f, 0.2f, transparency, 1.0f);

		for (u8 key = AEVK_0; key <= AEVK_Z; key++) {
			if (AEInputCheckTriggered(key)) {
				inspected_field += static_cast<char>(key);
			}
		}

		if (AEInputCheckTriggered(AEVK_BACK)) {
			if (inspected_field.length() > 0) {
				inspected_field.pop_back();
			}
		}

		AEGfxPrint(default_font, inspected_field.c_str(), x_pos / (screen_width / 2.0f), y_pos / (screen_height / 2.0f), 0.4f, 1.0f, 1.0f, 1.0f, 1.0f);

		if (AEInputCheckTriggered(AEVK_RETURN)) {
			if (is_inspected_object_updated == false) {
				inspected_object = NONE;
			}
			name = inspected_field;
			inspected_field = "";
		}

		if (dist_to_rect > 0.0f && AEInputCheckTriggered(AEVK_LBUTTON)) {
			if (is_inspected_object_updated == false) {
				inspected_object = NONE;
			}
			name = inspected_field;
			inspected_field = "";
		}
	}
	else {
		DrawRect(x_pos, y_pos - y_scale / 4, x_scale, y_scale, 0.0f, default_mesh, 0.2f, 0.2f, 0.2f, transparency, 1.0f);
		AEGfxPrint(default_font, name.c_str(), x_pos / (screen_width / 2.0f), y_pos / (screen_height / 2.0f), 0.4f, 1.0f, 1.0f, 1.0f, 1.0f);
	}

	y_pos -= (0.03f * screen_height);
}

void Object::Exit() {
	
	auto children_copy = children;

	for (const auto& child : children_copy) {
		child->Exit();
	}
}

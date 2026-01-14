/* Start Header ************************************************************************/
/*!
\file		Object2D.cpp
\author		Tai Xiao Xuan
\par		xiaoxuan.tai@digipen.edu
\date		09/03/2025
\brief		This file Implements the Base 2D Object Functions that all Scene Members must inherit from.
			New functions include matrix transformation concatenation with all parents in Update(), 
			EditorUpdate and EditorInspectorUpdate as well as InspectFloat function, used for Editor Mode.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#include "pch.hpp"
#include "GameDefn.hpp"
#include <iostream>

Object2D::Object2D(std::shared_ptr<Object> parent) : Object(std::move(parent)) {}

void Object2D::SetParent(const std::shared_ptr<Object>& new_parent) {
	if (new_parent == parent.lock()) {
		return;
	}
	Object::SetParent(new_parent);
}

void Object2D::Enter() {

    Object::Enter();
}

void Object2D::Init(const rapidjson::Value& child_value) {

	if (!child_value.HasMember("offset")) {
		std::cout << "Object2D has no member 'offset'!" << std::endl;
		return;
	}

	if (!child_value.HasMember("scale")) {
		std::cout << "Object2D has no member 'scale'!" << std::endl;
		return;
	}

	if (!child_value.HasMember("rotation")) {
		std::cout << "Object2D has no member 'rotation'!" << std::endl;
		return;
	}

	offset.x = child_value["offset"].HasMember("x") ? static_cast<f32>(child_value["offset"]["x"].GetDouble()) : 0.0f;
	offset.y = child_value["offset"].HasMember("y") ? static_cast<f32>(child_value["offset"]["y"].GetDouble()) : 0.0f;
	
	local_scale.x = child_value["scale"].HasMember("x") ? static_cast<f32>(child_value["scale"]["x"].GetDouble()) : 0.0f;
	local_scale.y = child_value["scale"].HasMember("y") ? static_cast<f32>(child_value["scale"]["y"].GetDouble()) : 0.0f;
	
	local_rotation = static_cast<f32>(child_value["rotation"].GetDouble());

	Object::Init(child_value);
}

void Object2D::PreUpdate() {

	bounding_box.min.x = position.x - local_scale.x / 2;
	bounding_box.min.y = position.y - local_scale.y / 2;
	bounding_box.max.x = position.x + local_scale.x / 2;
	bounding_box.max.y = position.y + local_scale.y / 2;

	velocity.x = 0.0f;
	velocity.y = 0.0f;

    auto children_copy = children;

    for (const auto& child : children_copy) {
        child->PreUpdate();
    }
}

void Object2D::Update() {

    AEMtx33 parent_transform{ 0.0f };
    AEMtx33Identity(&parent_transform);

    AEVec2 intermediate_position{ 0.0f, 0.0f };
    AEMtx33MultVec(&intermediate_position, &parent_transform, &offset);

    f32 total_parent_rotation = 0.0f;
    
    for (std::shared_ptr<Object2D> parent_object2D = std::dynamic_pointer_cast<Object2D>(parent.lock()); parent_object2D != 0; parent_object2D = std::dynamic_pointer_cast<Object2D>(parent_object2D->GetParent())) {

        AEMtx33 current_parent_transform{ 0.0f };

        AEMtx33 translation_matrix{ 0.0f };
        AEMtx33Identity(&translation_matrix);
        AEMtx33Trans(&translation_matrix, parent_object2D->GetLocalPosition().x, parent_object2D->GetLocalPosition().y);

        AEMtx33 rotation_matrix{ 0.0f };
        AEMtx33Identity(&rotation_matrix);
        AEMtx33Rot(&rotation_matrix, parent_object2D->GetRotation() * (PI / 180.0f));

        AEMtx33Concat(&current_parent_transform, &rotation_matrix, &translation_matrix);
        AEMtx33Concat(&parent_transform, &current_parent_transform, &parent_transform);

        total_parent_rotation += parent_object2D->GetRotation();
    }

    AEMtx33MultVec(&intermediate_position, &parent_transform, &intermediate_position);

    position = intermediate_position;

    rotation = total_parent_rotation + local_rotation;

    scale = local_scale;

    auto children_copy = children;

	for (const auto& child : children_copy) {
		child->Update();
	}
}

void Object2D::PostUpdate() {

    auto children_copy = children;

    for (const auto& child : children_copy) {
        child->PostUpdate();
    }
}

f32 Object2D::EditorInspectorUpdate() {

    f32 y_pos = Object::EditorInspectorUpdate();

    InspectFloat(offset.x, POSITION_X, "pX: ", (0.275f * screen_width), y_pos, (0.1875f * screen_width), (0.027f * screen_height));
	InspectFloat(offset.y, POSITION_Y, "pY: ", (0.275f * screen_width), y_pos, (0.1875f * screen_width), (0.027f * screen_height));
	InspectFloat(local_scale.x, SCALE_X, "sX: ", (0.275f * screen_width), y_pos, (0.1875f * screen_width), (0.027f * screen_height));
	InspectFloat(local_scale.y, SCALE_Y, "sY: ", (0.275f * screen_width), y_pos, (0.1875f * screen_width), (0.027f * screen_height));
	InspectFloat(local_rotation, ROTATION, "rot: ", (0.275f * screen_width), y_pos, (0.1875f * screen_width), (0.027f * screen_height));

	return y_pos;
}

void Object2D::EditorUpdate(f32 x_pos, f32& y_pos, f32 x_scale, f32 y_scale) {
	AEMtx33 parent_transform{ 0.0f };
	AEMtx33Identity(&parent_transform);

	AEVec2 intermediate_position{ 0.0f, 0.0f };
	AEMtx33MultVec(&intermediate_position, &parent_transform, &offset);

	f32 total_parent_rotation = 0.0f;

	for (std::shared_ptr<Object2D> parent_object2D = std::dynamic_pointer_cast<Object2D>(parent.lock()); parent_object2D != 0; parent_object2D = std::dynamic_pointer_cast<Object2D>(parent_object2D->GetParent())) {

		AEMtx33 current_parent_transform{ 0.0f };

		AEMtx33 translation_matrix{ 0.0f };
		AEMtx33Identity(&translation_matrix);
		AEMtx33Trans(&translation_matrix, parent_object2D->GetLocalPosition().x, parent_object2D->GetLocalPosition().y);

		AEMtx33 rotation_matrix{ 0.0f };
		AEMtx33Identity(&rotation_matrix);
		AEMtx33Rot(&rotation_matrix, parent_object2D->GetRotation() * (PI / 180.0f));

		AEMtx33Concat(&current_parent_transform, &rotation_matrix, &translation_matrix);
		AEMtx33Concat(&parent_transform, &current_parent_transform, &parent_transform);

		total_parent_rotation += parent_object2D->GetRotation();
	}

	AEMtx33MultVec(&intermediate_position, &parent_transform, &intermediate_position);

	position = intermediate_position;

	rotation = total_parent_rotation + local_rotation;

	scale = local_scale;

	Object::EditorUpdate(x_pos, y_pos, x_scale, y_scale);
}

void Object2D::InspectFloat(f32& value, InspectedObject type, std::string field_name, f32 x_pos, f32& y_pos, f32 x_scale, f32 y_scale) {
	AEVec2 cursor_pos{ static_cast<f32>(cursor_pos_world_x), static_cast<f32>(cursor_pos_world_y) };

	AEVec2 rect_pos{ x_pos + x_scale / 2.0f, y_pos + y_scale / 4.0f };

	f32 transparency = 0.5f;

	bool hovered = false;

	f32 dist_to_rect = AECalcDistPointToRect(&cursor_pos, &rect_pos, x_scale, y_scale);

	if (dist_to_rect <= 0.0f) {
		if (AEInputCheckTriggered(AEVK_LBUTTON)) {
			inspected_object = type;
			is_inspected_object_updated = true;
			//inspected_field = std::to_string(value);
		}
		else {
			hovered = true;
		}
	}

	if (hovered) {
		transparency = 0.7f;
	}

	if (inspected_object == type) {
		transparency = 1.0f;
		DrawRect(x_pos, y_pos - y_scale / 4, x_scale, y_scale, 0.0f, default_mesh, 0.2f, 0.2f, 0.2f, transparency, 1.0f);

		for (u8 key = AEVK_0; key <= AEVK_9; key++) {
			if (AEInputCheckTriggered(key)) {
				inspected_field += static_cast<char>(key);
			}
		}

		if (AEInputCheckTriggered(AEVK_PERIOD)) {
			if (inspected_field.find('.') == std::string::npos) {
				inspected_field += ".";
			}
		}

		if (AEInputCheckTriggered(AEVK_BACK)) {
			if (inspected_field.length() > 0) {
				inspected_field.pop_back();
			}
		}

		field_name += inspected_field;
		AEGfxPrint(default_font, field_name.c_str(), x_pos / (screen_width / 2.0f), y_pos / (screen_height / 2.0f), 0.4f, 1.0f, 1.0f, 1.0f, 1.0f);

		if (AEInputCheckTriggered(AEVK_RETURN)) {
			if (is_inspected_object_updated == false) {
				inspected_object = NONE;
			}
			if (inspected_field.length() > 0) {
				value = std::stof(inspected_field);
			}
			inspected_field = "";
		}

		if (dist_to_rect > 0.0f && AEInputCheckTriggered(AEVK_LBUTTON)) {
			if (is_inspected_object_updated == false) {
				inspected_object = NONE;
			}
			if (inspected_field.length() > 0) {
				value = std::stof(inspected_field);
			}
			inspected_field = "";
		}
	}
	else {
		DrawRect(x_pos, y_pos - y_scale / 4, x_scale, y_scale, 0.0f, default_mesh, 0.2f, 0.2f, 0.2f, transparency, 1.0f);
		field_name += std::to_string(value);
		AEGfxPrint(default_font, field_name.c_str(), x_pos / (screen_width / 2.0f), y_pos / (screen_height / 2.0f), 0.4f, 1.0f, 1.0f, 1.0f, 1.0f);
	}

	y_pos -= (0.03f * screen_height);
}

void Object2D::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const {
	writer.StartObject();
	writer.Key("type");
	writer.Int(OBJECT2D);
	
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

	writer.Key("children");
	writer.StartArray();

	for (const auto& child : children) {
		child->Serialize(writer);
	}

	writer.EndArray();
	writer.EndObject();
}

void Object2D::Exit() {

	if (children.size() > 0) {
		for (const auto& child : children) {
			child->Exit();
		}
	}
}

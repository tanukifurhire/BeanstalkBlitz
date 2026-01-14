/* Start Header ************************************************************************/
/*!
\file		Camera2D.cpp
\author		Nelson Tok Duan Jie
\par		t.duanjienelson@digipen.edu
\date		09/03/2025
\brief		This file implements the definitions of all the function to be used in Camara

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#include "pch.hpp"
#include "GameDefn.hpp"
#include "Utils.hpp"

f32 camera_x = 0.0f;
f32 camera_y = 0.0f;

Camera2D::Camera2D(std::shared_ptr<Object> parent) : Object2D(std::move(parent)) {}

void Camera2D::Enter() {
	

	Object2D::Enter();
}

void Camera2D::Init(const rapidjson::Value& child_value) {

    if (!child_value.HasMember("camera_speed")) {
        std::cout << "Rect2D has no member 'camera_speed'!" << std::endl;
        return;
    }

    if (!child_value.HasMember("min_clamp")) {
        std::cout << "Rect2D has no member 'min_clamp'!" << std::endl;
        return;
    }

    if (!child_value.HasMember("max_clamp")) {
        std::cout << "Rect2D has no member 'max_clamp'!" << std::endl;
        return;
    }

    if (!child_value.HasMember("lerp_factor")) {
        std::cout << "Rect2D has no member 'lerp_factor'!" << std::endl;
        return;
    }

    camera_speed = child_value["camera_speed"].GetFloat();

    min_clamp = child_value["min_clamp"].GetFloat();

    max_clamp = child_value["max_clamp"].GetFloat();

    lerpFactor = child_value["lerp_factor"].GetFloat();

    Object2D::Init(child_value);
}

void Camera2D::PreUpdate() {
	
	Object2D::PreUpdate();
}

void Camera2D::Update() {
    std::shared_ptr<Player> parent_object2D = std::dynamic_pointer_cast<Player>(parent.lock());

    if (parent_object2D) {
        is_player_parent = true;

        //position.x = lerp(position.x, parent_object2D->GetPosition().x, lerpFactor * deltaTime);
        position.y = lerp(position.y, parent_object2D->GetPosition().y + offset.y, lerpFactor * deltaTime);
        
        if (cursor_pos_world_y >= 350) {
            offset.y += camera_speed * deltaTime;
        }
        if (cursor_pos_world_y <= -350) {
            offset.y -= camera_speed * deltaTime;
        }
        if (cursor_pos_world_y < 350 && cursor_pos_world_y > -350) {
            offset.y = lerp(offset.y, 0.0f, lerpFactor * deltaTime);
        }

        offset.y = AEClamp(offset.y, min_clamp, max_clamp);

        camera_y = position.y + offset.y;

        GetBrickUILeft()->SetTexOffset(0.0f, -camera_y / (GetBrickUILeft()->GetScale().y / 2));
        GetBrickUIRight()->SetTexOffset(0.0f, -camera_y / (GetBrickUILeft()->GetScale().y / 2));
        GetClouds()->SetTexOffset(GetClouds()->GetTexOffset().x, -(camera_y / 10) / (GetClouds()->GetScale().y / 2));
        GetBeanstalk()->SetTexOffset(0.0f, -camera_y / (GetBrickUILeft()->GetScale().y / 2));
    }
    else {
        is_player_parent = false;
        Object2D::Update();
        camera_y = position.y;
    }
}

void Camera2D::EditorUpdate(f32 x_pos, f32& y_pos, f32 x_scale, f32 y_scale) {

	Object2D::EditorUpdate(x_pos, y_pos, x_scale, y_scale);

    //camera_x = position.x;
    //camera_y = position.y;
}

void Camera2D::PostUpdate() {

	Object2D::PostUpdate();
}

void Camera2D::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const {
    writer.StartObject();
    writer.Key("type");
    writer.Int(CAMERA2D);

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

    writer.Key("camera_speed");
    writer.Double(camera_speed);

    writer.Key("min_clamp");
    writer.Double(min_clamp);

    writer.Key("max_clamp");
    writer.Double(max_clamp);

    writer.Key("lerp_factor");
    writer.Double(lerpFactor);

    writer.Key("children");
    writer.StartArray();

    for (const auto& child : children) {
        child->Serialize(writer);
    }

    writer.EndArray();
    writer.EndObject();
}

void Camera2D::Exit() {

	Object2D::Exit();
}
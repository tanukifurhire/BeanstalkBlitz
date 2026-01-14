/* Start Header ************************************************************************/
/*!
\file		RectPlayer.cpp
\author		Tai Xiao Xuan
\par		xiaoxuan.tai@digipen.edu
\date		09/03/2025
\brief		This file implements a RectPlayer class, which handles rendering Rect2Ds in world space.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#include "pch.hpp"
#include "GameDefn.hpp"

RectPlayer::RectPlayer(std::shared_ptr<Object> parent) : Rect2D(std::move(parent)) {}

void RectPlayer::Enter() {
	Rect2D::Enter();
    Update();
}

void RectPlayer::Init(const rapidjson::Value& child_value) {

    Rect2D::Init(child_value);
}

void RectPlayer::PreUpdate() {
	Rect2D::PreUpdate();
}

void RectPlayer::Update() {
	Rect2D::Update();
}

void RectPlayer::PostUpdate() {
    AEMtx33 scale_matrix{ 0.0f };
    AEMtx33Identity(&scale_matrix);
    AEMtx33Scale(&scale_matrix, local_scale.x, local_scale.y);

    AEMtx33 rotation_matrix{ 0.0f };
    AEMtx33Identity(&rotation_matrix);
    AEMtx33Rot(&rotation_matrix, rotation * (PI / 180.0f));

    AEMtx33 translate{ 0.0f };
    AEMtx33Identity(&translate);
    AEMtx33Trans(&translate, position.x - camera_x, position.y - camera_y);

    AEMtx33 transform = { 0 };
    AEMtx33Concat(&transform, &rotation_matrix, &scale_matrix);
    AEMtx33Concat(&transform, &translate, &transform);

    AEGfxSetRenderMode(AE_GFX_RM_COLOR);

    AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);

    AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);

    AEGfxSetBlendMode(AE_GFX_BM_BLEND);
    AEGfxSetTransparency(transparency);

    AEGfxSetTransform(transform.m);

    AEGfxMeshDraw(aeMesh, AE_GFX_MDM_TRIANGLES);

    Object2D::PostUpdate();
}

void RectPlayer::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const {
    writer.StartObject();
    writer.Key("type");
    writer.Int(RECTPLAYER);

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

    writer.Key("transparency");
    writer.Double(transparency);

    writer.Key("color");
    writer.Int(color);

    writer.Key("children");
    writer.StartArray();

    for (const auto& child : children) {
        child->Serialize(writer);
    }

    writer.EndArray();
    writer.EndObject();
}

void RectPlayer::Exit() {
    AEGfxMeshFree(aeMesh);

    Object2D::Exit();
}
/* Start Header ************************************************************************/
/*!
\file	    Rect2D.cpp
\author		Tai Xiao Xuan
\par		xiaoxuan.tai@digipen.edu
\date		09/03/2025
\brief		This file implements a Rect2D class, to render a simple Rectangle on the screen.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#include "pch.hpp"
#include "GameDefn.hpp"
#include <iostream>

Rect2D::Rect2D(std::shared_ptr<Object> parent) : Object2D(std::move(parent)) {}

void Rect2D::Enter() {

    AEGfxMeshStart();

    AEGfxTriAdd(-0.5f, -0.5f, color, 0.0f, 1.0f,
        0.5f, -0.5f, color, 1.0f, 1.0f,
        -0.5f, 0.5f, color, 0.0f, 0.0f);

    AEGfxTriAdd(0.5f, -0.5f, color, 1.0f, 1.0f,
        0.5f, 0.5f, color, 1.0f, 0.0f,
        -0.5f, 0.5f, color, 0.0f, 0.0f);

    aeMesh = AEGfxMeshEnd();

    Object2D::Enter();
}

void Rect2D::Init(const rapidjson::Value& child_value) {

    if (!child_value.HasMember("transparency")) {
        std::cout << "Rect2D has no member 'transparency'!" << std::endl;
        return;
    }

    if (!child_value.HasMember("color")) {
        std::cout << "Rect2D has no member 'color'!" << std::endl;
        return;
    }

    transparency = static_cast<f32>(child_value["transparency"].GetDouble());

    color = static_cast<s32>(child_value["color"].GetInt());

    Object2D::Init(child_value);
}

void Rect2D::PreUpdate() {

    Object2D::PreUpdate();
}

void Rect2D::Update() {

    Object2D::Update();
}

void Rect2D::PostUpdate() {
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

    AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);

    AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);

    AEGfxSetBlendMode(AE_GFX_BM_BLEND);
    AEGfxSetTransparency(transparency);

    AEGfxSetTransform(transform.m);

    AEGfxMeshDraw(aeMesh, AE_GFX_MDM_TRIANGLES);

    Object2D::PostUpdate();
}

void Rect2D::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const {
    writer.StartObject();
    writer.Key("type");
    writer.Int(RECT2D);

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

void Rect2D::Exit() {

    if (aeMesh) {
        AEGfxMeshFree(aeMesh);
    }

    Object2D::Exit();
}

void DrawRect(f32 x_pos, f32 y_pos, f32 x_scale, f32 y_scale, f32 rot, AEGfxVertexList* mesh, f32 red, f32 green, f32 blue, f32 alpha, f32 transparency) {
    AEMtx33 scale_matrix{ 0.0f };
    AEMtx33Identity(&scale_matrix);
    AEMtx33Scale(&scale_matrix, x_scale, y_scale);

    AEMtx33 rotation_matrix{ 0.0f };
    AEMtx33Identity(&rotation_matrix);
    AEMtx33Rot(&rotation_matrix, rot * (PI / 180.0f));

    AEMtx33 translate{ 0.0f };
    AEMtx33Identity(&translate);
    AEMtx33Trans(&translate, x_pos, y_pos);

    AEMtx33 transform = { 0 };
    AEMtx33Concat(&transform, &rotation_matrix, &scale_matrix);
    AEMtx33Concat(&transform, &translate, &transform);

    AEGfxSetRenderMode(AE_GFX_RM_COLOR);

    AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);

    AEGfxSetColorToAdd(red, green, blue, alpha);

    AEGfxSetBlendMode(AE_GFX_BM_BLEND);
    AEGfxSetTransparency(transparency);

    AEGfxSetTransform(transform.m);

    AEGfxMeshDraw(mesh, AE_GFX_MDM_TRIANGLES);
}

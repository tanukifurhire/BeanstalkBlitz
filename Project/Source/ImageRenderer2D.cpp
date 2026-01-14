/* Start Header ************************************************************************/
/*!
\file		ImageRenderer2D.cpp
\author		Tai Xiao Xuan
\par		xiaoxuan.tai@digipen.edu
\date		09/03/2025
\brief		This file Implements functions for a basic Image Renderer that renders an image onto a mesh.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#include "pch.hpp"
#include "GameDefn.hpp"
#include <iostream>

ImageRenderer2D::ImageRenderer2D(std::shared_ptr<Object> parent) : Object2D(std::move(parent)) {}

void ImageRenderer2D::Enter() {

    AEGfxMeshStart();

    AEGfxTriAdd(-0.5f, -0.5f, color, 0.0f, 1.0f * tile_y,
                0.5f, -0.5f, color, 1.0f * tile_x, 1.0f * tile_y,
                -0.5f, 0.5f, color, 0.0f, 0.0f);

    AEGfxTriAdd(0.5f, -0.5f, color, 1.0f * tile_x, 1.0f * tile_y,
                0.5f, 0.5f, color, 1.0f * tile_x, 0.0f,
                -0.5f, 0.5f, color, 0.0f, 0.0f);

    aeMesh = AEGfxMeshEnd();

    Object2D::Enter();
}

void ImageRenderer2D::PreUpdate() {

    Object2D::PreUpdate();
}

void ImageRenderer2D::Update() {
    Object2D::Update();
}

void ImageRenderer2D::PostUpdate() {
    AEMtx33 scale_matrix{ 0.0f };
    AEMtx33Identity(&scale_matrix);
    AEMtx33Scale(&scale_matrix, local_scale.x, local_scale.y);

    AEMtx33 rotation_matrix{ 0.0f };
    AEMtx33Identity(&rotation_matrix);
    AEMtx33Rot(&rotation_matrix, rotation * (PI / 180.0f));

    AEMtx33 translate{ 0.0f };
    AEMtx33Identity(&translate);
    
    switch (space_type) {
    case SCREEN_SPACE:
        AEMtx33Trans(&translate, offset.x, offset.y);
        break;
    case WORLD_SPACE:
        AEMtx33Trans(&translate, position.x - camera_x, position.y - camera_y);
        break;
    }

    AEMtx33 transform = { 0 };
    AEMtx33Concat(&transform, &rotation_matrix, &scale_matrix);
    AEMtx33Concat(&transform, &translate, &transform);

    AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);

    AEGfxSetColorToMultiply(saturation, saturation, saturation, 1.0f);

    AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);

    AEGfxSetBlendMode(AE_GFX_BM_BLEND);
    AEGfxSetTransparency(transparency);

    AEGfxTextureSet(pTex, tex_offset_x, tex_offset_y);

    tex_offset_x += tex_scroll_speed_x * deltaTime;
    tex_offset_y += tex_scroll_speed_y * deltaTime;

    AEGfxSetTransform(transform.m);

    AEGfxMeshDraw(aeMesh, AE_GFX_MDM_TRIANGLES);

    Object2D::PostUpdate();
}

void ImageRenderer2D::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const {
    writer.StartObject();
    writer.Key("type");
    writer.Int(IMAGERENDERER2D);

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

    writer.Key("texture");
    writer.String(tex_file.c_str());

    for (const auto& child : children) {
        child->Serialize(writer);
    }

    writer.EndArray();
    writer.EndObject();
}

void ImageRenderer2D::Exit() {

    AEGfxMeshFree(aeMesh);

    if (pTex) {
        AEGfxTextureUnload(pTex);
    }

    Object2D::Exit();
}

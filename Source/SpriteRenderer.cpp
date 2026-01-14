/* Start Header ************************************************************************/
/*!
\file	    SpriteRenderer.cpp
\author		Tai Xiao Xuan
\par		xiaoxuan.tai@digipen.edu
\date		30/03/2025
\brief		This file implements a SpriteRenderer class, that handles Rendering Sprite Animations

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#include "GameDefn.hpp"

void SpriteRenderer::Enter() {
    AEGfxMeshStart();

    AEGfxTriAdd(-0.5f, -0.5f, color, 0.0f, 1.0f * sprite_tile_y,
        0.5f, -0.5f, color, 1.0f * sprite_tile_x, 1.0f * sprite_tile_y,
        -0.5f, 0.5f, color, 0.0f, 0.0f);

    AEGfxTriAdd(0.5f, -0.5f, color, 1.0f * sprite_tile_x, 1.0f * sprite_tile_y,
        0.5f, 0.5f, color, 1.0f * sprite_tile_x, 0.0f,
        -0.5f, 0.5f, color, 0.0f, 0.0f);

    aeMesh = AEGfxMeshEnd();

    currentState = &idleState;

    Object2D::Enter();
}

void SpriteRenderer::SetState(SpriteState* state) {
    currentState = state;
    current_frame = state->start;
}

void SpriteRenderer::PostUpdate() {
    AEMtx33 scale_matrix{ 0.0f };
    AEMtx33Identity(&scale_matrix);
    AEMtx33Scale(&scale_matrix, local_scale.x * draw_scale, local_scale.y * draw_scale);

    AEMtx33 rotation_matrix{ 0.0f };
    AEMtx33Identity(&rotation_matrix);
    AEMtx33Rot(&rotation_matrix, rotation * (PI / 180.0f));

    AEMtx33 translate{ 0.0f };
    AEMtx33Identity(&translate);

    switch (space_type) {
    case SCREEN_SPACE:
        AEMtx33Trans(&translate, position.x, position.y);
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

    current_frame_timer -= deltaTime;

    if (current_frame_timer < 0.0f) {

        if (current_frame >= currentState->end) {
            if (currentState->loop) {
                current_frame = currentState->start;
            }
        }
        else {
            current_frame++;
            current_frame_timer = current_frame_timer_max;
        }
    }

    tex_offset_y = static_cast<f32>(current_frame / 16) / 16.0f;
    tex_offset_x = static_cast<f32>(current_frame % 16) / 16.0f;

    switch (currentState->page) {
    case 0: {
        AEGfxTextureSet(pTex, tex_offset_x, tex_offset_y);
        break;
    }
    case 1: {
        AEGfxTextureSet(secondaryTex, tex_offset_x, tex_offset_y);
        break;
    }
    }

    AEGfxSetTransform(transform.m);

    AEGfxMeshDraw(aeMesh, AE_GFX_MDM_TRIANGLES);

    Object2D::PostUpdate();
}

void SpriteRenderer::Exit() {

    currentState = nullptr;

    if (secondaryTex) {
        AEGfxTextureUnload(secondaryTex);
    }

    ImageRenderer2D::Exit();
}
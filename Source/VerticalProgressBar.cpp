/* Start Header ************************************************************************/
/*!
\file	    VerticalProgressBar.cpp
\author		Tai Xiao Xuan
\par		xiaoxuan.tai@digipen.edu
\date		30/03/2025
\brief		This file implements a Vertical Progress Bar UI class

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#include "GameDefn.hpp"

void VerticalProgressBar::Enter() {
    AEGfxMeshStart();

    AEGfxTriAdd(-0.5f, 0.0f, color, 0.0f, 1.0f,
        0.5f, 0.0f, color, 1.0f, 1.0f,
        -0.5f, 1.0f, color, 0.0f, 0.0f);

    AEGfxTriAdd(0.5f, 0.0f, color, 1.0f, 1.0f,
        0.5f, 1.0f, color, 1.0f, 0.0f,
        -0.5f, 1.0f, color, 0.0f, 0.0f);

    aeMesh = AEGfxMeshEnd();

    Object2D::Enter();
}

void VerticalProgressBar::PreUpdate() {
    Rect2D::PreUpdate();
}

void VerticalProgressBar::Update() {
    Rect2D::Update();
}

void VerticalProgressBar::PostUpdate() {
    AEMtx33 scale_matrix{ 0.0f };
    AEMtx33Identity(&scale_matrix);
    AEMtx33Scale(&scale_matrix, local_scale.x, local_scale.y);

    AEMtx33 rotation_matrix{ 0.0f };
    AEMtx33Identity(&rotation_matrix);
    AEMtx33Rot(&rotation_matrix, rotation * (PI / 180.0f));

    AEMtx33 translate{ 0.0f };
    AEMtx33Identity(&translate);
    AEMtx33Trans(&translate, offset.x, offset.y);

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

void VerticalProgressBar::Exit() {
    Rect2D::Exit();
}
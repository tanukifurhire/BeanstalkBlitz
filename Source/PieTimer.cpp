
/* Start Header ************************************************************************/
/*!
\file		PieTimer.cpp
\author		Tai Xiao Xuan
\par		xiaoxuan.tai@digipen.edu
\date		30/03/2025
\brief		This file implements functions for a simple pie timer ui object.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#include "GameDefn.hpp"

void PieTimer::Enter() {
    AEGfxMeshStart();

    AEGfxTriAdd(0.0f, 0.0f, color, 0.0f, 1.0f,
        1.0f, 0.0f, color, 1.0f, 1.0f,
        0.0f, 1.0f, color, 0.0f, 0.0f);

    aeMesh = AEGfxMeshEnd();

    Object2D::Enter();
}

void PieTimer::PostUpdate() {
    for (int i{}; i < static_cast<int>(timer_progress * 360.0f); i++) {
        AEMtx33 scale_matrix{ 0.0f };
        AEMtx33Identity(&scale_matrix);
        AEMtx33Scale(&scale_matrix, local_scale.x, local_scale.y);

        AEMtx33 rotation_matrix{ 0.0f };
        AEMtx33Identity(&rotation_matrix);
        AEMtx33Rot(&rotation_matrix, (rotation + 44.1f - static_cast<f32>(i)) * (PI / 180.0f));

        AEMtx33 translate{ 0.0f };
        AEMtx33Identity(&translate);
        AEMtx33Trans(&translate, position.x - camera_x, position.y - camera_y);

        AEMtx33 skew{ 0.0f };
        AEMtx33Identity(&skew);

        f32 skewX = tanf(0.49f * (PI / 2));

        skew.m[0][1] = skewX;
        skew.m[1][0] = skewX;

        AEMtx33 transform = { 0 };
        AEMtx33Concat(&transform, &skew, &scale_matrix);
        AEMtx33Concat(&transform, &rotation_matrix, &transform);
        AEMtx33Concat(&transform, &translate, &transform);

        AEGfxSetRenderMode(AE_GFX_RM_COLOR);

        AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);

        if (i < 120) {
            AEGfxSetColorToAdd(0.0f, 1.0f, 0.0f, 0.0f);
        }
        else if (i < 300) {
            AEGfxSetColorToAdd(1.0f, 0.8f, 0.0f, 0.0f);
        }
        else {
            AEGfxSetColorToAdd(1.0f, 0.0f, 0.0f, 0.0f);
        }

        AEGfxSetBlendMode(AE_GFX_BM_BLEND);
        AEGfxSetTransparency(transparency);

        AEGfxSetTransform(transform.m);

        AEGfxMeshDraw(aeMesh, AE_GFX_MDM_TRIANGLES);
    }

    Object2D::PostUpdate();
}
/* Start Header ************************************************************************/
/*!
\file		DigipenLogo.cpp
\author		Tai Xiao Xuan
\par		xiaoxuan.tai@digipen.edu
\date		06/04/2025
\brief		This file implements a AudioDriver class, that stores all the audio files for this
			game, including Sound Effects and Music.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#include "GameDefn.hpp"

void DigipenLogo::Update() {
	timer -= deltaTime;

	if (timer > max / 2.f) {
		transparency += deltaTime / (max / 2.f);
		current_layer = -1;
	}
	else if (timer > 0.f && timer <= max / 2.f) {
		transparency -= deltaTime / (max / 2.f);
		current_layer = -1;
	}
	else {
		transparency = 0.f;
		AEAudioPlay(GetAudioDriver()->menu, GetAudioDriver()->music_group, 2.f, 1.f, 1);
		Exit();
		current_layer = 0;
		GetRoot()->RemoveChild(shared_from_this());
	}

	if (AEInputCheckTriggered(AEVK_ESCAPE) || AEInputCheckTriggered(AEVK_SPACE) || AEInputCheckTriggered(AEVK_RBUTTON) || AEInputCheckTriggered(AEVK_LBUTTON) || AEInputCheckTriggered(AEVK_RETURN)) {
		if (timer > max / 2.f) {
			timer = max - timer;
		}
	}

	ImageRenderer2D::Update();
}

void DigipenLogo::PostUpdate() {
	if (timer > 0.f) {
		AEMtx33 scale_matrix{ 0.0f };
		AEMtx33Identity(&scale_matrix);
		AEMtx33Scale(&scale_matrix, screen_width, screen_height);

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
		AEGfxSetTransparency(1.f);

		AEGfxSetTransform(transform.m);

		AEGfxMeshDraw(aeMesh, AE_GFX_MDM_TRIANGLES);
	}

	ImageRenderer2D::PostUpdate();
}
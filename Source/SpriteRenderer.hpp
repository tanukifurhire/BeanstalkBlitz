/* Start Header ************************************************************************/
/*!
\file	    SpriteRenderer.hpp
\author		Tai Xiao Xuan
\par		xiaoxuan.tai@digipen.edu
\date		30/03/2025
\brief		This file declares a SpriteRenderer class, that handles Rendering Sprite Animations

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#pragma once
#include "ImageRenderer2D.hpp"

class SpriteRenderer : public ImageRenderer2D {
public:
	SpriteRenderer(std::shared_ptr<Object> parent = nullptr) : ImageRenderer2D(std::move(parent)) {}

	void SetSpriteTiling(f32 x, f32 y) {
		sprite_tile_x = x;
		sprite_tile_y = y;
	}

	void SetDrawScale(f32 scale_v) {
		draw_scale = scale_v;
	}

	void LoadSecondaryMesh(std::string path) {
		secondaryTex = AEGfxTextureLoad(path.c_str());
	}

	void Enter() override;

	void PostUpdate() override;

	void Exit() override;

	struct SpriteState {
		s32 page{};
		s32 start{};
		s32 end{};
		bool loop;
	};

	void SetState(SpriteState* state);

	void SetFPS(f32 frames) {
		frames > 0.0f ? current_frame_timer_max = 1.0f / frames : current_frame_timer_max;
	}

	SpriteState idleState{ 0,0,79,true };
	SpriteState moveState{ 1,0,24,true };
	SpriteState jumpState{ 1,25,84,false };
	SpriteState fallState{ 0,80,90,true };
	SpriteState stunState{ 1,85,124,false };
	SpriteState chargeState{ 0,91,111,true };

	SpriteState bossIdle{ 1,0,0, false };
	SpriteState bossLift{ 1,0,79,false };
	SpriteState bossAttack{ 1,80,100,false };
	SpriteState bossWalk{ 0,0,24,true };

	SpriteState* currentState{ nullptr };

protected:
	f32 sprite_tile_x{1.0f};
	f32 sprite_tile_y{1.0f};

	f32 draw_scale{ 1.0f };

	AEGfxTexture* secondaryTex{ nullptr };
	std::string secondary_tex_file{ "Assets/JackedJack_SpriteSheet2.png" };

	s32 current_frame{};
	f32 current_frame_timer{0.0f};
	f32 current_frame_timer_max{1.0f};
};
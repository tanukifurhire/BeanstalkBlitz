/* Start Header ************************************************************************/
/*!
\file		ImageRenderer2D.hpp
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

#pragma once
#include "Object2D.hpp"

class ImageRenderer2D : public Object2D {
public:
    ImageRenderer2D(std::shared_ptr<Object> parent = nullptr);

    void SetTransparency(f32 val) {
        transparency = val;
    }

    void LoadTexture(const char* pFileName) {
        pTex = AEGfxTextureLoad(pFileName);
    }
    
    void SetTiling(s32 x, s32 y) {
        tile_x = x;
        tile_y = y;
    }

    void SetTexScrollSpeed(f32 x, f32 y) {
        tex_scroll_speed_x = x;
        tex_scroll_speed_y = y;
    }

    AEVec2 GetTexOffset() {
        return { tex_offset_x, tex_offset_y };
    }

    void SetTexOffset(f32 x, f32 y) {
        tex_offset_x = x;
        tex_offset_y = y;
    }

    void SetSaturation(f32 sat) {
        saturation = sat;
    }

    void SetSpaceType(SpaceType type) {
        space_type = type;
    }

    void Enter() override;

    void PreUpdate() override;

    void Update() override;

    void PostUpdate() override;

    void Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const override;

    void Exit() override;

protected:
    AEGfxVertexList* aeMesh = nullptr;
    AEGfxTexture* pTex = nullptr;
    std::string tex_file = "Assets/Brick_Wall.png";

    f32 transparency{ 1.0f };
    f32 saturation{ 1.0f };
    s32 color = 0xFF000000;

    s32 tile_x{ 1 };
    s32 tile_y{ 1 };

    f32 tex_scroll_speed_x{};
    f32 tex_scroll_speed_y{};

    f32 tex_offset_x{};
    f32 tex_offset_y{};

    SpaceType space_type{ SCREEN_SPACE };
};

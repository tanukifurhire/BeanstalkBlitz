/* Start Header ************************************************************************/
/*!
\file	    Rect2D.hpp
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

#pragma once
#include "Object2D.hpp"

void DrawRect(f32 x_pos, f32 y_pos, f32 x_scale, f32 y_scale, f32 rot, AEGfxVertexList* mesh, f32 red, f32 green, f32 blue, f32 alpha, f32 transparency);

class Rect2D : public Object2D {
public:
    Rect2D(std::shared_ptr<Object> parent = nullptr);

    void SetTransparency(f32 val) {
        transparency = val;
    }

    void SetColor(s32 color_val) {
        color = color_val;
    }

    void Enter() override;

    void PreUpdate() override;

    void Update() override;

    void PostUpdate() override;

    void Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const override;

    void Init(const rapidjson::Value& child_value) override;

    void Exit() override;

protected:
    AEGfxVertexList* aeMesh = nullptr;
    f32 transparency = 1.0f;
    s32 color = 0xFF000000;
};

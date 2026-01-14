/* Start Header ************************************************************************/
/*!
\file		Text2D.hpp
\author		Tai Xiao Xuan
\par		xiaoxuan.tai@digipen.edu
\date		09/03/2025
\brief		This file implements a base Text Rendering class, that can print text on the screen.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#pragma once
#include "Object2D.hpp"
#include "AEEngine.h"

class Text2D : public Object2D {
public:
    Text2D(std::shared_ptr<Object> parent = nullptr) : Object2D(std::move(parent)) {}

    std::string GetText() {
        return text_field;
    }

    void SetText(std::string text) {
        text_field = text;
    }

    void SetSpaceType(SpaceType type) {
        space_type = type;
    }

    void SetTextColor(f32 r, f32 g, f32 b, f32 a) {
        red = r;
        green = g;
        blue = b;
        alpha = a;
    }

    void SetTransparency(f32 trans) {
        transparency = trans;
    }

    void Enter() override;

    void PreUpdate() override;

    void Update() override;

    void PostUpdate() override;

    void Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const override;

    void Init(const rapidjson::Value& child_value) override;

    void Exit() override;

protected:
    std::string text_field{ "Hello World!" };
    int font_size = 50;

    std::string font_file = "Assets/SourceSans3-Bold.ttf";

    s8 pFont = 0;

    SpaceType space_type{ SCREEN_SPACE };

    f32 red{1.f};
    f32 green{1.f};
    f32 blue{1.f};
    f32 alpha{1.f};

    f32 transparency{1.f};
};
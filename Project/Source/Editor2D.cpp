/* Start Header ************************************************************************/
/*!
\file		Editor.cpp
\author		Tai Xiao Xuan
\par		xiaoxuan.tai@digipen.edu
\date		09/03/2025
\brief		This file Implements functions for an Editor User Interface to inspect the Object hierarchy and individual objects

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#include "pch.hpp"
#include "GameDefn.hpp"

Editor2D::Editor2D(std::shared_ptr<Object> parent) : Object2D(std::move(parent)) {}

void Editor2D::Enter() {

	editor = std::dynamic_pointer_cast<Editor2D>(shared_from_this());

    Object2D::Enter();
}

void Editor2D::PreUpdate() {

    Object2D::PreUpdate();
}

void Editor2D::Update() {

    //local_rotation += 20.0f * (f32)AEFrameRateControllerGetFrameTime();

    if (enable) {
        Object2D::Update();
    }
}

void Editor2D::PostUpdate() {
    if (AEInputCheckCurr(AEVK_LCTRL) && AEInputCheckCurr(AEVK_LALT) && AEInputCheckTriggered(AEVK_D)) {
        enable = !enable;
    }

    if (enable) {
        Object2D::PostUpdate();
        Hierarchy();
    }
}

void Editor2D::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const {
	writer.StartObject();
	writer.Key("type");
	writer.Int(EDITOR2D);

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

	writer.Key("children");
	writer.StartArray();

	for (const auto& child : children) {
		child->Serialize(writer);
	}

	writer.EndArray();
	writer.EndObject();
}

void Editor2D::Exit() {

    Object2D::Exit();
}

void Editor2D::Hierarchy() {
    f32 y_pos = (0.35f * screen_height);

    for (int i = 0; i < 4; ++i, y_pos -= (0.03f * screen_height)) {
        GetRoot()->GetChildren().at(i)->EditorUpdate(-0.47f * screen_width, y_pos, (0.1875f * screen_width), (0.027f * screen_height));
    }
}

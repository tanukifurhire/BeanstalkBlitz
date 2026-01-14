/* Start Header ************************************************************************/
/*!
\file		Camera2D.hpp
\author		Nelson Tok Duan Jie
\par		t.duanjienelson@digipen.edu
\date		09/03/2025
\brief		This file implements

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#pragma once
#include "Object2D.hpp"

class Camera2D : public Object2D {
public:
	Camera2D(std::shared_ptr<Object> parent = nullptr);

	void Enter() override;

	void PreUpdate() override;

	void Update() override;

	void PostUpdate() override;

	void EditorUpdate(f32 x_pos, f32& y_pos, f32 x_scale, f32 y_scale) override;

	void Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const override;

	void Init(const rapidjson::Value& child_value) override;

	void Exit() override;

	bool is_player_parent = false;

protected:
	f32 camera_speed = 1000.f;
	f32 min_clamp = -200.f;
	f32 max_clamp = 200.f;
	f32 lerpFactor = 5.f;
};
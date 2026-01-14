/* Start Header ************************************************************************/
/*!
\file		Object2D.hpp
\author		Tai Xiao Xuan
\par		xiaoxuan.tai@digipen.edu
\date		09/03/2025
\brief		This file declares the Base 2D Object Interface that all Scene Members must inherit from.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#pragma once
#include "Object.hpp"
#include "Utils.hpp"

enum SpaceType {
	SCREEN_SPACE,
	WORLD_SPACE
};

class Object2D : public Object {
public:
	Object2D(std::shared_ptr<Object> parent = nullptr);

	void SetPosition(AEVec2 position_val) {
		offset = position_val;
	}

	void SetParent(const std::shared_ptr<Object>& new_parent) override;

	AEVec2 GetPosition() {
		return position;
	}

	AEVec2 GetLocalPosition() {
		return offset;
	}

	void SetScale(AEVec2 scale_val) {
		local_scale = scale_val;
	}

	AEVec2 GetScale() {
		return local_scale;
	}

	void SetRotation(f32 rotation_val) {
		local_rotation = rotation_val;
	}

	f32 GetRotation() {
		return rotation;
	}

	void Enter() override;

	void PreUpdate() override;

	void Update() override;

	void PostUpdate() override;

	f32 EditorInspectorUpdate() override;

	void EditorUpdate(f32 x_pos, f32& y_pos, f32 x_scale, f32 y_scale) override;

	void InspectFloat(f32& value, InspectedObject type, std::string field_name, f32 x_pos, f32& y_pos, f32 x_scale, f32 y_scale);

	void Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const override;

	void Init(const rapidjson::Value& child_value) override;

	void Exit() override;

	AABB bounding_box{ {0.0f, 0.0f}, {0.0f, 0.0f} };

	AEVec2 velocity = { 0.0f, 0.0f };

protected:
	AEVec2 position = {0.0f, 0.0f};
	AEVec2 scale = { 1.0f, 1.0f };
	f32 rotation = 0.0f;

	AEVec2 offset = { 0.0f, 0.0f };
	AEVec2 local_scale = { 1.0f, 1.0f };
	f32 local_rotation = 0.0f;
};

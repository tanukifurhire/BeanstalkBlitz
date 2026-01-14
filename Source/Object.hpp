/* Start Header ************************************************************************/
/*!
\file		Object.hpp
\author		Tai Xiao Xuan
\par		xiaoxuan.tai@digipen.edu
\date		09/03/2025
\brief		This file declares the Base Object Interface that all Scene Members must inherit from.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#pragma once
#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"
#include <memory>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include "AEEngine.h"
#include <unordered_map>

enum InspectedObject {
	NONE,
	NAME,
	POSITION_X,
	POSITION_Y,
	SCALE_X,
	SCALE_Y,
	ROTATION
};

namespace rapidjson {
	template <typename Encoding, typename Allocator>
	class GenericValue;

	typedef GenericValue<UTF8<>, MemoryPoolAllocator<>> Value;  // or whatever the correct alias is
}

class Object : public std::enable_shared_from_this<Object> {
public:
	Object(std::shared_ptr<Object> parent = nullptr);

	std::shared_ptr<Object> GetParent() {
		return parent.lock();
	}

	void AddChild(const std::shared_ptr<Object>& child) {
		if (child) {
			std::cout << name << ": " << "Adding " << child->name << "!" << std::endl;
			child->parent = shared_from_this();
			children.push_back(child);

			if (child->already_initialised) {
				return;
			}

			child->Enter();
		}
	}

	void RemoveChild(const std::shared_ptr<Object>& child_to_remove) {
		std::cout << name << ": " << "Removing " << child_to_remove->name << "!" << std::endl;
		
		children.erase(std::remove_if(children.begin(), children.end(), [&child_to_remove](const auto& child) { return child == child_to_remove; }), children.end());
		children.shrink_to_fit();
	}

	const std::vector<std::shared_ptr<Object>>& GetChildren() {
		return children;
	}

	void ClearChildren() {
		for (const auto& child : children) {
			child->Exit();
		}

		children.clear();
	}

	void SetName(std::string name_val) {
		name = name_val;
	}

	std::string GetName() const {
		return name;
	}

	virtual void SetParent(const std::shared_ptr<Object>& new_parent);

	virtual void Enter();

	virtual void PreUpdate();

	virtual void Update();

	virtual void PostUpdate();

	virtual void Exit();

	virtual void EditorUpdate(f32 x_pos, f32& y_pos, f32 x_scale, f32 y_scale);

	virtual f32 EditorInspectorUpdate();

	virtual void Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const;

	virtual void Init(const rapidjson::Value& child_value);

	void InspectName(f32 x_pos, f32& y_pos, f32 x_scale, f32 y_scale);

	bool already_initialised = false;

protected:
	std::weak_ptr<Object> parent;
	std::vector<std::shared_ptr<Object>> children;
	std::string name = "Object";

	InspectedObject inspected_object = NONE;
	bool is_inspected_object_updated = false;
	std::string inspected_field = "";
};

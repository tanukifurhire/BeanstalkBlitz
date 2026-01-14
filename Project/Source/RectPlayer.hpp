/* Start Header ************************************************************************/
/*!
\file		RectPlayer.hpp
\author		Tai Xiao Xuan
\par		xiaoxuan.tai@digipen.edu
\date		09/03/2025
\brief		This file implements a RectPlayer class, which handles rendering Rect2Ds in world space.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#pragma once
#include "Rect2D.hpp"

class RectPlayer : public Rect2D {
public:
	RectPlayer(std::shared_ptr<Object> parent = nullptr);

	void Enter() override;

	void PreUpdate() override;

	void Update() override;

	void PostUpdate() override;

	void Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const override;

	void Init(const rapidjson::Value& child_value) override;

	void Exit() override;
protected:
	
};
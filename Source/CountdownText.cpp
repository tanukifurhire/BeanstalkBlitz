/* Start Header ************************************************************************/
/*!
\file		CountdownText.cpp
\author		Tai Xiao Xuan
\par		xiaoxuan.tai@digipen.edu
\date		30/03/2025
\brief		This file Implements functions for a countdown text UI on screen

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#include "GameDefn.hpp"

void CountdownText::Update() {
	if (!start) {
		return;
	}

	if (GetCamera()->GetParent() != VictoryZone()) {
		GetCamera()->SetParent(VictoryZone());
	}

	countdown -= deltaTime;

	if (countdown <= 3.f) {
		if (GetCamera()->GetParent() != GetPlayer()) {
			GetCamera()->SetParent(GetPlayer());
		}
	}

	std::stringstream ss;

	ss << static_cast<s32>(countdown + 1.f);

	text_field.clear();

	ss >> text_field;

	if (countdown <= 0.f) {

		SetTransparency(0.f);

		for (auto& child : GetPlayerBranch()->GetChildren()) {
			std::shared_ptr<Player> player = std::dynamic_pointer_cast<Player>(child);

			if (player) {
				std::shared_ptr<InputManager> input = std::dynamic_pointer_cast<InputManager>(player->GetChildren().at(5));

				if (input) {
					input->active = true;
				}
			}
		}

		for (auto& child : GetWorldBranch()->GetChildren()) {
			std::shared_ptr<EnemyHeaderClass> enemy = std::dynamic_pointer_cast<EnemyHeaderClass>(child);

			if (enemy) {
				enemy->active = true;
			}
		}

		start = false;
	}
	else {
		SetTransparency(1.f);
		for (auto& child : GetPlayerBranch()->GetChildren()) {
			std::shared_ptr<Player> player = std::dynamic_pointer_cast<Player>(child);

			if (player) {
				std::shared_ptr<InputManager> input = std::dynamic_pointer_cast<InputManager>(player->GetChildren().at(5));

				if (input) {
					input->active = false;
				}
			}
		}
	}
	
	Text2D::Update();
}
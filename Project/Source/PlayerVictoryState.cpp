/* Start Header ************************************************************************/
/*!
\file		PlayerVictoryState.cpp
\author		Tai Xiao Xuan
\par		xiaoxuan.tai@digipen.edu
\date		30/03/2025
\brief		This file implements a VictoryState class, that handles player Victory logic.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#include "PlayerVictoryState.hpp"
#include "PlayerMovementStateMachine.hpp"
#include "GameDefn.hpp"

void PlayerVictoryState::Enter() {
	GetCamera()->SetParent(state_machine->player.lock());

	std::shared_ptr<SpriteRenderer> sprite = std::dynamic_pointer_cast<SpriteRenderer>(state_machine->player.lock()->GetChildren().at(4));

	std::shared_ptr<Text2D> nametag = std::dynamic_pointer_cast<Text2D>(state_machine->player.lock()->GetChildren().at(6));

	current_layer = 3;

	if (nametag) {
		GetVictoryScreen()->SetErrorText(nametag->GetText());
	}
	else {
		GetVictoryScreen()->SetErrorText("Player");
	}

	if (sprite) {
		sprite->SetState(&sprite->jumpState);
	}

	if (state_machine->player.lock() != GetPlayer()) {
		AEAudioStopGroup(GetAudioDriver()->music_group);

		AEAudioPlay(GetAudioDriver()->defeat, GetAudioDriver()->music_group, 1.5f, 1.f, 1);
	}
	else {
		AEAudioStopGroup(GetAudioDriver()->music_group);

		AEAudioPlay(GetAudioDriver()->victory, GetAudioDriver()->music_group, 2.f, 1.f, 1);
	}
}

void PlayerVictoryState::HandleInput() {

} 

void PlayerVictoryState::Update() {
	AEVec2 direction{0.f, 10500.f};

	AEVec2 player_pos{ state_machine->player.lock()->GetLocalPosition().x, state_machine->player.lock()->GetLocalPosition().y };

	AEVec2Sub(&direction, &direction, &player_pos);

	if (AEVec2Length(&direction) <= 5.f) {
		GetVictoryScreen()->SetTransparency(1.f);
		return;
	}

	AEVec2Normalize(&direction, &direction);

	AEVec2Scale(&direction, &direction, 150.f * deltaTime);

	AEVec2 new_pos{state_machine->player.lock()->GetLocalPosition().x, state_machine->player.lock()->GetLocalPosition().y};

	AEVec2Add(&new_pos, &new_pos, &direction);

	state_machine->player.lock()->SetPosition(new_pos);
}

void PlayerVictoryState::PostUpdate() {

}

void PlayerVictoryState::Exit() {

}
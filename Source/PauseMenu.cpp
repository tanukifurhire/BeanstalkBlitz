#include "GameDefn.hpp"

void PauseMenu::Update() {
	if (AEInputCheckTriggered(AEVK_ESCAPE) && current_layer == -3) {
		current_layer = -2;
		on_enable.Emit(nullptr);
	}

	else if (AEInputCheckTriggered(AEVK_ESCAPE) && current_layer == -2) {
		current_layer = -3;
		on_enable.Emit(nullptr);
	}

	UIContainer::Update();
}

void PauseMenu::PostUpdate() {
	UIContainer::PostUpdate();
}
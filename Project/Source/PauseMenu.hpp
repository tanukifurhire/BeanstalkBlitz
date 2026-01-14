#pragma once
#include "UIContainer.hpp"
#include "Event.hpp"

class PauseMenu : public UIContainer {
public:
	PauseMenu(std::shared_ptr<Object> parent = nullptr) : UIContainer(std::move(parent)) {}

	void Update() override;

	void PostUpdate() override;

	Event on_enable;

protected:

};
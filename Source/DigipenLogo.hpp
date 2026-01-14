#pragma once
#include "ImageRenderer2D.hpp"

class DigipenLogo : public ImageRenderer2D {
public:
	DigipenLogo(std::shared_ptr<Object> parent = nullptr) : ImageRenderer2D(std::move(parent)) {}

	void Update() override;

	void PostUpdate() override;

	void SetTimerDuration(f32 dur) {
		max = dur;
		timer = max;
	}

protected:
	f32 timer{};
	f32 max{};
};
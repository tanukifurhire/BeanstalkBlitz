/* Start Header ************************************************************************/
/*!
\file		InputManager.hpp
\author		Tai Xiao Xuan, Vu Phan Hung
\par		xiaoxuan.tai@digipen.edu, phanhung.vu@digipen.edu
\date		09/03/2025
\brief		This file Implements functions for an Input Events Manager for the Player class.
			Hung handled the AIInputManager while Xiao Xuan did the InputManager and ServerInputManager.

			Xiao Xuan - 60%
			Hung - 40%

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#pragma once
#include "Object.hpp"
#include "Event.hpp"

struct InputHistory {
	u64 time{};
	bool jump_pressed;
	bool jump_released;
	bool jump_triggered;
	bool left_pressed;
	bool right_pressed;
	f32 dt{};
	f32 pos_x{};
	f32 pos_y{};
};

class InputManager : public Object
{
public:
	InputManager(std::shared_ptr<Object> parent = nullptr) : Object(std::move(parent)) {}

	void PreUpdate() override;
	
	Event on_jump_pressed;

	Event on_jump_released;

	Event on_jump_triggered;

	Event on_left_pressed;

	Event on_right_pressed;

	bool active = false;

	std::vector<InputHistory> input_history;

private:

};

class ServerInputManager : public InputManager {
public:
	ServerInputManager(std::shared_ptr<Object> parent = nullptr) : InputManager(std::move(parent)) {}

	void PreUpdate() override;

	bool server_on_jump_pressed = false;

	bool server_on_jump_released = false;

	bool server_on_jump_triggered = false;

	bool server_on_left_pressed = false;

	bool server_on_right_pressed = false;

	u64 latest_input_time{0};

	u64 last_input_time{0};

	u64 current_input_time{0};
};

class AIInputManager : public InputManager {
public:
	AIInputManager(std::shared_ptr<Object> parent = nullptr) : InputManager(std::move(parent)) {}

	void PreUpdate() override;

private:
	float jumpHoldTime{};
	bool isJumping = false;
	bool isPaused = false;
	float pauseTimer = 0.0f;

};
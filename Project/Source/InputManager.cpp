/* Start Header ************************************************************************/
/*!
\file		InputManager.cpp
\author		Tai Xiao Xuan, Vu Phan Hung
\par		xiaoxuan.tai@digipen.edu, phanhung.vu@digipen.edu
\date		09/03/2025
\brief		This file Implements functions for an Input Events Manager for the Player class.
			Hung handled the AIInputManager while Xiao Xuan did the InputManager and ServerInputManager.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#include "GameDefn.hpp"
#include "PlayerMovementStateMachine.hpp"
#include <chrono>

void InputManager::PreUpdate()
{
	if (active == false) {
		if (GetNetworkClient()->player_id != -1) {
			std::stringstream stream;

			stream << GetNetworkClient()->server_time << " ";

			stream << "0 " << "0 " << "0 " << "0 " << "0 ";

			stream << deltaTime << " " << GetPlayer()->GetPosition().x << " " << GetPlayer()->GetPosition().y;

			GetNetworkClient()->UpdatePlayerInput(stream.str());
		}

		return;
	}

	bool jump_pressed{false}, jump_released{false}, jump_triggered{false}, left_pressed{false}, right_pressed{false};

	std::stringstream stream;

	stream << GetNetworkClient()->server_time << " ";

	std::shared_ptr<InputMapping> inputmap = std::dynamic_pointer_cast<InputMapping>(GetSettingUIContainer()->GetChildren().at(1));

	if (inputmap) {
		if (AEInputCheckCurr(inputmap->keyBindings[2].key)) {
			on_jump_pressed.Emit(nullptr);
			jump_pressed = true;

			stream << "1 ";
		}
		else {
			stream << "0 ";
		}

		if (AEInputCheckReleased(inputmap->keyBindings[2].key) || !AEInputCheckCurr(inputmap->keyBindings[2].key)) {
			on_jump_released.Emit(nullptr);
			jump_released = true;

			stream << "1 ";
		}
		else {
			stream << "0 ";
		}

		if (AEInputCheckTriggered(inputmap->keyBindings[2].key)) {
			on_jump_triggered.Emit(nullptr);
			jump_triggered = true;

			stream << "1 ";
		}
		else {
			stream << "0 ";
		}

		if (AEInputCheckCurr(inputmap->keyBindings[0].key)) {
			on_left_pressed.Emit(nullptr);
			left_pressed = true;

			stream << "1 ";
		}
		else {
			stream << "0 ";
		}

		if (AEInputCheckCurr(inputmap->keyBindings[1].key)) {
			on_right_pressed.Emit(nullptr);
			right_pressed = true;

			stream << "1 ";
		}
		else {
			stream << "0 ";
		}
	}
	else {
		if (AEInputCheckCurr(AEVK_SPACE)) {
			on_jump_pressed.Emit(nullptr);
			jump_pressed = true;

			stream << "1 ";
		}
		else {
			stream << "0 ";
		}

		if (AEInputCheckReleased(AEVK_SPACE) || !AEInputCheckCurr(AEVK_SPACE)) {
			on_jump_released.Emit(nullptr);
			jump_released = true;

			stream << "1 ";
		}
		else {
			stream << "0 ";
		}

		if (AEInputCheckTriggered(AEVK_SPACE)) {
			on_jump_triggered.Emit(nullptr);
			jump_triggered = true;

			stream << "1 ";
		}
		else {
			stream << "0 ";
		}

		if (AEInputCheckCurr(AEVK_A)) {
			on_left_pressed.Emit(nullptr);
			left_pressed = true;

			stream << "1 ";
		}
		else {
			stream << "0 ";
		}

		if (AEInputCheckCurr(AEVK_D)) {
			on_right_pressed.Emit(nullptr);
			right_pressed = true;

			stream << "1 ";
		}
		else {
			stream << "0 ";
		}
	}

	stream << deltaTime << " " << GetPlayer()->GetPosition().x << " " << GetPlayer()->GetPosition().y;

	if (GetNetworkClient()->player_id != -1) {

		GetNetworkClient()->UpdatePlayerInput(stream.str());

		input_history.push_back({ GetNetworkClient()->server_time, jump_pressed, jump_released, jump_triggered, left_pressed, right_pressed, deltaTime, GetPlayer()->GetPosition().x, GetPlayer()->GetPosition().y});

		while (GetNetworkClient()->server_time - input_history.begin()->time >= 10000000) {
			input_history.erase(input_history.begin());

			if (input_history.empty()) {
				break;
			}
		}
	}
	else if (input_history.size() != 0) {
		input_history.clear();
	}
}

void AIInputManager::PreUpdate() {
	if (!active)
	{
		std::cout << "RETURN 1" << std::endl;
		return;
	}

	std::shared_ptr<AIPlayer> parent_player = std::dynamic_pointer_cast<AIPlayer>(parent.lock());

	if (parent_player->mapObject.empty())
	{
		std::cout << "RETURN 2" << std::endl;

		return;
	}

	if (parent_player->nextPlatform + 1 >= parent_player->mapObject.size()) {
		std::cout << "Surely it doesnt run here " << std::endl;
		if (isJumping)
		{
			jumpHoldTime += deltaTime;
			if (jumpHoldTime >= 0.95f) {  // Release jump after holding for 0.2 seconds
				//std::cout << "AI Jump Released" << std::endl;
				on_jump_released.Emit(nullptr);
				isJumping = false;
				jumpHoldTime = 0.0f;
			}
		}
		else {
			on_jump_pressed.Emit(nullptr);
			std::cout << "WHAT?????" << std::endl;
			isJumping = true;
			jumpHoldTime = 0.0f;
		}
		std::cout << "RETURN 3" << std::endl;

		return;
	}

	//std::cout << "OUTSIDE IF PLAYER POS Y: " << parent_player->GetPosition().y << "MAP OBJECT POSITION: " << parent_player->mapObject.at(parent_player->nextPlatform).y << std::endl;
	//std::cout << "OUTSIDE IF WHAT NEXT PLATFORM? " << parent_player->nextPlatform << std::endl;

	//skip the ground at nextplatform = 0
	//sync up the map at nextplatform = 1

	if (parent_player->nextPlatform < 1 && parent_player->GetPosition().y > parent_player->mapObject.at(parent_player->nextPlatform +1).y)
	{
		parent_player->nextPlatform++;
		std::cout << "RETURN 4" << std::endl;

		return;
	}

	if (parent_player->nextPlatform >= 1)
	{
		if (parent_player->GetPosition().y > parent_player->mapObject.at(parent_player->nextPlatform+1).y)
		{
			//std::cout << "PLAYER IS LOWER THAN THE CURRENT PLATFORM" << std::endl;
			//std::cout << "PLAYER POS Y: " << parent_player->GetPosition().y << "MAP OBJECT POSITION: " << parent_player->mapObject.at(parent_player->nextPlatform).y << std::endl;
			parent_player->nextPlatform++;
			isPaused = true;  // Start 1-second cooldown
			//std::cout << "WHAT NEXT PLATFORM? " << parent_player->nextPlatform << std::endl;
			std::cout << "RETURN 5 and start pausing" << std::endl;

			return;
		}

		else if (parent_player->GetPosition().y < parent_player->mapObject.at(parent_player->nextPlatform).y)
		{
			//std::cout << "PLAYER IS HIGHER THAN THE CURRENT PLATFORM" << std::endl;
			//std::cout << "PLAYER POS Y: " << parent_player->GetPosition().y << "MAP OBJECT POSITION: " << parent_player->mapObject.at(parent_player->nextPlatform).y << std::endl;
			//std::cout << "WHAT NEXT PLATFORM? " << parent_player->nextPlatform << std::endl;
			parent_player->nextPlatform--;
			std::cout << "RETURN 6" << std::endl;

			return;
		}
	}



	if (parent_player->nextPlatform < parent_player->mapObject.size())
		parent_player->curr = parent_player->mapObject.at(parent_player->nextPlatform);

	if (parent_player->nextPlatform < parent_player->mapObject.size() - 1)
	{
		parent_player->next = parent_player->mapObject.at(parent_player->nextPlatform + 1);
	}

	if (isPaused) {
		std::cout << "START PAUSING " << std::endl;
		pauseTimer += deltaTime;
		if (pauseTimer >= 1.2f) {
			std::cout << "FINISH PAUSING " << std::endl;

			isPaused = false;
			pauseTimer = 0.0f;
		}
		else {
			return; // Still pausing
		}
	}


	//next platform to the right
	/*std::cout << "AI POS X: " << parent_player->GetLocalPosition().x << std::endl;
	std::cout << "NEXT PLATFORM POS X: " << parent_player->next.x << std::endl;*/
	if (parent_player->GetLocalPosition().x < parent_player->next.x)
	{
			// Move right
		/*std::cout << "SHOULD IT MOVE RIGHT? " << parent_player->GetLocalPosition().x << " " << parent_player->curr.x << std::endl;*/
			if (parent_player->GetLocalPosition().x < parent_player->curr.x - 0.5f) {
				on_right_pressed.Emit(nullptr);  // Fire the event instead of modifying player_input
				return;
			}
			//jump when near
			else
			{
				if (!isJumping)
				{
					/*std::cout << "IS JUMPING SET TRUE RIGHT" << std::endl;*/
					isJumping = true;
					jumpHoldTime = 0.0f;
				}
			}
		
	}
	else
	{
			/*std::cout << "AI LEFT IS JUMPING? " << isJumping << std::endl;*/


			// Move left
			//std::cout << "SHOULD IT MOVE LEFT? " << parent_player->GetLocalPosition().x << parent_player->curr.x << std::endl;

			if (parent_player->GetLocalPosition().x > parent_player->curr.x + 0.5f) {
				on_left_pressed.Emit(nullptr);   // Fire the event
				return;
			}
			else
			{
				if (!isJumping)
				{
					//std::cout << "IS JUMPING SET TRUE LEFT" << std::endl;

					isJumping = true;
					jumpHoldTime = 0.0f;
				}
			}
		
	}

	if (isJumping)
	{
		//std::cout << "START PRESS JUMP AI " << std::endl;
		on_jump_pressed.Emit(nullptr);

		jumpHoldTime += deltaTime;
		if (jumpHoldTime >= 0.95f) {  // Release jump after holding for 0.2 seconds
			//std::cout << "AI Jump Released" << std::endl;
			on_jump_released.Emit(nullptr);
			isJumping = false;
			jumpHoldTime = 0.0f;
		}
	}
}

void ServerInputManager::PreUpdate() {

	f32 dt = deltaTime;

	std::shared_ptr<ServerPlayer> player = std::dynamic_pointer_cast<ServerPlayer>(parent.lock());

	if (!player) {
		return;
	}

	if (input_history.size() == 0) {
		player->GetPlayerStateMachine()->HandleInput();
		player->GetPlayerStateMachine()->Update();
	}

	for (const auto& input : input_history) {
		deltaTime = input.dt;

		server_on_jump_pressed = input.jump_pressed;
		server_on_jump_released = input.jump_released;
		server_on_jump_triggered = input.jump_triggered;
		server_on_left_pressed = input.left_pressed;
		server_on_right_pressed = input.right_pressed;

		if (server_on_left_pressed) {
			on_left_pressed.Emit(nullptr);
		}

		if (server_on_right_pressed) {
			on_right_pressed.Emit(nullptr);
		}

		if (server_on_jump_pressed) {
			//std::cout << "Jump Press!" << std::endl;
			on_jump_pressed.Emit(nullptr);
		}

		if (server_on_jump_released) {
			//std::cout << "Jump Release!" << std::endl;
			on_jump_released.Emit(nullptr);
		}

		if (server_on_jump_triggered) {
			//std::cout << "Jump Trigger!" << std::endl;
			on_jump_triggered.Emit(nullptr);
		}

		player->SetPosition({ input.pos_x, input.pos_y });

		player->velocity.x = 0.0f;
		player->velocity.y = 0.0f;
		player->GetPlayerStateMachine()->HandleInput();
		player->GetPlayerStateMachine()->Update();
	}

	deltaTime = dt;

	input_history.clear();

	server_on_jump_pressed = false;
	server_on_jump_released = false;
	server_on_jump_triggered = false;
	server_on_left_pressed = false;
	server_on_right_pressed = false;
}
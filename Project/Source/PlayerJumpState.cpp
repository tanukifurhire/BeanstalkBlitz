/* Start Header ************************************************************************/
/*!
\file		PlayerJumpState.cpp
\author		Tai Xiao Xuan
\par		xiaoxuan.tai@digipen.edu
\date		09/03/2025
\brief		This file implements a JumpState class, that handles player Jumping logic.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#include "PlayerJumpState.hpp"
#include "PlayerMovementStateMachine.hpp"
#include "Utils.hpp"

void PlayerJumpState::Enter() {
	state_name = "JumpState";

	state_machine->player.lock()->SetSpeed(state_machine->player.lock()->GetMaxSpeed());

	std::shared_ptr<SpriteRenderer> sprite = std::dynamic_pointer_cast<SpriteRenderer>(state_machine->player.lock()->GetChildren().at(4));

	if (sprite) {
		sprite->SetState(&sprite->jumpState);
	}

	if (state_machine->player.lock() == GetPlayer()) {
		AEAudioPlay(GetAudioDriver()->jump, GetAudioDriver()->sfx_group, 2.f, 1.f, 0);
	}

	PlayerAirborneState::Enter();
}

void PlayerJumpState::HandleInput() {

	state_machine->player.lock()->velocity.x += state_machine->player.lock()->player_input.x * state_machine->player.lock()->GetSpeed() * deltaTime;

	f32 jumpForce = state_machine->player.lock()->GetJumpForce();

	state_machine->player.lock()->velocity.y += AEClamp(jumpForce * deltaTime, 0.0f, 2000.0f * deltaTime);

	jumpForce -= 5000.0f * deltaTime;

	state_machine->player.lock()->SetJumpForce(jumpForce);

	PlayerAirborneState::HandleInput();
}

void PlayerJumpState::Update() {
	f32 jumpForce = state_machine->player.lock()->GetJumpForce();

	if (jumpForce <= 50.0f) {
		state_machine->player.lock()->SetJumpForce(0.0f);
		state_machine->ChangeState(&state_machine->fallingState);
	}

	AEVec2 offset = state_machine->player.lock()->GetLocalPosition();

	offset.y += state_machine->player.lock()->velocity.y;

	state_machine->player.lock()->SetPosition(offset);

	PlayerMovementState::Update();

	//std::shared_ptr<RectPlayer> airborne = std::dynamic_pointer_cast<RectPlayer>(child);

	// AABB Collision
	// if (is_colliding && state_machine->player.lock()->GetJumpForce()) { // if player is not colliding and is jumping (?)
	//for (const auto& child : world->GetChildren()) {

	//	if (!is_colliding) {
	//		f32 tFirst{ 0.f };
	//		if (CollisionIntersection_RectRect(airborne_check->bounding_box, state_machine->player.lock()->velocity, airborne->bounding_box, platform->velocity, tFirst, deltaTime)) {
	//			is_colliding = true;

	//			AEVec2 offset = state_machine->player.lock()->GetLocalPosition();

	//			offset.y += state_machine->player.lock()->velocity.y * tFirst;

	//			state_machine->player.lock()->SetPosition(offset);

	//			state_machine->player.lock()->SetJumpForce(0.0f);
	//			
	//			state_machine->ChangeState(&state_machine->fallingState);

	//			break;

	//			//std::cout << "Colliding with " << platform->GetName() << std::endl;
	//		}
	//	}
	//}


	// //CODE WITH HELP 

	//std::shared_ptr<RectPlayer> airborne_check = std::dynamic_pointer_cast<RectPlayer>(state_machine->player.lock()->GetChildren().at(0));
	//AEVec2 airborne_check_position = airborne_check->GetPosition();

	//std::shared_ptr<Object2D> world = std::dynamic_pointer_cast<Object2D>(GetRoot()->GetChildren().at(0));

	//bool is_colliding = false;

	//// Get player properties
	//auto player = state_machine->player.lock();
	//float jumpForce = player->GetJumpForce();
	//AEVec2 velocity = player->velocity;

	//// Check if the player is airborne (jumping)
	//bool isJumping = (jumpForce > 0.0f);

	//// Ignore collision checks when jumping
	//if (!isJumping) {
	//	for (const auto& child : world->GetChildren()) {
	//		std::shared_ptr<RectPlayer> platform = std::dynamic_pointer_cast<RectPlayer>(child);

	//		if (!is_colliding) {
	//			f32 tFirst{ 0.f };

	//			if (CollisionIntersection_RectRect(airborne_check->bounding_box, velocity, platform->bounding_box, platform->velocity, tFirst, deltaTime)) {
	//				is_colliding = true;

	//				AEVec2 offset = player->GetLocalPosition();
	//				offset.y += velocity.y * tFirst;

	//				player->SetPosition(offset);
	//				player->SetJumpForce(0.0f);

	//				state_machine->ChangeState(&state_machine->fallingState);

	//				break;
	//			}
	//		}
	//	}
	//}

	// Transition to falling state when reaching jump peak
	/*if (isJumping && jumpForce <= 0.0f) {
		player->SetJumpForce(0.0f);
		state_machine->ChangeState(&state_machine->fallingState);
	}*/




}

void PlayerJumpState::PostUpdate() {

	PlayerAirborneState::PostUpdate();
}

void PlayerJumpState::Exit() {
	if (state_machine->player.lock() == GetPlayer()) {
		AEAudioStopGroup(GetAudioDriver()->sfx_group);
	}

	PlayerAirborneState::Exit();
}
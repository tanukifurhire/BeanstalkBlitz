/* Start Header ************************************************************************/
/*!
\file		PlayerMovementState.cpp
\author		Tai Xiao Xuan
\par		xiaoxuan.tai@digipen.edu
\date		09/03/2025
\brief		This file implements a MovementState class, that handles the base horizontal movement of the Player.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#include "GameDefn.hpp"
#include "PlayerMovementState.hpp"
#include "PlayerMovementStateMachine.hpp"
#include "AEEngine.h"
#include "Utils.hpp"

void PlayerMovementState::Enter() {

	std::shared_ptr<InputManager> input = std::dynamic_pointer_cast<InputManager>(state_machine->player.lock()->GetChildren().at(5));
	if (input)
	{
		on_left = [this](void* context) { (void)context;
			this->OnLeft();
		};

		on_right = [this](void* context) { (void)context;
			this->OnRight();
		};

		input->on_left_pressed += on_left;
		input->on_right_pressed += on_right;
	}

	State::Enter();
}

void PlayerMovementState::HandleInput() {

	state_machine->player.lock()->velocity.x += state_machine->player.lock()->player_input.x * state_machine->player.lock()->GetSpeed();

	if (fabsf(state_machine->player.lock()->player_input.x) > 0.0f) {
		if (state_machine->player.lock()->GetSpeed() > 0.0f) {
			std::shared_ptr<SpriteRenderer> sprite = std::dynamic_pointer_cast<SpriteRenderer>(state_machine->player.lock()->GetChildren().at(4));

			if (sprite) {
				sprite->SetScale({ state_machine->player.lock()->player_input.x * 50.0f, sprite->GetScale().y });
			}
		}
	}

	State::HandleInput();
}

void PlayerMovementState::Update() {
	AABB left_wall{ {-screen_width, -99999.f},{world_boundary_x_min, 99999.f} };
	AABB right_wall{ {world_boundary_x_max, -99999.f},{screen_width, 99999.f} };

	f32 tFirst{ 0.0f };

	AEVec2 offset = state_machine->player.lock()->GetLocalPosition();

	AEVec2 calculated_position{ 0.0f, 0.0f };

	AEVec2Add(&calculated_position, &offset, &state_machine->player.lock()->velocity);

	AABB calculated_bounding_box{ {calculated_position.x - state_machine->player.lock()->GetScale().x / 2, calculated_position.y - state_machine->player.lock()->GetScale().y / 2},{calculated_position.x + state_machine->player.lock()->GetScale().x / 2, calculated_position.y + state_machine->player.lock()->GetScale().y / 2} };

	for (auto& child : GetWorldBranch()->GetChildren()) {

		std::shared_ptr<AIPlayer> ai_check = std::dynamic_pointer_cast<AIPlayer>(state_machine->player.lock());

		if (ai_check) {
			break;
		}
		
		std::shared_ptr<RectPlayer> platform = std::dynamic_pointer_cast<RectPlayer>(child);

		if (platform) {

			if (platform->GetName() == "SPIKE") {
				f32 platform_tFirst{ 0.0f };
				if (CollisionIntersection_RectRect(state_machine->player.lock()->bounding_box, state_machine->player.lock()->velocity, platform->bounding_box, platform->velocity, platform_tFirst, deltaTime)) {

					if (state_machine->current_state != &state_machine->stunState && state_machine->current_state != &state_machine->flyingState) {
						AEVec2 player_offset = state_machine->player.lock()->GetLocalPosition();

						player_offset.y += state_machine->player.lock()->velocity.y * platform_tFirst;

						state_machine->player.lock()->SetPosition(player_offset);

						dynamic_cast<PlayerStunState*>(&state_machine->stunState)->pos_x = state_machine->player.lock()->GetLocalPosition().x;
						dynamic_cast<PlayerStunState*>(&state_machine->stunState)->pos_y = state_machine->player.lock()->GetLocalPosition().y + state_machine->player.lock()->GetScale().y * 0.5f;
						dynamic_cast<PlayerStunState*>(&state_machine->stunState)->stun_type = ENEMY_STUN;

						state_machine->ChangeState(&state_machine->stunState);
					}

					break;
				}
			}

			continue;
		}

		std::shared_ptr<ImageRenderer2D> powerup = std::dynamic_pointer_cast<ImageRenderer2D>(child);

		if (powerup && powerup->GetName() == "POWERUP") {
			f32 powerup_tFirst{ 0.f };

			if (CollisionIntersection_RectRect(state_machine->player.lock()->bounding_box, state_machine->player.lock()->velocity, powerup->bounding_box, powerup->velocity, powerup_tFirst, deltaTime)) {
				
				if (state_machine->current_state != &state_machine->stunState) {
					powerup->Exit();

					GetWorldBranch()->RemoveChild(powerup);

					state_machine->ChangeState(&state_machine->flyingState);
				}

				break;
			}
		}
	}

	if (CollisionIntersection_RectRect(state_machine->player.lock()->bounding_box, state_machine->player.lock()->velocity, left_wall, {0.0f, 0.0f}, tFirst, deltaTime)) {
		if (!CollisionIntersection_RectRect(calculated_bounding_box, {0.0f, 0.0f}, left_wall, {0.0f, 0.0f}, tFirst, deltaTime)) {
			offset.x += state_machine->player.lock()->velocity.x * deltaTime;

			state_machine->player.lock()->SetPosition(offset);
		}
		else {
			offset.x += state_machine->player.lock()->velocity.x * tFirst;

			state_machine->player.lock()->SetPosition(offset);
		}
	}
	else if (CollisionIntersection_RectRect(state_machine->player.lock()->bounding_box, state_machine->player.lock()->velocity, right_wall, { 0.0f, 0.0f }, tFirst, deltaTime)) {
		if (!CollisionIntersection_RectRect(calculated_bounding_box, { 0.0f, 0.0f }, right_wall, { 0.0f, 0.0f }, tFirst, deltaTime)) {
			offset.x += state_machine->player.lock()->velocity.x * deltaTime;

			state_machine->player.lock()->SetPosition(offset);
		}
		else {
			offset.x += state_machine->player.lock()->velocity.x * tFirst;

			state_machine->player.lock()->SetPosition(offset);
		}
	}
	else {
		offset.x += state_machine->player.lock()->velocity.x * deltaTime;

		state_machine->player.lock()->SetPosition(offset);
	}

	for (auto& child : GetPlayerBranch()->GetChildren()) {
		std::shared_ptr<Player> player = std::dynamic_pointer_cast<Player>(child);

		if (player) {
			f32 collision_tFirst{};
			if (CollisionIntersection_RectRect(player->bounding_box, player->velocity, state_machine->player.lock()->bounding_box, state_machine->player.lock()->velocity, collision_tFirst, deltaTime)) {
				
				if (player->velocity.y > state_machine->player.lock()->velocity.y && player->GetLocalPosition().y < state_machine->player.lock()->GetLocalPosition().y && player->velocity.y > 0.f) {
					dynamic_cast<PlayerStunState*>(&state_machine->stunState)->pos_x = state_machine->player.lock()->GetLocalPosition().x;
					dynamic_cast<PlayerStunState*>(&state_machine->stunState)->pos_y = state_machine->player.lock()->GetLocalPosition().y + (state_machine->player.lock()->GetLocalPosition().y - player->GetLocalPosition().y < 0.f ? state_machine->player.lock()->GetScale().y / 2.f : -state_machine->player.lock()->GetScale().y / 2.f);
					dynamic_cast<PlayerStunState*>(&state_machine->stunState)->stun_type = ENEMY_STUN;
					
					state_machine->ChangeState(&state_machine->stunState);
					return;
				}
			}
		}
	}

	for (auto& child : GetWorldBranch()->GetChildren()) {
		std::shared_ptr<FinishLine> finishLine = std::dynamic_pointer_cast<FinishLine>(child);

		if (finishLine) {
			std::shared_ptr<InputManager> input = std::dynamic_pointer_cast<InputManager>(state_machine->player.lock()->GetChildren().at(5));

			std::shared_ptr<RectPlayer> ground_check = std::dynamic_pointer_cast<RectPlayer>(state_machine->player.lock()->GetChildren().at(0));

			if (input && ground_check) {
				f32 collision_tFirst{};

				if (input->active) {
					std::shared_ptr<ImageRenderer2D> progress_bar = std::dynamic_pointer_cast<ImageRenderer2D>(state_machine->player.lock()->GetChildren().at(9));

					std::shared_ptr<VerticalProgressBar> vprog = std::dynamic_pointer_cast<VerticalProgressBar>(state_machine->player.lock()->GetChildren().at(8));

					if (progress_bar && vprog) {
						progress_bar->SetTransparency(1.f);
						vprog->SetTransparency(1.f);
						f32 y_pos = (ground_check->GetPosition().y / 10200.f) * (0.8f * screen_height);
						progress_bar->SetPosition({ progress_bar->GetLocalPosition().x, -0.4f * screen_height + y_pos});
						vprog->SetScale({ 20.f, y_pos });
					}
				}
				else {
					std::shared_ptr<ImageRenderer2D> progress_bar = std::dynamic_pointer_cast<ImageRenderer2D>(state_machine->player.lock()->GetChildren().at(9));

					std::shared_ptr<VerticalProgressBar> vprog = std::dynamic_pointer_cast<VerticalProgressBar>(state_machine->player.lock()->GetChildren().at(8));

					if (progress_bar && vprog) {
						progress_bar->SetTransparency(0.f);

						vprog->SetTransparency(0.f);
					}
				}

				if (CollisionIntersection_RectRect(ground_check->bounding_box, state_machine->player.lock()->velocity, finishLine->bounding_box, finishLine->velocity, collision_tFirst, deltaTime)) {
					// This player has finished!

					input->active = false;

					state_machine->active = false;

					state_machine->ChangeState(&state_machine->victoryState);

					GetWorldBranch()->RemoveChild(finishLine);
				}
			}
		}
	}

	State::Update();
}

void PlayerMovementState::PostUpdate() {

	State::PostUpdate();
}

void PlayerMovementState::OnLeft() {
	state_machine->player.lock()->player_input.x -= 1.0f;
}

void PlayerMovementState::OnRight() {
	state_machine->player.lock()->player_input.x += 1.0f;
}

void PlayerMovementState::Exit() {
	std::shared_ptr<InputManager> input = std::dynamic_pointer_cast<InputManager>(state_machine->player.lock()->GetChildren().at(5));
	if (input)
	{
		input->on_left_pressed -= on_left;

		input->on_right_pressed -= on_right;
	}
	State::Exit();
}


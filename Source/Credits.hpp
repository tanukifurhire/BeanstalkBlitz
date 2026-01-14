/* Start Header ************************************************************************/
/*!
\file		Credits.hpp
\author		Xavier Seet
\par		x.seet@digipen.edu
\date		19/03/2025
\brief		This file implements a credit screen to show the developers and
			coordinators of the game.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#pragma once
#include "Rect2D.hpp"

class Credits : public UIContainer {
public:
	Credits(std::shared_ptr<Object> parent = nullptr);

	void Enter() override;

	void PreUpdate() override;

	void Update() override;

	void PostUpdate() override;

	void Exit() override;

	void SetTextScrollSpeed(f32 val) {
		scroll_speed = val;
	}

	bool roll_credits{ false };
	bool CreditsState = false;

	std::vector<std::string> credits_names = {
		"CREDITS",
		"",
		"",
		"DEVELOPED BY",
		"Tai Xiao Xuan, xiaoxuan.tai@digipen.edu",
		"Hung Vu Phan, phanhung.vu@digipen.edu",
		"Tan Chun Kwang Xavier, chunkwangxavier.tan@digipen.edu",
		"Nelson Tok Duan Jie, t.duanjienelson@digipen.edu",
		"Xavier Seet, x.seet@digipen.edu",
		"",
		"",
		"INSTRUCTORS",
		"Cheng Ding Xiang", 
		"Gerald Wong", 
		"Soroor",
		"",
		"PRESIDENT",
		"Claude Comair"
		"",
		"",
		"EXECUTIVES",
		"Jason Chu, John Bauer",
		"Samir Abou Samra, Raymond Yan",
		"Prasanna Ghali ,Michelle Comair",
		"Xin Li, Angela Kugler",
		"Melvin Gonsalvez, Meighan McKelvey"
		"",
		"",
		"SPECIAL THANKS",
		"Javier Chong     Charlzell Tay",
		"Smack Studio",
		"",
		"",
		"\"Bossa Antigua\" Kevin MacLeod (incompetech.com)",
		"\"Monkeys Spinning Monkeys\" Kevin MacLeod (incompetech.com)",
		"\"Happy Bee\" Kevin MacLeod (incompetech.com)",
		"\"Oppressive Gloom\" Kevin MacLeod (incompetech.com)",
		"Licensed under Creative Commons: By Attribution 4.0 License",
		"http://creativecommons.org/licenses/by/4.0/"
		"",
		"",
		"",
		"",
		"All content copyrighted 2025 DigiPen Institute of Technology Singapore.",
		"All Rights Reserved"
	};

protected:
	f32 scroll_speed{ 50.f };
};
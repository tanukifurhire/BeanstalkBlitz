/* Start Header ************************************************************************/
/*!
\file	    Credits.cpp
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

#include "pch.hpp"
#include "GameDefn.hpp"
#include <iostream>


Credits::Credits(std::shared_ptr<Object> parent) : UIContainer(std::move(parent)) {}

void Credits::Enter() {

    UIContainer::Enter();
}

void Credits::PreUpdate() {

    //HandleInput();

    UIContainer::PreUpdate();
}

void Credits::Update() {

    if (roll_credits) {
        // Adjust scroll speed based on input
        if (AEInputCheckCurr(AEVK_SPACE) || AEInputCheckCurr(AEVK_LBUTTON)) {
            scroll_speed = 200.0f;  // Increase speed when left mouse button is pressed
        }
        else {
            scroll_speed = 50.0f;  // Default scrolling speed
        }

        for (auto& child : GetChildren()) {
            std::shared_ptr<Text2D> text = std::dynamic_pointer_cast<Text2D>(child);

            if (text) {
                AEVec2 text_offset{};

                text_offset = text->GetLocalPosition();

                // Wrap text to be an infinite loop
                if (text_offset.y >= screen_height) {
                    text_offset.y = -0.6f * screen_height; // Reset position to bottom
                }
                
                text_offset.y += scroll_speed * deltaTime; // Text moves up every frame

                text->SetPosition(text_offset);
            }
        }
    }
    else {
        f32 startY = 0.45f * screen_height; // Starting position at the top
        f32 yOffset = -0.03f * screen_height; // Distance between each name

        for (auto& child : GetChildren()) {
            std::shared_ptr<Text2D> text = std::dynamic_pointer_cast<Text2D>(child);

            if (text) {
                text->SetPosition({0.f * screen_width, startY});

                startY += yOffset;
            }
        }
    }

    UIContainer::Update();
}

void Credits::PostUpdate() {

    UIContainer::PostUpdate();
}


void Credits::Exit() {

    UIContainer::Exit();
}

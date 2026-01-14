/* Start Header ************************************************************************/
/*!
\file		Tutorial.cpp
\author		Tok Duan Jie Nelson
\par		t.duanjienelson@digipen.edu
\date		30/03/2025
\brief		This file implements the definitions of functions needed for tutorial

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#include "pch.hpp"
#include "GameDefn.hpp"

Tutorial::Tutorial(std::shared_ptr<Object> parent) : Object2D(std::move(parent)) {}

void Tutorial::HandleInput() {
    if (TutorialState) {
        if (AEInputCheckTriggered(AEVK_UP)) {                   // page 1
            GetTutorialUIContainer()->MoveTo(0.0f); 
        }
        if (AEInputCheckTriggered(AEVK_DOWN)) {                 // page 2
            GetTutorialUIContainer()->MoveTo(screen_height); 
        }
        if (AEInputCheckTriggered(AEVK_ESCAPE)) {               // exit tutorial state
            if (current_layer == -2) {
                GetPauseMenu()->MoveTo(0.f);
                GetTutorialUIContainer()->MoveTo(-screen_height);
            }
            else {
                GetMainMenuUIContainer()->MoveTo(0.0f);
                GetTutorialUIContainer()->MoveTo(-screen_height);
            }

            TutorialState = false;

            AEAudioPlay(GetAudioDriver()->cancel, GetAudioDriver()->sfx_group, 2.f, 1.f, 0);
        }
    }
}

void Tutorial::Enter() {
    AEGfxMeshStart();
    AEGfxTriAdd(
        -0.5f, -0.5f, 0xFFFFFFFF, 0.0f, 1.0f,
        0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f,
        -0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f);

    AEGfxTriAdd(
        0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f,
        0.5f, 0.5f, 0xFFFFFFFF, 1.0f, 0.0f,
        -0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f);

    pMesh = AEGfxMeshEnd();
    pText = AEGfxCreateFont("Assets/SourceCodePro-Black.ttf", 32);
	Object2D::Enter();
}

void Tutorial::PreUpdate() {

    HandleInput();

	Object2D::PreUpdate();
}

void Tutorial::Update() {

    

	Object2D::Update();
}

void Tutorial::PostUpdate() {
    AEMtx33 scale_matrix{ 0.0f };
    AEMtx33Identity(&scale_matrix);
    AEMtx33Scale(&scale_matrix, local_scale.x, local_scale.y);
    
    AEMtx33 rotation_matrix{ 0.0f };
    AEMtx33Identity(&rotation_matrix);
    AEMtx33Rot(&rotation_matrix, rotation * (PI / 180.0f));
    
    AEMtx33 translate{ 0.0f };
    AEMtx33Identity(&translate);
    AEMtx33Trans(&translate, position.x, position.y);
    
    AEMtx33 transform = { 0 };
    AEMtx33Concat(&transform, &rotation_matrix, &scale_matrix);
    AEMtx33Concat(&transform, &translate, &transform);
    
    std::shared_ptr<Object2D> parent_object2D = std::dynamic_pointer_cast<Object2D>(parent.lock());



    // drawing UI box for input mapping setting
    AEGfxSetRenderMode(AE_GFX_RM_COLOR);
    AEGfxSetColorToAdd(1.0f, 1.0f, 1.0f, 1.0f);
    AEGfxSetBlendMode(AE_GFX_BM_NONE);
    AEGfxSetTransform(transform.m);
    AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);

    // for rendering text
    AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
    AEGfxSetBlendMode(AE_GFX_BM_BLEND);

    // page 1 of tutorial
    AEGfxPrint(pText, "How To Play", 
        -0.2f, (position.y + (0.95f * screen_height)) / (screen_height / 2),
        1.5f, 0.0f, 0.0f, 0.0f, 1.0f);
    AEGfxPrint(pText, "Default keys:",
        -0.48f, (position.y + (0.85f * screen_height)) / (screen_height / 2),
        1.2f, 0.0f, 0.0f, 0.0f, 1.0f);
    AEGfxPrint(pText, "'A'* to move left.",
        -0.45f, (position.y + ((0.80f - 0*text_y) * screen_height)) / (screen_height / 2),
        1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
    AEGfxPrint(pText, "'D'* to move right.",
        -0.45f, (position.y + ((0.80f - 2 * text_y) * screen_height)) / (screen_height / 2),
        1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
    AEGfxPrint(pText, "Tap 'Space'* to jump.",
        -0.45f, (position.y + ((0.80f - 4 * text_y) * screen_height)) / (screen_height / 2),
        1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
    AEGfxPrint(pText, "Hold 'Space'* to charge PowerJump.",
        -0.45f, (position.y + ((0.80f - 6 * text_y) * screen_height)) / (screen_height / 2),
        1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
    AEGfxPrint(pText, "Use your 'Mouse' to look beyond",
        -0.45f, (position.y + ((0.80f - 8 * text_y) * screen_height)) / (screen_height / 2),
        1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
    AEGfxPrint(pText, "your screen.",
        -0.45f, (position.y + ((0.80f - 9 * text_y) * screen_height)) / (screen_height / 2),
        1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
    AEGfxPrint(pText, "*You can change your keys bindings",
        -0.45f, (position.y + ((0.80f - 11 * text_y) * screen_height)) / (screen_height / 2),
        1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
    AEGfxPrint(pText, "in the SETTINGS page.",
        -0.45f, (position.y + ((0.80f - 12 * text_y) * screen_height)) / (screen_height / 2),
        1.0f, 0.0f, 0.0f, 0.0f, 1.0f);

    AEGfxPrint(pText, "Press 'Esc' key to return to main menu.",
        -0.45f, (position.y + (0.14f * screen_height)) / (screen_height / 2),
        0.8f, 0.0f, 0.0f, 0.0f, 1.0f);
    AEGfxPrint(pText, "Press 'down arrow' key to scroll to page 2.",
        -0.45f, (position.y + (0.10f * screen_height)) / (screen_height / 2),
        0.8f, 0.0f, 0.0f, 0.0f, 1.0f);

    // page 2 of tutorial
    AEGfxPrint(pText, "How to Win",
        -0.2f, (position.y + (-0.05f * screen_height)) / (screen_height / 2),
        1.5f, 0.0f, 0.0f, 0.0f, 1.0f);
    AEGfxPrint(pText, "Be the faster jumper to reach",
        -0.45f, (position.y + ((-0.15f - 0 * text_y) * screen_height)) / (screen_height / 2),
        1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
    AEGfxPrint(pText, "the top!!!",
        -0.45f, (position.y + ((-0.15f - 1 * text_y) * screen_height)) / (screen_height / 2),
        1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
    AEGfxPrint(pText, "Use PowerJump to knock down",
        -0.45f, (position.y + ((-0.15f - 3 * text_y) * screen_height)) / (screen_height / 2),
        1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
    AEGfxPrint(pText, "your opponent!!!",
        -0.45f, (position.y + ((-0.15f - 4 * text_y) * screen_height)) / (screen_height / 2),
        1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
    AEGfxPrint(pText, "Avoid enemies to avoid",
        -0.45f, (position.y + ((-0.15f - 6 * text_y) * screen_height)) / (screen_height / 2),
        1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
    AEGfxPrint(pText, "getting stun!!!",
        -0.45f, (position.y + ((-0.15f - 7 * text_y) * screen_height)) / (screen_height / 2),
        1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
    AEGfxPrint(pText, "Use Power-ups to get ahead!!!",
        -0.45f, (position.y + ((-0.15f - 9 * text_y) * screen_height)) / (screen_height / 2),
        1.0f, 0.0f, 0.0f, 0.0f, 1.0f);


    AEGfxPrint(pText, "Press 'Esc' key to return to main menu.",
        -0.45f, (position.y + (-0.86f * screen_height)) / (screen_height / 2),
        0.8f, 0.0f, 0.0f, 0.0f, 1.0f);
    AEGfxPrint(pText, "Press 'up arrow' key to scroll to page 1.",
        -0.45f, (position.y + (-0.90f * screen_height)) / (screen_height / 2),
        0.8f, 0.0f, 0.0f, 0.0f, 1.0f);
    Object2D::PostUpdate();
}

void Tutorial::Exit() {

    AEGfxMeshFree(pMesh); // free mesh
    AEGfxDestroyFont(pText); //free text

    Object2D::Exit();
}
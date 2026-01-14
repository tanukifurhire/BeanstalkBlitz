/* Start Header ************************************************************************/
/*!
\file		InputMapping.cpp
\author		Tok Duan Jie Nelson, Tai Xiao Xuan
\par		t.duanjienelson@digipen.edu, xiaoxuan.tai@digipen.edu
\date		09/03/2025
\brief		This file implements the definitions of all the function to be used in input mapping
            
            Nelson - provide the behavior of input mapping

            Xiao Xuan - added rapidjson as IO file to save user's setting 
            and linked input mapping to the game controls

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#include "pch.hpp"
#include "GameDefn.hpp"
#include <fstream>

InputMapping::InputMapping(std::shared_ptr<Object> parent) : Object2D(std::move(parent)) {}

bool InputMapping::CheckSameKey(u8 newKey) {
    for (const auto& binding : keyBindings) {
        if (binding.key == newKey) {                            // return true if existing key is same as new input key 
            return true;                                        
        }
    }
    return false;
}

/*bool InputMapping::IsKeyPressed(const std::string& action) {                      
    return AEInputCheckCurr(GetKeyBinding(action));
}

u8 InputMapping::GetKeyBinding(const std::string& action) {                         
    for (const auto& binding : keyBindings) {
        if (binding.action == action) {
            return binding.key;
        }
    }
    return NULL;
}*/

void InputMapping::ChangeKeyBinding(const std::string& action, u8 newKey) { 
    if (CheckSameKey(newKey)) {
        //std::cerr << "Key is already in use. Choose another key.\n";              // Debugging 
        keyConflict = true;
        return;
    }

    for (auto& binding : keyBindings) {
        if (binding.action == action) {
            binding.key = newKey;
            //std::cout << "Updated " << action << " to " << newKey << "\n";        // Debugging
            return;
        }
    }
    //std::cerr << "Invalid action name.\n" << action << "\n";                      // Debugging
} // end ChangeKeyBinding()

void InputMapping::RenderUI() {
    float yOffset = (position.y + (0.15f * screen_height)) / (screen_height / 2);                               // Starting y position to print each selection
    for (int i = 0; i < 3; ++i) {
        
        if (i == selectedOption) {
            AEGfxPrint(pText, ">>", -0.33f, yOffset, 1.0f, 1.f, 1.f, 1.f, 1.0f);                             // Highlight selection with ">>"
        }

        AEGfxPrint(pText, keyBindings[i].action.c_str(), -0.28f, yOffset, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);        // Render the movement's name state for the player

        if (keyConflict) {
            AEGfxPrint(pText, "Duplicated Key! Choose another.", -0.33f, 
            (position.y + (0.21f * screen_height)) / (screen_height / 2), 1.0f, 1.0f, 0.2f, 0.2f, 1.0f);        // Render Conflicting key input
        }

        std::string keyName = "[" + GetKeyName(keyBindings[i].key) + "]";                                       // Render keyNameMap class
        AEGfxPrint(pText, keyName.c_str(), 0.2f, yOffset, 1.0f, 1.f, 1.f, 1.f, 1.0f);


        yOffset -= (0.08f * screen_height) / (screen_height / 2);                                               // Next y position to print slection
    }

    if (awaitingKeyPress) {
        AEGfxPrint(pText, "Press a new key...", -0.2f, 
        (position.y - (0.2f * screen_height)) / (screen_height / 2), 1.0f, 1.0f, 0.2f, 0.2f, 1.0f);             // Prompt the user to input new key state
    }
    else {                                                                                                      
        AEGfxPrint(pText, "Use 'Up/Down' to navigate, 'Enter' to rebind", -0.38f, 
        (position.y - (0.18f * screen_height)) / (screen_height / 2), 0.8f, 1.0f, 1.0f, 1.0f, 1.0f);            // Guideline for using InputMapping
        AEGfxPrint(pText, "Press 'Esc' to exit Input Mapping", -0.38f, 
        (position.y - (0.22f * screen_height)) / (screen_height / 2), 0.8f, 1.0f, 1.0f, 1.0f, 1.0f);            // Guideline for using InputMapping
    }
} // end RenderUI()

void InputMapping::HandleInput() {
    if (SettingState) {                                 
        if (awaitingKeyPress) {
            for (u8 key = 1; key < 255; ++key) {                                                                // Scan all possible keys
                if (AEInputCheckTriggered(key)) {
                    keyConflict = false;
                    ChangeKeyBinding(keyBindings[selectedOption].action, key);
                    awaitingKeyPress = false;
                    return;
                }
            }
        }
        else {
            if (AEInputCheckTriggered(AEVK_UP) && selectedOption > 0) {
                selectedOption--;
            }
            if (AEInputCheckTriggered(AEVK_DOWN) && selectedOption < 2) {
                selectedOption++;
            }
            if (AEInputCheckTriggered(AEVK_RETURN)) {
                awaitingKeyPress = true;
            }
            if (AEInputCheckTriggered(AEVK_ESCAPE)) {
                GetMainMenuUIContainer()->MoveTo(0.0f);
                GetSettingUIContainer()->MoveTo(-screen_height);
                SettingState = false;

                AEAudioPlay(GetAudioDriver()->cancel, GetAudioDriver()->sfx_group, 2.f, 1.f, 0);
            }
        }
    }
}




void InputMapping::Enter() {
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

    std::ifstream file("input_map.json");

    if (!file.is_open()) {
        std::cout << "Input map not found!" << std::endl;

        keyBindings[0] = { "MoveLeft", AEVK_A };
        keyBindings[1] = { "MoveRight", AEVK_D };
        keyBindings[2] = { "Jump", AEVK_SPACE };
        //keyBindings[3] = { "Use PowerUp", AEVK_E };
    }
    else {
        std::ostringstream oss;

        oss << file.rdbuf();

        std::string json_content = oss.str();

        rapidjson::Document document;

        document.Parse(json_content);

        if (document.HasParseError()) {
            std::cout << "Error Parsing JSON file!" << std::endl;

            keyBindings[0] = { "MoveLeft", AEVK_A };
            keyBindings[1] = { "MoveRight", AEVK_D };
            keyBindings[2] = { "Jump", AEVK_SPACE };
            //keyBindings[3] = { "Use PowerUp", AEVK_E };
        }

        else if (!document.IsObject()) {
            std::cout << "JSON must contain an Object at root level!" << std::endl;

            keyBindings[0] = { "MoveLeft", AEVK_A };
            keyBindings[1] = { "MoveRight", AEVK_D };
            keyBindings[2] = { "Jump", AEVK_SPACE };
            //keyBindings[3] = { "Use PowerUp", AEVK_E };
        }

        else {
            keyBindings[0] = { "MoveLeft", static_cast<u8>(document["MoveLeft"].GetInt()) };
            keyBindings[1] = { "MoveRight", static_cast<u8>(document["MoveRight"].GetInt()) };
            keyBindings[2] = { "Jump", static_cast<u8>(document["Jump"].GetInt()) };
            //keyBindings[3] = { "Use PowerUp", static_cast<u8>(document["PowerUp"].GetInt()) };
        }
    }

    Object2D::Enter();
}

void InputMapping::PreUpdate() {

    HandleInput();

    Object2D::PreUpdate();
}

void InputMapping::Update() {

    Object2D::Update();
}

void InputMapping::PostUpdate() {
    Object2D::PostUpdate();

    //AEMtx33 scale_matrix{ 0.0f };
    //AEMtx33Identity(&scale_matrix);
    //AEMtx33Scale(&scale_matrix, local_scale.x, local_scale.y);

    //AEMtx33 rotation_matrix{ 0.0f };
    //AEMtx33Identity(&rotation_matrix);
    //AEMtx33Rot(&rotation_matrix, rotation * (PI / 180.0f));

    //AEMtx33 translate{ 0.0f };
    //AEMtx33Identity(&translate);
    //AEMtx33Trans(&translate, position.x, position.y);

    //AEMtx33 transform = { 0 };
    //AEMtx33Concat(&transform, &rotation_matrix, &scale_matrix);
    //AEMtx33Concat(&transform, &translate, &transform);

    //std::shared_ptr<Object2D> parent_object2D = std::dynamic_pointer_cast<Object2D>(parent.lock());


    //// drawing UI box for input mapping setting
    //AEGfxSetRenderMode(AE_GFX_RM_COLOR);
    //AEGfxSetColorToAdd(1.0f, 1.0f, 1.0f, 1.0f);
    //AEGfxSetBlendMode(AE_GFX_BM_NONE);
    //AEGfxSetTransform(transform.m);
    //AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);

    // for rendering input mapping setting 
    AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
    AEGfxSetBlendMode(AE_GFX_BM_BLEND);
    //AEGfxPrint(pText, "This is a Test.", 0.0f, 0.0f, 1.0f, 0.f, 0.f, 0.f, 1.0f);

    RenderUI();
}

void InputMapping::Exit() {

    AEGfxMeshFree(pMesh);
    AEGfxDestroyFont(pText);

    rapidjson::StringBuffer buffer;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);

    std::ofstream file("input_map.json");

    writer.StartObject();
    writer.Key("MoveLeft");
    writer.Int(keyBindings[0].key);

    writer.Key("MoveRight");
    writer.Int(keyBindings[1].key);

    writer.Key("Jump");
    writer.Int(keyBindings[2].key);

    //writer.Key("PowerUp");
    //writer.Int(keyBindings[3].key);
    
    writer.EndObject();

    if (file.is_open()) {
        file << buffer.GetString();
        file.close();
        std::cout << "Input map saved to: input_map.json" << std::endl;
    }

    Object2D::Exit();
}
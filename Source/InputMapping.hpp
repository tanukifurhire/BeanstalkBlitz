/* Start Header ************************************************************************/
/*!
\file		InputMapping.hpp
\author		Tok Duan Jie Nelson
\par		t.duanjienelson@digipen.edu, 
\date		09/03/2025
\brief		This file implements the declaration of functions needed for input mapping 

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#pragma once
#include "Object2D.hpp"

struct KeyBinding {
    std::string action{"ACTION"};
    u8 key{AEVK_SPACE};
};

class InputMapping : public Object2D {
public:
	InputMapping(std::shared_ptr<Object> parent = nullptr);
    

	void Enter() override;

	void PreUpdate() override;

	void Update() override;

	void PostUpdate() override;

	void Exit() override;

    bool SettingState = false; // Check if the user is in the InputMapping state

    // Type of Control keys for the player
    std::array<KeyBinding, 3> keyBindings;

protected:
	AEGfxVertexList* pMesh = nullptr;   // Mesh for rendering
    s8 pText = 0;                       // text for rendering
    int selectedOption = 0;             // Tracks which action is selected for rebinding
    bool awaitingKeyPress = false;      // When true, the game is waiting for a new key to be input by user
    bool keyConflict = false;           // Check for conflicting keys selected

    // KeyNames display to be use in GetKeyName fucntion
    std::unordered_map<u8, std::string> keyNameMap = {
        // Mouse buttons
        {static_cast<u8>(AEVK_LBUTTON), "Left Mouse Button"}, {static_cast<u8>(AEVK_RBUTTON), "Right Mouse Button"}, {static_cast<u8>(AEVK_MBUTTON), "Middle Mouse Button"},

        // Special keys
        {static_cast<u8>(AEVK_BACK), "Backspace"}, {static_cast<u8>(AEVK_TAB), "Tab"}, {static_cast<u8>(AEVK_RETURN), "Enter"}, {static_cast<u8>(AEVK_LSHIFT), "L-Shift"},
        {static_cast<u8>(AEVK_RSHIFT), "R-Shift"}, {static_cast<u8>(AEVK_LCTRL), "L-Ctrl"}, {static_cast<u8>(AEVK_RCTRL), "R-Ctrl"}, {static_cast<u8>(AEVK_LALT), "L-Alt"},
        {static_cast<u8>(AEVK_RALT), "R-Alt"}, {static_cast<u8>(AEVK_PRINTSCREEN), "Print Screen"}, {static_cast<u8>(AEVK_SCROLLLOCK), "Scroll Lock"},
        {static_cast<u8>(AEVK_PAUSE), "Pause"}, {static_cast<u8>(AEVK_CAPSLOCK), "Caps Lock"}, {static_cast<u8>(AEVK_ESCAPE), "Esc"},

        // Navigation keys
        {static_cast<u8>(AEVK_SPACE), "Space"}, {static_cast<u8>(AEVK_PAGEUP), "Page Up"}, {static_cast<u8>(AEVK_PAGEDOWN), "Page Down"}, {static_cast<u8>(AEVK_END), "End"},
        {static_cast<u8>(AEVK_HOME), "Home"}, {static_cast<u8>(AEVK_LEFT), "Left Arrow"}, {static_cast<u8>(AEVK_UP), "Up Arrow"}, {static_cast<u8>(AEVK_RIGHT), "Right Arrow"},
        {static_cast<u8>(AEVK_DOWN), "Down Arrow"}, {static_cast<u8>(AEVK_INSERT), "Insert"}, {static_cast<u8>(AEVK_DELETE), "Delete"},

        // Numeric keys
        {static_cast<u8>(AEVK_0), "0"}, {static_cast<u8>(AEVK_1), "1"}, {static_cast<u8>(AEVK_2), "2"}, {static_cast<u8>(AEVK_3), "3"}, {static_cast<u8>(AEVK_4), "4"},
        {static_cast<u8>(AEVK_5), "5"}, {static_cast<u8>(AEVK_6), "6"}, {static_cast<u8>(AEVK_7), "7"}, {static_cast<u8>(AEVK_8), "8"}, {static_cast<u8>(AEVK_9), "9"},

        // Alphabet keys
        {static_cast<u8>(AEVK_A), "A"}, {static_cast<u8>(AEVK_B), "B"}, {static_cast<u8>(AEVK_C), "C"}, {static_cast<u8>(AEVK_D), "D"}, {static_cast<u8>(AEVK_E), "E"},
        {static_cast<u8>(AEVK_F), "F"}, {static_cast<u8>(AEVK_G), "G"}, {static_cast<u8>(AEVK_H), "H"}, {static_cast<u8>(AEVK_I), "I"}, {static_cast<u8>(AEVK_J), "J"},
        {static_cast<u8>(AEVK_K), "K"}, {static_cast<u8>(AEVK_L), "L"}, {static_cast<u8>(AEVK_M), "M"}, {static_cast<u8>(AEVK_N), "N"}, {static_cast<u8>(AEVK_O), "O"},
        {static_cast<u8>(AEVK_P), "P"}, {static_cast<u8>(AEVK_Q), "Q"}, {static_cast<u8>(AEVK_R), "R"}, {static_cast<u8>(AEVK_S), "S"}, {static_cast<u8>(AEVK_T), "T"},
        {static_cast<u8>(AEVK_U), "U"}, {static_cast<u8>(AEVK_V), "V"}, {static_cast<u8>(AEVK_W), "W"}, {static_cast<u8>(AEVK_X), "X"}, {static_cast<u8>(AEVK_Y), "Y"},
        {static_cast<u8>(AEVK_Z), "Z"},

        // Numpad keys
        {static_cast<u8>(AEVK_NUMPAD0), "Numpad 0"}, {static_cast<u8>(AEVK_NUMPAD1), "Numpad 1"}, {static_cast<u8>(AEVK_NUMPAD2), "Numpad 2"}, {static_cast<u8>(AEVK_NUMPAD3), "Numpad 3"},
        {static_cast<u8>(AEVK_NUMPAD4), "Numpad 4"}, {static_cast<u8>(AEVK_NUMPAD5), "Numpad 5"}, {static_cast<u8>(AEVK_NUMPAD6), "Numpad 6"}, {static_cast<u8>(AEVK_NUMPAD7), "Numpad 7"},
        {static_cast<u8>(AEVK_NUMPAD8), "Numpad 8"}, {static_cast<u8>(AEVK_NUMPAD9), "Numpad 9"}, {static_cast<u8>(AEVK_NUM_MULTIPLY), "Numpad *"}, {static_cast<u8>(AEVK_NUM_PLUS), "Numpad +"},
        {static_cast<u8>(AEVK_NUM_MINUS), "Numpad -"}, {static_cast<u8>(AEVK_NUM_PERIOD), "Numpad ."}, {static_cast<u8>(AEVK_NUM_DIVIDE), "Numpad /"}, {static_cast<u8>(AEVK_NUMLOCK), "Num Lock"},

        // Function keys
        {static_cast<u8>(AEVK_F1), "F1"}, {static_cast<u8>(AEVK_F2), "F2"}, {static_cast<u8>(AEVK_F3), "F3"}, {static_cast<u8>(AEVK_F4), "F4"}, {static_cast<u8>(AEVK_F5), "F5"},
        {static_cast<u8>(AEVK_F6), "F6"}, {static_cast<u8>(AEVK_F7), "F7"}, {static_cast<u8>(AEVK_F8), "F8"}, {static_cast<u8>(AEVK_F9), "F9"}, {static_cast<u8>(AEVK_F10), "F10"},
        {static_cast<u8>(AEVK_F11), "F11"}, {static_cast<u8>(AEVK_F12), "F12"},

        // Symbol keys
        {static_cast<u8>(AEVK_SEMICOLON), ";"}, {static_cast<u8>(AEVK_SLASH), "/"}, {static_cast<u8>(AEVK_BACKQUOTE), "`"}, {static_cast<u8>(AEVK_LBRACKET), "["},
        {static_cast<u8>(AEVK_BACKSLASH), "\\"}, {static_cast<u8>(AEVK_RBRACKET), "]"}, {static_cast<u8>(AEVK_QUOTE), "'"},

        // Additional symbol keys
        {static_cast<u8>(AEVK_EQUAL), "="}, {static_cast<u8>(AEVK_MINUS), "-"}, {static_cast<u8>(AEVK_PERIOD), "."}, {static_cast<u8>(AEVK_COMMA), ","}
    }; 
    // End keyNameMap class

    // return the string in the second index, else return "Unknown" for keyNameMap if the new input is not included in keyNameMap class
    std::string GetKeyName(u8 key) {
        auto it = keyNameMap.find(key);
        return (it != keyNameMap.end()) ? it->second : "Unknown";
    }

    // This function update the key binding
    void ChangeKeyBinding(const std::string& action, u8 newKey);

    /* Debugging tool
    // Get key binding by action name (Debugging tool)
    u8 GetKeyBinding(const std::string& action);
    
    // Check if a key is currently pressed (Debugging tool)
    bool IsKeyPressed(const std::string& action);
    */

    // This function check for similar Key used
    bool CheckSameKey(u8 newKey);
 

    // This function handles the rendering of text of InputMapping
    void RenderUI();

    // This function handles the toggling  of selection between each input
    void HandleInput();
};
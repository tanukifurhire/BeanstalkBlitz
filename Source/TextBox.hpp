/* Start Header ************************************************************************/
/*!
\file		TextBox.hpp
\author		Tai Xiao Xuan
\par		xiaoxuan.tai@digipen.edu
\date		09/03/2025
\brief		This file implements a base TextBox class, that allows for editable text fields at runtime.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#pragma once
#include "Button.hpp"

class TextBox : public Button {
public:
	TextBox(std::shared_ptr<Object> parent = nullptr) : Button(std::move(parent)) {}

	void Update() override;

	void PostUpdate() override;

	void SetDefaultText(std::string text_val) {
		default_text = text_val;
	}

protected:
	std::string default_text = "Player";
};
/* Start Header ************************************************************************/
/*!
\file	    VictoryScreen.hpp
\author		Tai Xiao Xuan
\par		xiaoxuan.tai@digipen.edu
\date		30/03/2025
\brief		This file declares a Victory Screen UI class

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#pragma once
#include "ErrorButton.hpp"

class VictoryScreen : public ErrorButton {
public:
	VictoryScreen(std::shared_ptr<Object> parent = nullptr) : ErrorButton(std::move(parent)) {}

	void Update() override;

	void PostUpdate() override;

protected:

};
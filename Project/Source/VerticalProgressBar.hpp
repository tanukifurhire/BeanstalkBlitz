/* Start Header ************************************************************************/
/*!
\file	    VerticalProgressBar.cpp
\author		Tai Xiao Xuan
\par		xiaoxuan.tai@digipen.edu
\date		30/03/2025
\brief		This file declares a Vertical Progress Bar UI class

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#pragma once
#include "Rect2D.hpp"

class VerticalProgressBar : public Rect2D {
public:
    VerticalProgressBar(std::shared_ptr<Object> parent = nullptr) : Rect2D(std::move(parent)) {}

    void Enter() override;

    void PreUpdate() override;

    void Update() override;

    void PostUpdate() override;

    void Exit() override;

protected:
};

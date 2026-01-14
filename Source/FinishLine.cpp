/* Start Header ************************************************************************/
/*!
\file		FinishLine.cpp
\author		Tai Xiao Xuan
\par		xiaoxuan.tai@digipen.edu
\date		30/03/2025
\brief		This file defines functions for a simple Finish Line object

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#include "GameDefn.hpp"

void FinishLine::Update() {
	Object2D::Update();
	Object2D::PreUpdate();
}
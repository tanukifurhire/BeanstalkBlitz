/* Start Header ************************************************************************/
/*!
\file		FinishLine.hpp
\author		Tai Xiao Xuan
\par		xiaoxuan.tai@digipen.edu
\date		30/03/2025
\brief		This file declares a simple Finish Line object

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#pragma once
#include "RectPlayer.hpp"

class FinishLine : public Object2D {
public:
	FinishLine(std::shared_ptr<Object> parent = nullptr) : Object2D(std::move(parent)) {}

	void Update() override;

protected:

};
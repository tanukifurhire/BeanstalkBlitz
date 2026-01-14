/* Start Header ************************************************************************/
/*!
\file		Tutorial.hpp
\author		Tok Duan Jie Nelson 
\par		t.duanjienelson@digipen.edu
\date		30/03/2025
\brief		This file implements the declaration of functions needed for tutorial

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#pragma once
#include "Object2D.hpp"

class Tutorial : public Object2D {
public:
	Tutorial(std::shared_ptr<Object> parent = nullptr);


	void Enter() override;

	void PreUpdate() override;

	void Update() override;

	void PostUpdate() override;

	void Exit() override;

	bool TutorialState = false; // Check if the user is in the InputMapping state

protected:
	AEGfxVertexList* pMesh = nullptr;   // Mesh for rendering
	s8 pText = 0;                       // text for rendering
	f32 text_y = 0.04f;					// offset for y
	f32 x_offset = position.x;			// for turning page

	void HandleInput();
};
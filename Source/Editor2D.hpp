/* Start Header ************************************************************************/
/*!
\file		Editor.hpp
\author		Tai Xiao Xuan
\par		xiaoxuan.tai@digipen.edu
\date		09/03/2025
\brief		This file Implements functions for an Editor User Interface to inspect the Object hierarchy and individual objects

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#pragma once
#include "Object2D.hpp"

class Editor2D : public Object2D {
public: 
    Editor2D(std::shared_ptr<Object> parent = nullptr);

    void Enter() override;

    void PreUpdate() override;

    void Update() override;

    void PostUpdate() override;

    void Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const override;

    void Exit() override;

    void Hierarchy();

    bool enable = false;

protected:

};

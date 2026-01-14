/* Start Header ************************************************************************/
/*!
\file		AudioDriver.hpp
\author		Tai Xiao Xuan
\par		xiaoxuan.tai@digipen.edu
\date		06/04/2025
\brief		This file defines an AudioDriver class, that stores all the audio files for this
			game, including Sound Effects and Music.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#pragma once
#include "Object.hpp"

class AudioDriver : public Object {
public:
	AudioDriver(std::shared_ptr<Object> parent = nullptr) : Object(std::move(parent)) {}

	void Enter() override;

	void Exit() override;

	AEAudioGroup sfx_group{};

	AEAudioGroup music_group{};

	AEAudio menu{};
	AEAudio game{};
	AEAudio victory{};
	AEAudio defeat{};

	AEAudio confirm{};
	AEAudio cancel{};
	AEAudio hit{};
	AEAudio jump{};
	AEAudio run{};
	AEAudio step_leaf{};
	AEAudio charging{};
	AEAudio boosting{};
};
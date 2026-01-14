/* Start Header ************************************************************************/
/*!
\file		AudioDriver.cpp
\author		Tai Xiao Xuan
\par		xiaoxuan.tai@digipen.edu
\date		06/04/2025
\brief		This file implements a AudioDriver class, that stores all the audio files for this
			game, including Sound Effects and Music.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#include "GameDefn.hpp"

void AudioDriver::Enter() {
	menu = AEAudioLoadMusic("Assets/Bossa Antigua.mp3");
	game = AEAudioLoadMusic("Assets/Monkeys Spinning Monkeys.mp3");
	victory = AEAudioLoadMusic("Assets/Happy Bee.mp3");
	defeat = AEAudioLoadMusic("Assets/Oppressive Gloom.mp3");

	confirm = AEAudioLoadSound("Assets/Comfirm button.wav");
	cancel = AEAudioLoadSound("Assets/Cancel button.wav");
	hit = AEAudioLoadSound("Assets/hit.wav");
	jump = AEAudioLoadSound("Assets/jump.wav");
	run = AEAudioLoadSound("Assets/run.wav");
	step_leaf = AEAudioLoadSound("Assets/step leaf.wav");
	charging = AEAudioLoadSound("Assets/charging.wav");
	boosting = AEAudioLoadSound("Assets/Boost.wav");

	sfx_group = AEAudioCreateGroup();

	music_group = AEAudioCreateGroup();

	Object::Enter();
}

void AudioDriver::Exit() {
	AEAudioUnloadAudio(confirm);
	AEAudioUnloadAudio(cancel);
	AEAudioUnloadAudio(hit);
	AEAudioUnloadAudio(jump);
	AEAudioUnloadAudio(run);
	AEAudioUnloadAudio(step_leaf);
	AEAudioUnloadAudio(charging);
	AEAudioUnloadAudio(boosting);

	AEAudioUnloadAudio(menu);
	AEAudioUnloadAudio(game);
	AEAudioUnloadAudio(victory);
	AEAudioUnloadAudio(defeat);

	AEAudioUnloadAudioGroup(sfx_group);
	AEAudioUnloadAudioGroup(music_group);

	Object::Exit();
}
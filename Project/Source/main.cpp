/* Start Header ************************************************************************/
/*!
\file		main.cpp
\author		Tai Xiao Xuan
\par		xiaoxuan.tai@digipen.edu
\date		09/03/2025
\brief		This file implements the main entry point of Beanstalk Blitz, and defines the default mesh/display fonts for the project.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

// ---------------------------------------------------------------------------
// includes

#define _GLIBCXX_USE_NANOSLEEP
#include <crtdbg.h> // To check for memory leaks
#include "GameDefn.hpp"
#include "AIPlayer.hpp"	//have to do this cause circular dependencies prevent compiling
#include <memory>
#include <thread>
#include <WinUser.h>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include "windows.h"

// ---------------------------------------------------------------------------
// main

s32 cursor_delta_x = 0;
s32 cursor_delta_y = 0;

s32 cursor_pos_world_x = 0;
s32 cursor_pos_world_y = 0;

s8 default_font;
AEGfxVertexList* default_mesh = nullptr;
AEGfxTexture* default_tex = nullptr;
AEGfxTexture* stun_tex = nullptr;
s32 default_color = 0x00000000;

f32 screen_width = 0.0f;
f32 screen_height = 0.0f;

f32 deltaTime = 0.0f;

std::weak_ptr<Object> editor_picked_object;
std::weak_ptr<Object> editor_hovered_object;
std::weak_ptr<Object> editor_inspected_object;

std::weak_ptr<Editor2D> editor;

bool game_running = true;

void LoadDefaults() {
	default_font = AEGfxCreateFont("Assets/SourceCodePro-Black.ttf", 50);

	default_tex = AEGfxTextureLoad("Assets/Rocket.png");

	stun_tex = AEGfxTextureLoad("Assets/Smack.png");

	AEGfxMeshStart();

	AEGfxTriAdd(0.0f, 0.0f, default_color, 0.0f, 1.0f,
		1.0f, 0.0f, default_color, 1.0f, 1.0f,
		0.0f, 1.0f, default_color, 0.0f, 0.0f);

	AEGfxTriAdd(1.0f, 0.0f, default_color, 1.0f, 1.0f,
		1.0f, 1.0f, default_color, 1.0f, 0.0f,
		0.0f, 1.0f, default_color, 0.0f, 0.0f);

	default_mesh = AEGfxMeshEnd();
}

void FreeDefaults() {
	AEGfxDestroyFont(default_font);
	AEGfxMeshFree(default_mesh);
	AEGfxDestroyFont(default_font);
	AEGfxTextureUnload(default_tex);
	AEGfxTextureUnload(stun_tex);
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// 849, 847, 846, 845, 844, 843, 842, 841, 840
	//_CrtSetBreakAlloc(2402);

	(void)nCmdShow;

	// Initialization of your own variables go here

	#ifndef _SERVER
#ifndef _DEBUG
		// Using custom window procedure
		AESysInit(hInstance, 1, 1600, 900, 0, 144, false, NULL);
#else
		// Using custom window procedure
		AESysInit(hInstance, 1, 1600, 900, 0, 144, false, NULL);
#endif	
	#else
		// Using custom window procedure
		AESysInit(hInstance, 0, 0, 0, 1, 144, false, NULL);
	#endif

	// reset the system modules
	AESysReset();

	AESysSetWindowTitle("Beanstalk Blitz");

	AESysSetWindowIcon("Assets/jackedjack_icon.ico", 48, 48);

	screen_width = static_cast<f32>(AEGfxGetWindowWidth());
	screen_height = static_cast<f32>(AEGfxGetWindowHeight());

	#ifndef _SERVER
		GetRoot()->Enter();
		BaseScene();
	#else
		GetNetworkServer()->Enter();
	#endif

	AEInputShowCursor(0);

	ShowCursor(true);

	LoadDefaults();

	// Game Loop
	while (game_running && AESysDoesWindowExist())
	{
		// Informing the system about the loop's start
		AESysFrameStart();

		screen_width = static_cast<f32>(AEGfxGetWindowWidth());
		screen_height = static_cast<f32>(AEGfxGetWindowHeight());

		s32 cursor_x{ 0 };
		s32 cursor_y{ 0 };

		AEInputGetCursorPosition(&cursor_x, &cursor_y);

		cursor_pos_world_x = (cursor_x - AEGfxGetWindowWidth() / 2);
		cursor_pos_world_y = -(cursor_y - AEGfxGetWindowHeight() / 2);

		#ifndef _SERVER
			// Pre-Update logic goes here
			if (!editor.lock()->enable) {
				deltaTime = static_cast<f32>(AEFrameRateControllerGetFrameTime());

				GetRoot()->PreUpdate();
			}

			GetNetworkClient()->Update();

			// Your own update logic goes here
			if (!editor.lock()->enable) {
				GetRoot()->Update();
			}

			GetLobbyContainer()->Update();

			// Your own rendering logic goes here
			GetRoot()->PostUpdate();
		#else
			GetNetworkServer()->Update();
		#endif

		// Informing the system about the loop's end
		AESysFrameEnd();

	}

	SaveHierarchy();

	Static_Exit();

	FreeDefaults();

	// free the system
	AESysExit();
}
/* Start Header ************************************************************************/
/*!
\file		GameDefn.hpp
\author		Tai Xiao Xuan, Vu Phan Hung, Xavier Seet Long Chuan, Tan Chun Kwang Xavier, Nelson Tok Duan Jie
\par		xiaoxuan.tai@digipen.edu, phanhung.vu@digipen.edu, x.seet@digipen.edu, chunkwangxavier.tan@digipen.edu, t.duanjienelson@digipen.edu
\date		09/03/2025
\brief		This file includes all header files and extern variables for other classes to use, as well as some Networking enums. 
			Tai Xiao Xuan - 60%
			Vu Phan Hung - 10%
			Xavier Seet Long Chuan - 10%
			Tan Chun Kwang Xavier - 10%
			Nelson Tok Duan Jie - 10%

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#pragma once
#define PORT 9080
#define MAX_BUFFER 1048

#include <functional>

#include "Root.hpp"
#include "Object.hpp"
#include "Object2D.hpp"
#include "Editor2D.hpp"
#include "ImageRenderer2D.hpp"
#include "Rect2D.hpp"
#include "Text2D.hpp"
#include "Map.hpp"
#include "Camera2D.hpp"
#include "RectPlayer.hpp"
#include "EnemyHeaderClass.hpp"
#include "EnemyBasic1.hpp"
#include "EnemyBasic2.hpp"
#include "EnemyBasic3.hpp"
#include "EnemyBoss.hpp"
#include "StateMachine.hpp"
#include "State.hpp"
#include "Player.hpp"
#include "AIPlayer.hpp"
#include "NetworkingTest.hpp"
#include "Button.hpp"
#include "Event.hpp"
#include "UIContainer.hpp"
#include "ErrorButton.hpp"
#include "TextBox.hpp"
#include "InputManager.hpp"
#include "MapContainer.hpp"
#include "PlayerManager.hpp"
#include "InputMapping.hpp"
#include "FinishLine.hpp"
#include "PieTimer.hpp"
#include "SpriteRenderer.hpp"
#include "VerticalProgressBar.hpp"
#include "Credits.hpp"
#include "CountdownText.hpp"
#include "VictoryScreen.hpp"
#include "Tutorial.hpp"
#include "AudioDriver.hpp"
#include "DigipenLogo.hpp"
#include "PauseMenu.hpp"

extern bool game_running;

extern s8 default_font;
extern AEGfxVertexList* default_mesh;
extern AEGfxTexture* default_tex;
extern AEGfxTexture* stun_tex;
extern s32 default_color;
extern f32 screen_width;
extern f32 screen_height;
extern s32 cursor_pos_world_x;
extern s32 cursor_pos_world_y;
extern f32 deltaTime;

extern std::weak_ptr<Object> editor_picked_object;
extern std::weak_ptr<Object> editor_hovered_object;
extern std::weak_ptr<Object> editor_inspected_object;

extern std::weak_ptr<Editor2D> editor;

enum ObjectType {
	OBJECT,
	OBJECT2D,
	EDITOR2D,
	RECT2D,
	RECTPLAYER,
	TEXT2D,
	IMAGERENDERER2D,
	CAMERA2D
};

enum RPCCall {
	CONNECT,
	ONPLAYERCONNECTED,
	UPDATE_CLIENT,
	UPDATE_SERVER,
	CREATE_LOBBY,
	RETURN_CREATE_LOBBY,
	REQUEST_LOBBIES,
	RETURN_REQUEST_LOBBIES,
	JOIN_LOBBY,
	RETURN_JOIN_LOBBY,
	DISCONNECT,
	ON_DISCONNECTED,
	LEAVE_LOBBY,
	RETURN_LEAVE_LOBBY,
	REQUEST_LOBBY_DETAILS,
	RETURN_REQUEST_LOBBY_DETAILS,
	PING,
	RETURN_PING,
	REFRESH_CLIENTS,
	REFRESH_LOBBY_CLIENTS,
	ON_READY,
	ON_RETURN_START_GAME,
	UPDATE_PLAYER,
	RETURN_PLAYER_UPDATE
};



extern std::weak_ptr<Object> editor_inspected_object;

extern f32 camera_x;
extern f32 camera_y;

extern std::shared_ptr<NetworkClient> network_client;

extern Event test_event;

extern f32 world_boundary_x_min;
extern f32 world_boundary_x_max;

extern s8 current_layer;
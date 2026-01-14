/* Start Header ************************************************************************/
/*!
\file		Root.hpp
\author		Tai Xiao Xuan, Vu Phan Hung, Xavier Seet Long Chuan, Tan Chun Kwang Xavier, Nelson Tok Duan Jie
\par		xiaoxuan.tai@digipen.edu, phanhung.vu@digipen.edu, x.seet@digipen.edu, chunkwangxavier.tan@digipen.edu, t.duanjienelson@digipen.edu
\date		09/03/2025
\brief		This file implements the Base Scene as well as Static members for the game, and handling UI scene callbacks
			Each member handled their respective functions for the features they were responsible for, as such 
			20% contribution for each member. 

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#pragma once
#include <memory>
#include "Object.hpp"
#include "NetworkingTest.hpp"
#include "Camera2D.hpp"
#include "Player.hpp"
#include "UIContainer.hpp"
#include "ImageRenderer2D.hpp"
#include "TextBox.hpp"
#include "MapContainer.hpp"
#include "PlayerManager.hpp"
#include "Credits.hpp"
#include "CountdownText.hpp"
#include "VictoryScreen.hpp"
#include "AudioDriver.hpp"
#include "PauseMenu.hpp"

void Static_Exit();

std::shared_ptr<Object2D>& VictoryZone();

std::shared_ptr<Object>& GetRoot();

std::shared_ptr<NetworkClient>& GetNetworkClient();

std::shared_ptr<NetworkServer>& GetNetworkServer();

std::shared_ptr<LobbyContainer>& GetLobbyContainer();

std::shared_ptr<LobbyDetails>& GetLobbyDetails();

std::shared_ptr<Camera2D>& GetCamera();

std::shared_ptr<Player>& GetPlayer();

std::shared_ptr<UIContainer>& GetMultiplayerUIContainer();

std::shared_ptr<UIContainer>& GetMainMenuUIContainer();

std::shared_ptr<UIContainer>& GetLobbyUIContainer();

std::shared_ptr<UIContainer>& GetCurrentLobbyUI();

std::shared_ptr<UIContainer>& GetSettingUIContainer();

std::shared_ptr<UIContainer>& GetTutorialUIContainer();

std::shared_ptr<TextBox>& GetPlayerName();

std::shared_ptr<Object2D>& GetUIBranch();

std::shared_ptr<UIContainer>& GetErrorUIContainer();

std::shared_ptr<VictoryScreen>& GetVictoryScreen();

std::shared_ptr<ImageRenderer2D>& GetBrickUILeft();

std::shared_ptr<ImageRenderer2D>& GetBrickUIRight();

std::shared_ptr<ImageRenderer2D>& GetClouds();

std::shared_ptr<ImageRenderer2D>& GetBeanstalk();

std::shared_ptr<PlayerManager>& GetPlayerBranch();

std::shared_ptr<Object2D>& GetWorldBranch();

std::shared_ptr<MapContainer>& GetMapContainer();

std::shared_ptr<Credits>& GetCreditUIContainer();

std::shared_ptr<CountdownText>& GetCountdownTimer();

std::shared_ptr<AudioDriver>& GetAudioDriver();

std::shared_ptr<PauseMenu>& GetPauseMenu();

void CreateLobbyUI(std::string name, int lobby_id, int num_players, int num_spectators, f32 x, f32 y);

void SaveHierarchy();

void LoadHierarchy(std::string file_name);

void BaseScene();
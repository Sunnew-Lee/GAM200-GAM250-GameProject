/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Level1.cpp
Project: BIT_SAVER
Author:
Creation date: 3/07/2021
-----------------------------------------------------------------*/
#include "../Engine/Engine.h"	//GetGameStateManage
#include"Level1.h"
#include"..\Objects\Hero.h"
#include"..\Objects\Track.h"
#include"..\Objects\Notes.h"
#include"..\Objects\Boss.h"
#include"..\Objects\Note_collisionBox.h"
#include"..\Objects\Background.h"
#include"../Levels/State.h"
#include "..\Objects\EnergyBar.h"
#include "..\Objects\stage_bar.h"
#include"../../Engine/Sprite/GameParticles.h"

enum class STATE
{
	EXTRA,
	GENERATING,
	OPTION,
	FINISH
};


Level1::Level1() : 
mainMenu(InputKey::Keyboard::Escape), 
optionMenu(InputKey::Keyboard::O),
isOption(false),
camera({ 0,0 })
{
	//camera = glm::vec2{ 0,0 };
	gameObjectManager = nullptr;
	heroPtr = nullptr;
	trackPtr = nullptr;
	bossPtr = nullptr;
	notebox = nullptr;
	backPtr = nullptr;
	energyBar = nullptr;
	stageBar = nullptr;
	curr_state = 0;
	combo = 0;
}

void Level1::Load()
{
	Engine::GetMusic().Play(Music::SOUND_NUM::REWIND);
	gameObjectManager = new GameObjectManager();

	heroPtr = new Hero({ -4,0 });
	backPtr = new Background();
	trackPtr = new Track(Music::SOUND_NUM::REWIND);
	notebox = new Note_box({ -4,0 });
	bossPtr = new Boss({ 12,0 });
	energyBar = new EnergyBar({ -4,1.2 });
	stageBar = new Stage_bar({ -10,9 },204,82);   // total music time 204  ,  extra time 82

	backPtr->Add("../images/background1.png", 0);
	backPtr->Add("../images/parallax1-5.png", 0.5);
	backPtr->Add("../images/parallax1-4.png", 0.8);
	backPtr->Add("../images/parallax1-3.png", 1.1);
	backPtr->Add("../images/parallax1-2.png", 1.3);
	backPtr->Add("../images/parallax1-1.png", 1.5);


	AddGSComponent(gameObjectManager);
	AddGSComponent(backPtr);

	gameObjectManager->Add(heroPtr);
	gameObjectManager->Add(bossPtr);
	gameObjectManager->Add(notebox);
	gameObjectManager->Add(trackPtr);
	gameObjectManager->Add(energyBar);
	gameObjectManager->Add(stageBar);

	AddGSComponent(new HitEmitter());
}

void Level1::Update(double dt)
{
    if (isOption == false && optionMenu.IsKeyDown() == true)
    {
	Engine::GetMusic().Pause(Music::SOUND_NUM::REWIND);

    }
    if (isOption == false && optionMenu.IsKeyReleased() == true)
    {
	isOption = true;
    }
    else if (isOption == true && optionMenu.IsKeyDown() == true)
    {
	isOption = false;
	Engine::GetMusic().Resume(Music::SOUND_NUM::REWIND);

    }
    if (isOption != true)
    {
	GetGSComponent<Background>()->Update(dt);
	gameObjectManager->UpdateAll(dt);
    }
	//camera.Dynamic_movement(notebox->GetDestroyed(),dt);
	//camera.Update({ 0,0 },dt);

	if (mainMenu.IsKeyReleased() == true)
	{
		Engine::GetGameStateManager().Shutdown();
	}
	if (stageBar->Getchangeflag() == 1)
	{
		curr_state = static_cast<int>(STATE::GENERATING);
		Engine::GetMusic().Pause(Music::SOUND_NUM::REWIND);
		bossPtr->GenerateBoss();
		trackPtr->SetUpdate(false);
		stageBar->SetUpdate(false);
		Engine::GetMusic().Play(Music::SOUND_NUM::BOSS_ENTRANCE);
		Engine::GetMusic().volumeUp(Music::SOUND_NUM::BOSS_ENTRANCE);
	}
	if (curr_state == static_cast<int>(STATE::GENERATING)&&bossPtr->GetPosition().x == 8)
	{
		Engine::GetMusic().Resume(Music::SOUND_NUM::REWIND);
		Engine::GetMusic().pitchUp(Music::SOUND_NUM::REWIND);
		trackPtr->SetUpdate(true);
		stageBar->SetUpdate(true);
		curr_state = static_cast<int>(STATE::FINISH);
	}

}

void Level1::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(1.0f, 0.5f, 1.0f, 1.0f);
	GetGSComponent<Background>()->Draw(camera.GetMatrix());
	gameObjectManager->DrawAll(camera.GetMatrix());
}

void Level1::Unload()
{
	heroPtr = nullptr;
	trackPtr = nullptr;
	notebox = nullptr;
	bossPtr = nullptr;
	backPtr = nullptr;
	energyBar = nullptr;
	stageBar = nullptr;
	Engine::GetMusic().Stop(Music::SOUND_NUM::ENERGY);
	ClearGSComponent();
	gameObjectManager->Unload();
}

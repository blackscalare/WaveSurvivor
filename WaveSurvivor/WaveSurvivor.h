#pragma once
#include "Renderer.h"
#include "Definitions.h"
#include "GameHandler.h"
#include "LevelUpScreenHandler.h"
#include "GUI.h"
#include "Tools.h"
#include "TextureHandler.h"
#include "MainMenu/MainMenu.h"
#include "EventHandler.h"
#include "Menus/OptionsMenu/OptionsMenu.h"

class WaveSurvivor
{
public:
	WaveSurvivor();
	~WaveSurvivor();

	int Run();
private:
	void Init();
	void MainMenuButtonCallback(int buttonId);
	void LevelUpScreenCardCallback();
	void GameOverCallback();

	Renderer* renderer;
	GameHandler* gameHandler;
	LevelUpScreenHandler* levelUpScreenHandler;
	TextureHandler* textureHandler;
	MainMenu* mainMenu;
	OptionsMenu* optionsMenu;
	EventHandler* eventHandler;
};


#pragma once
#include "Renderer.h"
#include "Definitions.h"
#include "GameHandler.h"
#include "LevelUpScreenHandler.h"
#include "GUI.h"
#include "Tools.h"
#include "TextureHandler.h"
#include "MainMenu/MainMenu.h"

class WaveSurvivor
{
public:
	WaveSurvivor();
	~WaveSurvivor();

	int Run();
private:
	void Init();
	void MainMenuButtonCallback(int buttonId);

	Renderer* renderer;
	GameHandler* gameHandler;
	LevelUpScreenHandler* levelUpScreenHandler;
	TextureHandler* textureHandler;
	MainMenu* mainMenu;
};

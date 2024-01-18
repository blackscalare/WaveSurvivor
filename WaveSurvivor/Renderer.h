#pragma once
#include "Definitions.h"
#include "GameHandler.h"
#include "LevelUpScreenHandler.h"
#include "GUI.h"
#include "Tools.h"
#include "TextureHandler.h"
#include "MainMenu/MainMenu.h"
#include "Menus/OptionsMenu/OptionsMenu.h"

class Renderer
{
public:
	Renderer(TextureHandler* textureHandler, GameHandler* gameHandler, LevelUpScreenHandler* levelUpScreenHandler, MainMenu* mainMenu, OptionsMenu* optionsMenu);
	~Renderer();

	void Render();
	void SetState(RenderState state);

private:
	void DrawObjects(std::vector<Object> objectsInViewport);
	void DrawEnemies(std::vector<Zombie*> enemiesInViewport);
	void DrawProjectiles(std::vector<Projectile*> projectilesInViewport);
	void DrawDebug();
	void DrawPlayer();
	void DrawGUI();
	void DrawBackground();
	void ReturnToMainMenu();
	void RenderGame();
	void RenderLevelUpScreen();
	void RenderMainMenu();
	void RenderOptions();

	RenderState currentState;
	GameHandler* gameHandler;
	LevelUpScreenHandler* levelUpScreenHandler;
	TextureHandler* textureHandler;
	MainMenu* mainMenu;
	OptionsMenu* optionsMenu;

	int centerX;
	int centerY;
	int framesCounter;
	int playerFramesSpeed;
	Rectangle playerFrameRec;
	int currentPlayerFrame;

	// TODO move shader logic
	int secondsLoc;
	int freqXLoc;
	int freqYLoc;
	int ampXLoc;
	int ampYLoc;
	int speedXLoc;
	int speedYLoc;

	float freqX = 25.0f;
	float freqY = 25.0f;
	float ampX = 5.0f;
	float ampY = 5.0f;
	float speedX = 8.0f;
	float speedY = 8.0f;

	float screenSize[2] = { (float)GetScreenWidth(), (float)GetScreenHeight() };
};


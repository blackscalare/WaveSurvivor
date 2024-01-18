#pragma once
#include "Definitions.h"
#include "GameHandler.h"
#include "LevelUpScreenHandler.h"
#include "GUI.h"
#include "Tools.h"
#include "TextureHandler.h"
#include "MainMenu/MainMenu.h"

class Renderer
{
public:
	Renderer(TextureHandler* textureHandler, GameHandler* gameHandler, LevelUpScreenHandler* levelUpScreenHandler, MainMenu* mainMenu);
	~Renderer();

	void Render();
	void SetState(RenderState state);

private:
	void DrawObjects(std::vector<Object> objectsInViewport);
	void DrawEnemies(std::vector<Zombie*> enemiesInViewport);
	void DrawProjectiles(std::vector<Position_f> projectilesInViewport);
	void DrawDebug();
	void DrawPlayer();
	void DrawGUI();
	void DrawBackground();
	void ReturnToMainMenu();
	void RenderGame();
	void RenderLevelUpScreen();
	void RenderMainMenu();

	RenderState currentState;
	GameHandler* gameHandler;
	LevelUpScreenHandler* levelUpScreenHandler;
	TextureHandler* textureHandler;
	MainMenu* mainMenu;

	int centerX;
	int centerY;
	int framesCounter;
	int playerFramesSpeed;
	Rectangle playerFrameRec;
	int currentPlayerFrame;

	int secondsLoc;
	int freqXLoc;
	int freqYLoc;
	int ampXLoc;
	int ampYLoc;
	int speedXLoc;
	int speedYLoc;
};


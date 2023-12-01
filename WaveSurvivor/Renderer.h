#pragma once
#include "Definitions.h"
#include "GameHandler.h"
#include "LevelUpScreenHandler.h"
#include "GUI.h"
#include "Tools.h"

class Renderer
{
public:
	Renderer();
	~Renderer();

	void Render();

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

	RenderState currentState;
	GameHandler* gameHandler;
	LevelUpScreenHandler* levelUpScreenHandler;
	int centerX;
	int centerY;
	int framesCounter;
	int playerFramesSpeed;
	Rectangle playerFrameRec;
	int currentPlayerFrame;
	Texture2D background;
	Texture2D playerTexture;
	Texture2D zombieTexture;
	Texture2D xpOrbTexture;
	Texture2D chestTexture;
};


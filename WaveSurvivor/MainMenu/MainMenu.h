#pragma once
#include "../Definitions.h"
#include "../TextureHandler.h"

class MainMenu
{
public:
	MainMenu(TextureHandler* textureHandler);
	void Update();
	std::vector<MenuButton*> GetButtons();

private:
	void Init();
	
	TextureHandler* textureHandler;

	Texture2D* buttonTexture;
	float frameHeight;
	Rectangle sourceRec;
	Vector2 mousePoint;
	std::vector<MenuButton*> buttons;
};
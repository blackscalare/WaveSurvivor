#pragma once
#include "../Definitions.h"
#include "../TextureHandler.h"
#include "../Tools.h"

class MainMenu
{
public:
	MainMenu(TextureHandler* textureHandler);
	void Update();
	std::vector<MenuButton*> GetButtons();
	void BindCallback(std::function<void(int)> fn)
	{
		//callback = std::bind(fn, std::placeholders::_1);
		callback = fn;
	}

private:
	void Init();
	
	TextureHandler* textureHandler;

	Texture2D* buttonTexture;
	float frameHeight;
	Rectangle sourceRec;
	Vector2 mousePoint;
	std::vector<MenuButton*> buttons;
	std::function<void(int)> callback;
};
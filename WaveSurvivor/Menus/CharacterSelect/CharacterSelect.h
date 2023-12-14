#pragma once
#include "../../Definitions.h"
#include "../../TextureHandler.h"

class CharacterSelect
{
public:
	CharacterSelect(TextureHandler* textureHandler);
	void Update();

private:
	void Init();

	TextureHandler* textureHandler;
	Texture2D* backgroundTexture;
	Texture2D* characterSelectBorderTexture;
	float frameHeight;
	Rectangle sourceRec;
	Vector2 mousePoint;
};
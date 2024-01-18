#pragma once
#include "Definitions.h"

class TextureHandler
{
public:
	TextureHandler();
	~TextureHandler();
	Texture2D* GetTexture(TextureName name);
	Shader* GetShader(ShaderName shader);

private:
	std::map<TextureName, Texture2D*> textures;
	Texture2D background;
	Texture2D playerTexture;
	Texture2D zombieTexture;
	Texture2D xpOrbTexture;
	Texture2D chestTexture;
	Texture2D cardTexture;
	Texture2D damageUpgradeTexture;
	Texture2D healthUpgradeTexture;
	Texture2D moveSpeedUpgradeTexture;
	Texture2D fireSpeedUpgradeTexture;
	Texture2D pickupUpgradeTexture;
	Texture2D thornAuraTexture;
	Texture2D buttonTexture;
	Texture2D mainMenuBackgroundTexture;
	Texture2D altMenuBackgroundTexture;
	Texture2D characterSelectBorderTexture;
	
	std::map<ShaderName, Shader*> shaders;
	Shader xpOrbShader;
};


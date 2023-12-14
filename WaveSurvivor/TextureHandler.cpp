#include "TextureHandler.h"

TextureHandler::TextureHandler()
{
	background						= LoadTexture("textures/mapTexture_huge.png");
	playerTexture					= LoadTexture("textures/playerAnimTexture.png");
	zombieTexture					= LoadTexture("textures/zombieTexture.png");
	xpOrbTexture					= LoadTexture("textures/xpOrbTexture.png");
	chestTexture					= LoadTexture("textures/chestTexture.png");
	cardTexture						= LoadTexture("textures/cardTexture_biggest.png");
	damageUpgradeTexture			= LoadTexture("textures/damageUpgradeTexture.png");
	healthUpgradeTexture			= LoadTexture("textures/healthUpgradeTexture.png");
	moveSpeedUpgradeTexture			= LoadTexture("textures/moveSpeedUpgradeTexture.png");
	fireSpeedUpgradeTexture			= LoadTexture("textures/fireSpeedUpgradeTexture.png");
	pickupUpgradeTexture			= LoadTexture("textures/pickupUpgradeTexture.png");
	thornAuraTexture				= LoadTexture("textures/thornAuraTexture.png");
	buttonTexture					= LoadTexture("textures/buttonTexture.png");
	mainMenuBackgroundTexture		= LoadTexture("textures/mainMenuBackgroundTexture.png");
	altMenuBackgroundTexture		= LoadTexture("textures/altMenuBackground.png");
	characterSelectBorderTexture	= LoadTexture("textures/characterSelectBorder.png");
	
	textures.insert(std::make_pair(BACKGROUND_TEXTURE, &background));
	textures.insert(std::make_pair(PLAYER_TEXTURE, &playerTexture));
	textures.insert(std::make_pair(ZOMBIE_TEXTURE, &zombieTexture));
	textures.insert(std::make_pair(XP_ORB_TEXTURE, &xpOrbTexture));
	textures.insert(std::make_pair(CHEST_TEXTURE, &chestTexture));
	textures.insert(std::make_pair(CARD_TEXTURE, &cardTexture));
	textures.insert(std::make_pair(DAMAGE_UPGRADE_TEXTURE, &damageUpgradeTexture));
	textures.insert(std::make_pair(HEALTH_UPGRADE_TEXTURE, &healthUpgradeTexture));
	textures.insert(std::make_pair(MOVE_SPEED_UPGRADE_TEXTURE, &moveSpeedUpgradeTexture));
	textures.insert(std::make_pair(FIRE_SPEED_UPGRADE_TEXTURE, &fireSpeedUpgradeTexture));
	textures.insert(std::make_pair(PICKUP_UPGRADE_TEXTURE, &pickupUpgradeTexture));
	textures.insert(std::make_pair(THORN_AURA_TEXTURE, &thornAuraTexture));
	textures.insert(std::make_pair(BUTTON_TEXTURE, &buttonTexture));
	textures.insert(std::make_pair(MAIN_MENU_BACKGROUND_TEXTURE, &mainMenuBackgroundTexture));
	textures.insert(std::make_pair(ALT_MENU_BACKGROUND_TEXTURE, &altMenuBackgroundTexture));
	textures.insert(std::make_pair(CHARACTER_SELECT_BORDER_TEXTURE, &characterSelectBorderTexture));
}

TextureHandler::~TextureHandler()
{
	for (auto& texture : textures) {
		UnloadTexture(*texture.second);
		delete texture.second;
	}
}

Texture2D* TextureHandler::GetTexture(TextureName name)
{
	return textures[name];
}

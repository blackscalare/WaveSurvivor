#pragma once
#include "Definitions.h"
#include "GameHandler.h"
#include "TextureHandler.h"

#define NUM_CARD_FRAMES 3
#define CARD_PADDING 20

class LevelUpScreenHandler
{
public:
	LevelUpScreenHandler(GameHandler* gameHandler, TextureHandler* textureHandler);
	~LevelUpScreenHandler();
	void Init(bool reset);
	void Update();
	void Reset();
	void BindCallback(std::function<void(void)> fn) {
		callback = fn;
	}

	Texture2D GetCardTexture() const;
	Rectangle GetSourceRec() const;
	Rectangle GetCardBounds() const;
	CardEvent GetCardEvent() const;
	std::vector<Card*> GetCards();

private:
	GameHandler* gameHandler;
	TextureHandler* textureHandler;
	float frameWidth;
	Rectangle sourceRec;
	Rectangle cardBounds;
	int cardState = 0;
	Vector2 mousePoint;
	CardEvent cardEvent;
	std::vector<Card*> cards;
	std::function<void(void)> callback;

	Texture2D* cardTexture;
	Texture2D* damageUpgradeTexture;
	Texture2D* healthUpgradeTexture;
	Texture2D* moveSpeedUpgradeTexture;
	Texture2D* fireSpeedUpgradeTexture;
	Texture2D* pickupUpgradeTexture;
	Texture2D* thornAuraTexture;
	std::vector<Texture2D*> images;
};


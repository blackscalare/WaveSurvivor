#include "LevelUpScreenHandler.h"
#include "Logger.h"

LevelUpScreenHandler::LevelUpScreenHandler(GameHandler* gameHandler, TextureHandler* textureHandler)
{
	this->gameHandler = gameHandler;
	this->textureHandler = textureHandler;
	Init(false);
}

LevelUpScreenHandler::~LevelUpScreenHandler()
{
	/*for (auto tex : images) {
		delete tex;
	}*/
}

void LevelUpScreenHandler::Init(bool reset)
{
	if (!reset) {
		cardTexture = textureHandler->GetTexture(CARD_TEXTURE);
		damageUpgradeTexture = textureHandler->GetTexture(DAMAGE_UPGRADE_TEXTURE);
		healthUpgradeTexture = textureHandler->GetTexture(HEALTH_UPGRADE_TEXTURE);
		moveSpeedUpgradeTexture = textureHandler->GetTexture(MOVE_SPEED_UPGRADE_TEXTURE);
		fireSpeedUpgradeTexture = textureHandler->GetTexture(FIRE_SPEED_UPGRADE_TEXTURE);
		pickupUpgradeTexture = textureHandler->GetTexture(PICKUP_UPGRADE_TEXTURE);
		thornAuraTexture = textureHandler->GetTexture(THORN_AURA_TEXTURE);

		// IMPORTANT Images are in the same order as the CardType enum
		images = { moveSpeedUpgradeTexture, damageUpgradeTexture, fireSpeedUpgradeTexture, healthUpgradeTexture, pickupUpgradeTexture, thornAuraTexture };
	}
	else {
		for (auto c : cards)
			delete c;
		cards.clear();
	}

	Card* card1 = new Card;
	Card* card2 = new Card;
	Card* card3 = new Card;

	card1->id = 0;
	card2->id = 1;
	card3->id = 2;

	frameWidth = (float)cardTexture->width / NUM_CARD_FRAMES;
	sourceRec = { 0, 0, frameWidth, (float)cardTexture->height };

	card1->sourceRec = sourceRec;
	card2->sourceRec = sourceRec;
	card3->sourceRec = sourceRec;

	card1->bounds = { GetScreenWidth() / 2.0f - (cardTexture->width * 3.f) / NUM_CARD_FRAMES / 2.0f - CARD_PADDING, GetScreenHeight() / 2.0f - cardTexture->height / 2.0f, frameWidth, (float)cardTexture->height };
	card2->bounds = { GetScreenWidth() / 2.0f - cardTexture->width / NUM_CARD_FRAMES / 2.0f, GetScreenHeight() / 2.0f - cardTexture->height / 2.0f, frameWidth, (float)cardTexture->height };
	card3->bounds = { GetScreenWidth() / 2.0f + cardTexture->width / NUM_CARD_FRAMES / 2.0f + CARD_PADDING, GetScreenHeight() / 2.0f - cardTexture->height / 2.0f, frameWidth, (float)cardTexture->height };

	mousePoint = { 0.f, 0.f };
	cardEvent.selectedCard = -1;
	cardEvent.cardAction = false;

	cards = { card1, card2, card3 };

	// Generate card types
	std::srand(static_cast<unsigned int>(std::time(nullptr)));
	CardType cardTypes[] = { SPEED, DAMAGE, ATTACK_SPEED, HEALTH, PICKUP, THORN_AURA };
	// Shuffle the card types array to ensure they are never the same
	std::shuffle(std::begin(cardTypes), std::end(cardTypes), std::default_random_engine(std::rand()));
	for (int i = 0; i < cards.size(); ++i) {
		cards[i]->type = cardTypes[i];
	}

	// Set appropriate texts and descriptions
	for (Card* c : cards) {
		c->text = CARD_TEXT[c->type];
		c->description = CARD_DESCRIPTIIONS[c->type];
		c->image = images[c->type];
	}
}

void LevelUpScreenHandler::Update()
{
	mousePoint = GetMousePosition();

	for (Card* c : cards) {
		if (CheckCollisionPointRec(mousePoint, c->bounds)) {
			if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) c->state = 2;
			else c->state = 1;
			if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
				cardEvent.selectedCard = c->id;
				cardEvent.cardAction = true;
			}
		}
		else {
			c->state = 0;
		}

		c->sourceRec.x = c->state * frameWidth;
	}
	if (cardEvent.cardAction) {
		// Handle card click
		Logger::Debug("Clicked card", cardEvent.selectedCard);
		gameHandler->HandleSelectedCard(cards[cardEvent.selectedCard]);
	}
}

void LevelUpScreenHandler::Reset()
{
	Init(true);
}

Texture2D LevelUpScreenHandler::GetCardTexture() const
{
	return *cardTexture;
}

Rectangle LevelUpScreenHandler::GetSourceRec() const
{
	return sourceRec;
}

Rectangle LevelUpScreenHandler::GetCardBounds() const
{
	return cardBounds;
}

CardEvent LevelUpScreenHandler::GetCardEvent() const
{
	return cardEvent;
}

std::vector<Card*> LevelUpScreenHandler::GetCards()
{
	return cards;
}

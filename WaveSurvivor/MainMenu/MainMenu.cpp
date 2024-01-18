#include "MainMenu.h"

#define BUTTON_PADDING 120

MainMenu::MainMenu(TextureHandler* textureHandler)
{
	this->textureHandler = textureHandler;
	Init();
}

void MainMenu::Update()
{
	mousePoint = GetMousePosition();

	for (MenuButton* button : buttons) {
		if (CheckCollisionPointRec(mousePoint, button->bounds)) {
			if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) button->state = 2;
			else button->state = 1;
			if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
				// Handle click event
				callback(button->id);
			}
		}
		else {
			button->state = 0;
		}

		button->sourceRec.y = button->state * frameHeight;
	}

	float scaleX = static_cast<float>(GetScreenWidth()) / static_cast<float>(textureHandler->GetTexture(MAIN_MENU_BACKGROUND_TEXTURE)->width);
	float scaleY = static_cast<float>(GetScreenHeight()) / static_cast<float>(textureHandler->GetTexture(MAIN_MENU_BACKGROUND_TEXTURE)->height);

	// Calculate the average scale to fit the texture into the window
	float scale = (scaleX + scaleY) / 2.0f;
	DrawTextureEx(*textureHandler->GetTexture(MAIN_MENU_BACKGROUND_TEXTURE), { 0,0 }, 0, scale, WHITE);

	for (auto& button : buttons) {
		// TODO: replace card text sizes with button text sizes
		int buttonXMax = button->bounds.x + (button->bounds.x + button->bounds.width);
		int buttonTextX = Tools::Text::CenterTextX(buttonXMax, button->text, CARD_TEXT_SIZE);
		DrawTextureRec(*textureHandler->GetTexture(BUTTON_TEXTURE), button->sourceRec, { button->bounds.x, button->bounds.y }, WHITE);
		DrawText(button->text, buttonTextX, button->bounds.y + CARD_TEXT_SIZE + 20, CARD_TEXT_SIZE, WHITE);
	}
}

std::vector<MenuButton*> MainMenu::GetButtons()
{
	return buttons;
}

void MainMenu::Init()
{
	buttonTexture = textureHandler->GetTexture(BUTTON_TEXTURE);
	MenuButton* startButton = new MenuButton;
	MenuButton* optionsButton = new MenuButton;
	MenuButton* exitButton = new MenuButton;

	startButton->id = MAIN_MENU_BUTTON_START_ID;
	optionsButton->id = MAIN_MENU_BUTTON_OPTIONS_ID;
	exitButton->id = MAIN_MENU_BUTTON_EXIT_ID;

	frameHeight = (float)buttonTexture->height / NUM_BUTTON_FRAMES;
	sourceRec = { 0, 0, (float)buttonTexture->width, frameHeight };

	startButton->sourceRec = sourceRec;
	optionsButton->sourceRec = sourceRec;
	exitButton->sourceRec = sourceRec;

	startButton->bounds = { GetScreenWidth() / 2.0f - buttonTexture->width / 2.0f, GetScreenHeight() / 2.0f - buttonTexture->height / NUM_BUTTON_FRAMES / 2.0f - BUTTON_PADDING, (float)buttonTexture->width, frameHeight };
	optionsButton->bounds = { GetScreenWidth() / 2.0f - buttonTexture->width / 2.0f, GetScreenHeight() / 2.0f - buttonTexture->height / NUM_BUTTON_FRAMES / 2.0f, (float)buttonTexture->width, frameHeight };
	exitButton->bounds = { GetScreenWidth() / 2.0f - buttonTexture->width / 2.0f, GetScreenHeight() / 2.0f - buttonTexture->height / NUM_BUTTON_FRAMES / 2.0f + BUTTON_PADDING, (float)buttonTexture->width, frameHeight };

	mousePoint = { 0,0 };

	startButton->text = "START";
	optionsButton->text = "OPTIONS";
	exitButton->text = "EXIT";

	buttons = { startButton, optionsButton, exitButton };
}

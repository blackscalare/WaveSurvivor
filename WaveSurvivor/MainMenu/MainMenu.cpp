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
			}
		}
		else {
			button->state = 0;
		}

		button->sourceRec.y = button->state * frameHeight;
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

	startButton->id = 0;
	optionsButton->id = 1;
	exitButton->id = 2;

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

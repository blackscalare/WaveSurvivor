#include "WaveSurvivor.h"
#include "Logger.h"

WaveSurvivor::WaveSurvivor()
{
	Init();
}

WaveSurvivor::~WaveSurvivor()
{
	if (eventHandler != nullptr) {
		delete eventHandler;
	}

	if (textureHandler != nullptr) {
		delete textureHandler;
	}

	if (gameHandler != nullptr) {
		delete gameHandler;
	}

	if (levelUpScreenHandler != nullptr) {
		delete levelUpScreenHandler;
	}

	if (mainMenu != nullptr) {
		delete mainMenu;
	}

	if (renderer != nullptr) {
		delete renderer;
	}
}

int WaveSurvivor::Run()
{
	renderer->Render();
	return 0;
}

void WaveSurvivor::Init()
{
	// Init window properties and FPS
	InitWindow(WIDTH, HEIGHT, TITLE);
	SetTargetFPS(FPS);
	
	eventHandler = new EventHandler();
	eventHandler->LoadEvents();
	textureHandler = new TextureHandler();
	gameHandler = new GameHandler(textureHandler, eventHandler);
	levelUpScreenHandler = new LevelUpScreenHandler(gameHandler, textureHandler);
	optionsMenu = new OptionsMenu();
	mainMenu = new MainMenu(textureHandler);
	mainMenu->BindCallback(std::bind(&WaveSurvivor::MainMenuButtonCallback, this, std::placeholders::_1));

	// Initialize everything else before the renderer
	renderer = new Renderer(textureHandler, gameHandler, levelUpScreenHandler, mainMenu, optionsMenu);

}

void WaveSurvivor::MainMenuButtonCallback(int buttonId)
{
	switch (buttonId) {
	case MAIN_MENU_BUTTON_START_ID:
		renderer->SetState(GAME);
		gameHandler->ResetStartTime();
		break;
	case MAIN_MENU_BUTTON_OPTIONS_ID:
		Logger::Debug("Implement");
		break;
	case MAIN_MENU_BUTTON_EXIT_ID:
		// TODO: Implement proper shutdown
		CloseWindow();
		exit(0);
		break;
	default:
		Logger::Log(Logger::ERROR, "Main menu button not implemented!");
	}
}

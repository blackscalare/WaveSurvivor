#include "WaveSurvivor.h"
#include "Logger.h"

WaveSurvivor::WaveSurvivor()
{
	Init();
}

WaveSurvivor::~WaveSurvivor()
{
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

	textureHandler = new TextureHandler();
	gameHandler = new GameHandler(textureHandler);
	levelUpScreenHandler = new LevelUpScreenHandler(gameHandler, textureHandler);
	mainMenu = new MainMenu(textureHandler);
	mainMenu->BindCallback(std::bind(&WaveSurvivor::MainMenuButtonCallback, this, std::placeholders::_1));

	// Initialize everything else before the renderer
	renderer = new Renderer(textureHandler, gameHandler, levelUpScreenHandler, mainMenu);

}

void WaveSurvivor::MainMenuButtonCallback(int buttonId)
{
	switch (buttonId) {
	case MAIN_MENU_BUTTON_START_ID:
		renderer->SetState(GAME);
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
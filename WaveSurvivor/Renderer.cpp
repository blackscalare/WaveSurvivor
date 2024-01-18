#include "Renderer.h"
#include "Logger.h"

Renderer::Renderer(TextureHandler* textureHandler, GameHandler* gameHandler, LevelUpScreenHandler* levelUpScreenHandler, MainMenu* mainMenu, OptionsMenu* optionsMenu)
{
	currentState = MAIN_MENU;

	this->textureHandler = textureHandler;
	this->gameHandler = gameHandler;
	this->levelUpScreenHandler = levelUpScreenHandler;
	this->mainMenu = mainMenu;
	this->optionsMenu = optionsMenu;

	centerX = WIDTH / 2;
	centerY = HEIGHT / 2;

	// TODO: move player animations somewhere else, animation handler?
	framesCounter = 0;
	playerFramesSpeed = 7;
	currentPlayerFrame = 0;
	playerFrameRec = { 0.f, 0.f, (float)textureHandler->GetTexture(PLAYER_TEXTURE)->width / 3, (float)textureHandler->GetTexture(PLAYER_TEXTURE)->height };

	secondsLoc = GetShaderLocation(*textureHandler->GetShader(XP_ORB_SHADER), "seconds");
	freqXLoc = GetShaderLocation(*textureHandler->GetShader(XP_ORB_SHADER), "freqX");
	freqYLoc = GetShaderLocation(*textureHandler->GetShader(XP_ORB_SHADER), "freqY");
	ampXLoc = GetShaderLocation(*textureHandler->GetShader(XP_ORB_SHADER), "ampX");
	ampYLoc = GetShaderLocation(*textureHandler->GetShader(XP_ORB_SHADER), "ampY");
	speedXLoc = GetShaderLocation(*textureHandler->GetShader(XP_ORB_SHADER), "speedX");
	speedYLoc = GetShaderLocation(*textureHandler->GetShader(XP_ORB_SHADER), "speedY");
	
	float freqX = 25.0f;
	float freqY = 25.0f;
	float ampX = 5.0f;
	float ampY = 5.0f;
	float speedX = 8.0f;
	float speedY = 8.0f;

	float screenSize[2] = { (float)GetScreenWidth(), (float)GetScreenHeight() };
	SetShaderValue(*textureHandler->GetShader(XP_ORB_SHADER), GetShaderLocation(*textureHandler->GetShader(XP_ORB_SHADER), "size"), &screenSize, SHADER_UNIFORM_VEC2);
	SetShaderValue(*textureHandler->GetShader(XP_ORB_SHADER), freqXLoc, &freqX, SHADER_UNIFORM_FLOAT);
	SetShaderValue(*textureHandler->GetShader(XP_ORB_SHADER), freqYLoc, &freqY, SHADER_UNIFORM_FLOAT);
	SetShaderValue(*textureHandler->GetShader(XP_ORB_SHADER), ampXLoc, &ampX, SHADER_UNIFORM_FLOAT);
	SetShaderValue(*textureHandler->GetShader(XP_ORB_SHADER), ampYLoc, &ampY, SHADER_UNIFORM_FLOAT);
	SetShaderValue(*textureHandler->GetShader(XP_ORB_SHADER), speedXLoc, &speedX, SHADER_UNIFORM_FLOAT);
	SetShaderValue(*textureHandler->GetShader(XP_ORB_SHADER), speedYLoc, &speedY, SHADER_UNIFORM_FLOAT);
}

Renderer::~Renderer()
{
}

void Renderer::Render()
{
	while (!WindowShouldClose()) {
		ClearBackground(BLACK);

		if (gameHandler->GetGameOver()) {
			ReturnToMainMenu();
		}

		if (gameHandler->PlayerJustLeveledUp()) {
			currentState = LEVEL_UP;
			gameHandler->PauseGame();
			gameHandler->SetPlayerJustLeveledUp(false);
		}

		if (gameHandler->PlayerJustOpenedChest()) {
			// TODO this should be very similar to the level up screen but have other things?
			currentState = LEVEL_UP;
			gameHandler->PauseGame();
			gameHandler->SetPlayerJustOpenedChest(false);
		}

		BeginDrawing();

			switch (currentState) {
			case MAIN_MENU:
				//RenderMainMenu();
				RenderOptions();
				break;
			case GAME:
			case LEVEL_UP:
				RenderGame();
				break;
			case OPTIONS:
				RenderOptions();
				break;
			}

		EndDrawing();
	}

	CloseWindow();

}

void Renderer::SetState(RenderState state)
{
	currentState = state;
}

void Renderer::DrawObjects(std::vector<Object> objectsInViewport)
{
	if (objectsInViewport.size() > 0) {
		for (const Object& o : objectsInViewport) {
			// Convert position from world to screen
			Position pos = o.GetPosition();
			Position screenPos = Tools::ScreenSpace::GetWorldToScreen(pos, gameHandler->GetPlayerPosition());

			switch (o.GetType()) {
			case NONE:
				DrawRectangle(screenPos.x, screenPos.y, 10, 10, WHITE);
				break;
			case XP_ORB:
				SetShaderValue(*textureHandler->GetShader(XP_ORB_SHADER), secondsLoc, &framesCounter, SHADER_UNIFORM_FLOAT);
				BeginShaderMode(*textureHandler->GetShader(XP_ORB_SHADER));
					DrawTexture(*textureHandler->GetTexture(XP_ORB_TEXTURE), screenPos.x, screenPos.y, WHITE);
				EndShaderMode();
				break;
			case CHEST:
				DrawTexture(*textureHandler->GetTexture(CHEST_TEXTURE), screenPos.x, screenPos.y, WHITE);
				break;
			default:
				Logger::Log(Logger::ERROR, "Object type not implement");
			}
		}
	}
}

void Renderer::DrawEnemies(std::vector<Zombie*> enemiesInViewport)
{
	if (enemiesInViewport.size() > 0) {
		for (Zombie* z : enemiesInViewport) {
			Position pos = z->GetPosition();
			// Convert position from world to screen
			Position screenPos = Tools::ScreenSpace::GetWorldToScreen( pos, gameHandler->GetPlayerPosition());

			DrawTexture(*textureHandler->GetTexture(ZOMBIE_TEXTURE), screenPos.x, screenPos.y, WHITE);
			GUI::HealthBar::DrawEnemyHealthBar(z, gameHandler->GetPlayerPosition());

			if(gameHandler->GetDebugMode()) {
				Position screenPos = Tools::ScreenSpace::GetWorldToScreen(*z->GetHitbox().source, gameHandler->GetPlayerPosition());
				DrawRectangleLines(screenPos.x, screenPos.y, z->GetHitbox().area.width, z->GetHitbox().area.width, GREEN);
			}
		}
	}
}

void Renderer::DrawProjectiles(std::vector<Position_f> projectilesInViewport)
{
	if (projectilesInViewport.size() > 0) {
		for (Position_f o : projectilesInViewport) {
			Position_f screenPos = Tools::ScreenSpace::GetWorldToScreen_f(o, gameHandler->GetPlayerPosition());

			DrawRectangleV(screenPos.ToVector(), { DEFAULT_PROJECTILE_WIDTH, DEFAULT_PROJECTILE_HEIGHT }, YELLOW);
		}
	}
}

void Renderer::DrawDebug()
{
	int centerX = GetScreenWidth() / 2;
	int centerY = GetScreenHeight() / 2;

	Position playerFireDestination = gameHandler->GetPlayerFireDestination();

	// Draw ray casting line to nearest enemy
	if (playerFireDestination.x != 0 && playerFireDestination.y != 0) {
		Position screenPos = Tools::ScreenSpace::GetWorldToScreen(playerFireDestination, gameHandler->GetPlayerPosition());
		Position playerCenter = Tools::Shape::GetRectangleCenter(centerX, centerY, DEFAULT_PLAYER_WIDTH, DEFAULT_PLAYER_HEIGHT);
		DrawLine(playerCenter.x, playerCenter.y, screenPos.x, screenPos.y, GREEN);
	}

	GUI::Debug::DrawDebugUI();
}

void Renderer::DrawPlayer()
{
	if (gameHandler->PlayerIsMoving()) {
		framesCounter++;
		if (framesCounter >= (60 / playerFramesSpeed)) {
			framesCounter = 0;
			currentPlayerFrame++;
			if (currentPlayerFrame > 3) currentPlayerFrame = 0;
			playerFrameRec.x = (float)currentPlayerFrame * (float)textureHandler->GetTexture(PLAYER_TEXTURE)->width / 3;
		}
	}
	else {
		framesCounter = 0;
		currentPlayerFrame = 0;
		playerFrameRec = { 0.f, 0.f, (float)textureHandler->GetTexture(PLAYER_TEXTURE)->width / 3, (float)textureHandler->GetTexture(PLAYER_TEXTURE)->height };
	}
	
	DrawTextureRec(*textureHandler->GetTexture(PLAYER_TEXTURE), playerFrameRec, { (float)centerX, (float)centerY }, WHITE);
	GUI::HealthBar::DrawPlayerHealthBar(gameHandler->GetWorldPtr()->GetPlayerPtr());

	// Draw hitbox
	if (gameHandler->GetDebugMode()) {
		Player* p = gameHandler->GetWorldPtr()->GetPlayerPtr();
		Position screenPos = Tools::ScreenSpace::GetWorldToScreen(*p->GetHitbox().source, gameHandler->GetPlayerPosition());
		Position playerCenter = Tools::Shape::GetRectangleCenter(centerX, centerY, DEFAULT_PLAYER_WIDTH, DEFAULT_PLAYER_HEIGHT);
		// Draw hitbox
		DrawRectangleLines(screenPos.x, screenPos.y, p->GetHitbox().area.width, p->GetHitbox().area.width, GREEN);

		// Draw pickup radius
		DrawCircleLines(playerCenter.x, playerCenter.y, p->GetPickupRadius(), GREEN);
	}
}

void Renderer::DrawGUI()
{
	GUI::HUD::DrawXPBar(gameHandler->GetWorldPtr()->GetPlayerPtr());
	GUI::HUD::DrawZombiesKilled(gameHandler->GetZombiesKilled());
	GUI::HUD::DrawTime(gameHandler->GetElapsedTime());
}

void Renderer::DrawBackground()
{
	Position playerPos = gameHandler->GetPlayerPosition();

	float x = fmod(-playerPos.x + GetScreenWidth() / 2.0f, textureHandler->GetTexture(BACKGROUND_TEXTURE)->width);
	float y = fmod(-playerPos.y + GetScreenHeight() / 2.0f, textureHandler->GetTexture(BACKGROUND_TEXTURE)->height);

	if(playerPos.x + GetScreenWidth() / 2 > gameHandler->GetWorldPtr()->GetWorldSize().x) {
		DrawTextureEx(*textureHandler->GetTexture(BACKGROUND_TEXTURE), { playerPos.x + GetScreenWidth() / 2.0f, y }, 0, 1.0f, WHITE);
	}

	DrawTextureEx(*textureHandler->GetTexture(BACKGROUND_TEXTURE), { x, y }, 0, 1.0f, WHITE);
}

void Renderer::ReturnToMainMenu()
{
}

void Renderer::RenderGame()
{
	gameHandler->Update();

	std::vector<Object> objectsInViewport = gameHandler->GetObjectsInViewport();
	std::vector<Zombie*> enemiesInViewport = gameHandler->GetEnemiesInViewport();
	std::vector<Position_f> projectilesInViewport = gameHandler->GetProjectilesInViewport();
	
	DrawBackground();
	DrawGUI();
	DrawPlayer();
	DrawObjects(objectsInViewport);
	DrawEnemies(enemiesInViewport);
	DrawProjectiles(projectilesInViewport);

	if (currentState == LEVEL_UP)
		RenderLevelUpScreen();

	if (gameHandler->GetDebugMode()) {
		DrawDebug();
	}
}

void Renderer::RenderLevelUpScreen()
{
	levelUpScreenHandler->Update();

	// Fade out background
	DrawRectangle(0, 0, WIDTH, HEIGHT, { 0, 0, 0, 100 });

	std::vector<Card*> cards = levelUpScreenHandler->GetCards();

	for (auto& c : cards) {
		int cardXMax = c->bounds.x + (c->bounds.x + c->bounds.width);
		int cardTextX = Tools::Text::CenterTextX(cardXMax, c->text, CARD_TEXT_SIZE);
		int cardDescriptionX = Tools::Text::CenterTextX(cardXMax, c->description, CARD_DESCRIPTION_SIZE);
		int cardImageX = (cardXMax - c->image->width) / 2;
		DrawTextureRec(levelUpScreenHandler->GetCardTexture(), c->sourceRec, { c->bounds.x, c->bounds.y }, WHITE);
		DrawText(c->text, cardTextX, c->bounds.y + CARD_TEXT_SIZE + 20, CARD_TEXT_SIZE, WHITE);
		DrawTexture(*c->image, cardImageX, c->bounds.y + CARD_TEXT_SIZE + 20 + c->image->height - 30, WHITE);
		DrawText(c->description, cardDescriptionX, c->bounds.y + CARD_TEXT_SIZE + 20 + c->image->height + 50, CARD_DESCRIPTION_SIZE, WHITE);
	}

	if (levelUpScreenHandler->GetCardEvent().cardAction) {
		currentState = GAME;
		levelUpScreenHandler->Reset();
		gameHandler->UnpauseGame();
	}
}

void Renderer::RenderMainMenu()
{
	mainMenu->Update();
}

void Renderer::RenderOptions()
{
	optionsMenu->Update();
}

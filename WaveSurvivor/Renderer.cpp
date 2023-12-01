#include "Renderer.h"
#include "Logger.h"

#define WIDTH 1280
#define HEIGHT 720
#define TITLE "Wave Survivor"

Renderer::Renderer()
{
	InitWindow(WIDTH, HEIGHT, TITLE);
	SetTargetFPS(FPS);
	
	// TODO should be main menu or something at start
	currentState = GAME;

	// TODO should not be initialized here since we do not start the game immediately
	gameHandler = new GameHandler();
	levelUpScreenHandler = new LevelUpScreenHandler(gameHandler);

	centerX = WIDTH / 2;
	centerY = HEIGHT / 2;


	// TODO: Move all texture loading and initialization to a separate class
	// Renderer should only be responsible for rendering, nothing else.
	background		= LoadTexture("textures/mapTexture_huge.png");
	playerTexture	= LoadTexture("textures/playerAnimTexture.png");
	zombieTexture	= LoadTexture("textures/zombieTexture.png");
	xpOrbTexture	= LoadTexture("textures/xpOrbTexture.png");
	chestTexture	= LoadTexture("textures/chestTexture.png");

	framesCounter = 0;
	playerFramesSpeed = 7;
	currentPlayerFrame = 0;
	playerFrameRec = { 0.f, 0.f, (float)playerTexture.width / 3, (float)playerTexture.height };
}

Renderer::~Renderer()
{
	if(gameHandler != nullptr)
		delete gameHandler;
	if(levelUpScreenHandler != nullptr)
		delete levelUpScreenHandler;
	UnloadTexture(background);
	UnloadTexture(playerTexture);
	UnloadTexture(zombieTexture);
	UnloadTexture(xpOrbTexture);
	UnloadTexture(chestTexture);
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
			gameHandler->SetPlayerJustLeveledUp(false);
		}

		if (gameHandler->PlayerJustOpenedChest()) {
			// TODO this should be very similar to the level up screen but have other things?
			currentState = LEVEL_UP;
			gameHandler->SetPlayerJustOpenedChest(false);
		}

		BeginDrawing();

			switch (currentState) {
			case MAIN_MENU:
				break;
			case GAME:
			case LEVEL_UP:
				RenderGame();
				break;
			case OPTIONS:
				break;
			}

		EndDrawing();
	}

	CloseWindow();

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
				//DrawCircle(screenPos.x, screenPos.y, DEFAULT_XP_ORB_RADIUS, GREEN);
				DrawTexture(xpOrbTexture, screenPos.x, screenPos.y, WHITE);
				break;
			case CHEST:
				DrawTexture(chestTexture, screenPos.x, screenPos.y, WHITE);
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

			//DrawRectangle(screenPos.x, screenPos.y, DEFAULT_ZOMBIE_WIDTH, DEFAULT_ZOMBIE_WIDTH, RED);
			DrawTexture(zombieTexture, screenPos.x, screenPos.y, WHITE);
			GUI::HealthBar::DrawEnemyHealthBar(z, gameHandler->GetPlayerPosition());

			if(gameHandler->GetDebugMode()) {
				// TODO: Since the zombies are only circles now, adjust the drawn hitbox
				/*Position zPos = *z->GetHitbox().source;
				zPos.x -= DEFAULT_ZOMBIE_RADIUS;
				zPos.y -= DEFAULT_ZOMBIE_RADIUS;*/
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
}

void Renderer::DrawPlayer()
{
	if (gameHandler->PlayerIsMoving()) {
		framesCounter++;
		if (framesCounter >= (60 / playerFramesSpeed)) {
			framesCounter = 0;
			currentPlayerFrame++;
			if (currentPlayerFrame > 3) currentPlayerFrame = 0;
			playerFrameRec.x = (float)currentPlayerFrame * (float)playerTexture.width / 2;
		}
	}
	else {
		framesCounter = 0;
		currentPlayerFrame = 0;
		playerFrameRec = { 0.f, 0.f, (float)playerTexture.width / 3, (float)playerTexture.height };
	}
	
	//DrawRectangle(centerX, centerY, 10, 10, WHITE);
	//DrawTexture(playerTexture, centerX, centerY, WHITE);
	DrawTextureRec(playerTexture, playerFrameRec, { (float)centerX, (float)centerY }, WHITE);
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
}

void Renderer::DrawBackground()
{
	Position playerPos = gameHandler->GetPlayerPosition();

	float x = fmod(-playerPos.x + GetScreenWidth() / 2.0f, background.width);
	float y = fmod(-playerPos.y + GetScreenHeight() / 2.0f, background.height);

	if(playerPos.x + GetScreenWidth() / 2 > gameHandler->GetWorldPtr()->GetWorldSize().x) {
		DrawTextureEx(background, { playerPos.x + GetScreenWidth() / 2.0f, y }, 0, 1.0f, WHITE);
	}

	DrawTextureEx(background, { x, y }, 0, 1.0f, WHITE);
}

void Renderer::ReturnToMainMenu()
{
	if(gameHandler != nullptr)
		delete gameHandler;
	if (levelUpScreenHandler != nullptr)
		delete levelUpScreenHandler;
	UnloadTexture(background);
	UnloadTexture(playerTexture);
	UnloadTexture(zombieTexture);
	UnloadTexture(xpOrbTexture);
}

void Renderer::RenderGame()
{
	if (gameHandler == nullptr)
		gameHandler = new GameHandler();

	if(currentState != LEVEL_UP)
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
	if (levelUpScreenHandler == nullptr)
		levelUpScreenHandler = new LevelUpScreenHandler(gameHandler);

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
	}
}

#include "GUI.h"
#include "Logger.h"
#include "raygui.h"

void GUI::HUD::DrawXPBar(Player* player)
{
	int xp = player->GetXp();
	int level = player->GetLevel();
	long long levelThreshold = player->GetLevelThreshold();

	float t = xp / static_cast<float>(levelThreshold);
	float xpBarWidth = 0.0f + t * (GetScreenWidth() - 0.0f);

	DrawRectangle(0, 0, xpBarWidth, 10, GOLD);
	char levelString[8];
	snprintf(levelString, 8, "%d", level);
	DrawText(levelString, GetScreenWidth() / 2 - TextLength(levelString), 20, 30, WHITE);
}

void GUI::HUD::DrawZombiesKilled(int zombiesKilled)
{
	char zombiesKilledString[16];
	snprintf(zombiesKilledString, 16, "%d", zombiesKilled);
	DrawText(zombiesKilledString, TextLength(zombiesKilledString) + 10, 20, 20, WHITE);
}

void GUI::HUD::DrawTime(long long elapsedTime)
{
	int hours = static_cast<int>(elapsedTime / 3600000); // 1 hour = 3600000 milliseconds
	int minutes = static_cast<int>((elapsedTime % 3600000) / 60000); // 1 minute = 60000 milliseconds
	int seconds = static_cast<int>((elapsedTime % 60000) / 1000); // 1 second = 1000 milliseconds

	std::ostringstream formattedTime;

	if (hours > 0) {
		formattedTime << std::setfill('0') << std::setw(2) << hours << ":";
	}

	formattedTime << std::setfill('0') << std::setw(2) << minutes << ":";
	formattedTime << std::setfill('0') << std::setw(2) << seconds;
	DrawText(formattedTime.str().c_str(), TextLength(formattedTime.str().c_str()), 20 + 20, 20, WHITE);
}

void GUI::HealthBar::DrawPlayerHealthBar(Player* player)
{
	int health = player->GetHealth();
	int maxPlayerHealth = player->GetMaxHealth();
	Position pos = player->GetPosition();
	float barWidth = 100.0f;

	float normalizedHealth = (float)health / (float)maxPlayerHealth;
	float healthWidth = normalizedHealth * barWidth;

	Position playerScreenPos = Tools::ScreenSpace::GetWorldToScreen(pos, pos);
	Position healthBarPos = Position((playerScreenPos.x + DEFAULT_PLAYER_WIDTH / 2) - (barWidth / 2), playerScreenPos.y - DEFAULT_PLAYER_HEIGHT - 5);

	DrawRectangle(healthBarPos.x, healthBarPos.y, healthWidth, 10, GREEN);
	DrawRectangleLines(healthBarPos.x, healthBarPos.y, barWidth, 10, WHITE);

}

void GUI::HealthBar::DrawEnemyHealthBar(Zombie* zombie, Position playerPos)
{
	int health = zombie->GetHealth();
	int maxZombieHealth = zombie->GetMaxHealth();
	Position pos = zombie->GetPosition();
	float barWidth = 30.0f;

	float normalizedHealth = (float)health / (float)maxZombieHealth;
	float healthWidth = normalizedHealth * barWidth;

	Position zombieScreenPos = Tools::ScreenSpace::GetWorldToScreen(pos, playerPos);
	Position healthBarPos = Position((zombieScreenPos.x + DEFAULT_ZOMBIE_WIDTH / 2) - (barWidth / 2), zombieScreenPos.y - DEFAULT_ZOMBIE_HEIGHT);

	DrawRectangle(healthBarPos.x, healthBarPos.y, healthWidth, 5, RED);
	DrawRectangleLines(healthBarPos.x, healthBarPos.y, barWidth, 5, WHITE);
}

void GUI::Debug::DrawDebugUI(GameHandler* gameHandler)
{
	const char* WindowBox000Text = "Debug";    // WINDOWBOX: WindowBox000
	const char* CheckBox_GodModeText = "Godmode";    // CHECKBOXEX: CheckBox_GodMode
	const char* Button_SpawnEnemyText = "Spawn enemy";    // BUTTON: Button_SpawnEnemy
	const char* Button_SpawnChestText = "Spawn chest";    // BUTTON: Button_SpawnChest
	const char* Button_TriggerCardsText = "Trigger cards";    // BUTTON: Button_TriggerCards
	const char* Button_LevelText = "Level up";

	Vector2 anchor01 = { 920, 48 };            // ANCHOR ID:1
	bool WindowBox000Active = true;            // WindowBox: WindowBox000
	bool CheckBox_GodModeChecked = false;            // CheckBoxEx: CheckBox_GodMode

	// Define controls rectangles
	Rectangle layoutRecs[6] = {
		{
		 anchor01.x + -56, anchor01.y + 0, 216, 192
		},    // WindowBox: WindowBox000
		{
		anchor01.x + -8, anchor01.y + 40, 16, 16
		},    // CheckBoxEx: CheckBox_GodMode
		{
		anchor01.x + -8, anchor01.y + 80, 112, 16
		},    // Button: Button_LevelUp
		{
		anchor01.x + -8, anchor01.y + 104, 112, 16
		},    // Button: Button_SpawnEnemy
		{
		anchor01.x + -8, anchor01.y + 128, 112, 16
		},    // Button: Button_SpawnChest
		{
		anchor01.x + -8, anchor01.y + 152, 112, 16
		},    // Button: Button_TriggerCards
	};

	if (WindowBox000Active)
	{
		WindowBox000Active = !GuiWindowBox(layoutRecs[0], WindowBox000Text);
		GuiCheckBox(layoutRecs[1], CheckBox_GodModeText, &CheckBox_GodModeChecked);

		if (GuiButton(layoutRecs[2], Button_LevelText)) gameHandler->DebugLevelUpPlayer();
		if (GuiButton(layoutRecs[3], Button_SpawnEnemyText)) gameHandler->SpawnEnemy();
		if (GuiButton(layoutRecs[4], Button_SpawnChestText)) gameHandler->SpawnChest();
		if (GuiButton(layoutRecs[5], Button_TriggerCardsText)) gameHandler->playerLevelUpCallback();
	}
}

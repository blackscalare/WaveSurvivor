#include "GUI.h"
#include "Logger.h"

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
	char zombiesKilledString[8];
	snprintf(zombiesKilledString, 8, "%d", zombiesKilled);
	DrawText(zombiesKilledString, TextLength(zombiesKilledString) + 10, 20, 20, WHITE);
}

void GUI::HUD::DrawTime(long long startTime)
{
	long long currentTime = Tools::Time::GetCurrentEpocMs();
	long long diff = currentTime - startTime;

	int hours = static_cast<int>(diff / 3600000); // 1 hour = 3600000 milliseconds
	int minutes = static_cast<int>((diff % 3600000) / 60000); // 1 minute = 60000 milliseconds
	int seconds = static_cast<int>((diff % 60000) / 1000); // 1 second = 1000 milliseconds

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
	Position pos = player->GetPosition();

	Position playerScreenPos = Tools::ScreenSpace::GetWorldToScreen(pos, pos);
	Position healthBarPos = Position(playerScreenPos.x - (DEFAULT_PLAYER_HEALTH / 2), playerScreenPos.y - DEFAULT_PLAYER_HEIGHT - 5);

	DrawRectangle(healthBarPos.x, healthBarPos.y, health, 10, GREEN);
	DrawRectangleLines(healthBarPos.x, healthBarPos.y, DEFAULT_PLAYER_HEALTH, 10, WHITE);

}

void GUI::HealthBar::DrawEnemyHealthBar(Zombie* zombie, Position playerPos)
{
	int health = zombie->GetHealth();
	Position pos = zombie->GetPosition();

	Position zombieScreenPos = Tools::ScreenSpace::GetWorldToScreen(pos, playerPos);
	Position healthBarPos = Position(zombieScreenPos.x - (DEFAULT_ZOMBIE_HEALTH / 2), zombieScreenPos.y - (DEFAULT_ZOMBIE_RADIUS * 2) - 5);

	DrawRectangle(healthBarPos.x, healthBarPos.y, health, 10, RED);
	DrawRectangleLines(healthBarPos.x, healthBarPos.y, DEFAULT_ZOMBIE_HEALTH, 10, WHITE);
}
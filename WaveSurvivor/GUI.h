#pragma once
#include "Definitions.h"
#include "Player.h"
#include "Zombie.h"
#include "Tools.h"
//#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

namespace GUI
{
	class HUD {
	public:
		static void DrawXPBar(Player* player);
		static void DrawZombiesKilled(int zombiesKilled);
		static void DrawTime(long long elapsedTime);
	};

	class HealthBar {
	public:
		static void DrawPlayerHealthBar(Player* player);
		static void DrawEnemyHealthBar(Zombie* zombie, Position playerPos);
	};

	class Debug {
	public:
		static void DrawDebugUI();
	};
};


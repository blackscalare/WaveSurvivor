#pragma once
#include "Definitions.h"
#include "Player.h"
#include "Zombie.h"
#include "Tools.h"

namespace GUI
{
	class HUD {
	public:
		static void DrawXPBar(Player* player);
	};

	class HealthBar {
	public:
		static void DrawPlayerHealthBar(Player* player);
		static void DrawEnemyHealthBar(Zombie* zombie, Position playerPos);
	};
};


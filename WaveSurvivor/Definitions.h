#pragma once
#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <map>
#include <chrono>
#include <vector>
#include <set>
#include <random>
#include <functional>
#include <iomanip>
#include <fstream>
#include <string>
#include <filesystem>
#include <thread>

#include "raylib.h"
#include "raymath.h"

// Other properties
#ifdef _DEBUG
#define EVENTS_PATH "../Events/"
#else
#define EVENTS_PATH "Events/"
#endif

// Window properties
#define WIDTH 1280
#define HEIGHT 720
#define FPS 60
#define TITLE "Wave Survivor"

// Sizes
#define DEFAULT_PLAYER_WIDTH 20
#define DEFAULT_PLAYER_HEIGHT 20
#define DEFAULT_ZOMBIE_WIDTH 20
#define DEFAULT_ZOMBIE_HEIGHT 20
#define DEFAULT_ZOMBIE_RADIUS 10
#define DEFAULT_PROJECTILE_WIDTH 5
#define DEFAULT_PROJECTILE_HEIGHT 5
#define DEFAULT_XP_ORB_RADIUS 10
#define WORLD_WIDTH 10000
#define WORLD_HEIGHT 10000

// Speed
#define DEFAULT_PLAYER_MOVE_SPEED 7.0f
#define DEFAULT_ZOMBIE_MOVE_SPEED 1.0f
#define DEFAULT_PROJECTILE_SPEED 5.0f

// Health
#define DEFAULT_PLAYER_HEALTH 100
#define DEFAULT_ZOMBIE_HEALTH 50

// Damage
#define DEFAULT_ZOMBIE_DAMAGE 2
#define DEFAULT_PROJECTILE_DAMAGE 5

// Player stats
#define DEFAULT_PLAYER_PICKUP_RADIUS 20.0f
#define DEFAULT_PLAYER_ATTACK_SPEED 800

// Render settings
#define MAX_DISTANCE_FROM_PLAYER 1000
#define MIN_DISTANCE_FROM_PLAYER 300

// XP values
#define DEFAULT_ZOMBIE_XP_VALUE 20
#define DEFAULT_PLAYER_LEVEL_THRESHOLD 100

// Spawn rates
#define DEFAULT_CHEST_SPAWNRATE 100

// Card texts -- changes depending on the CardTypes
/*
* SPEED
* DAMAGE
* ATTACK_SPEED
* HEALTH
*/
#define CARD_TEXT_SIZE 20
#define CARD_DESCRIPTION_SIZE 10

constexpr const char* CARD_TEXT[] = {
	"Speed",
	"Damage",
	"Attack speed",
	"Health",
	"Pickup",
	"Thorn aura"
};

constexpr const char* CARD_DESCRIPTIIONS[] = {
	"Move speed +5%",
	"Damage +5%",
	"Attack speed +5%",
	"Health +5%",
	"Pickup +5%",
	"Creates a thorn aura around player"
};
// Card texts

// Spells
#define DEFAULT_THORN_AURA		30
#define DEFAULT_THORN_DAMAGE	10
#define DEFAULT_THORN_FIRE_RATE 1100
#define DEFAULT_BOLT_FIRE_RATE 800
#define DEFAULT_BOLT_DAMAGE	10
// Spell

// IDs
#define MAIN_MENU_BUTTON_START_ID		0
#define MAIN_MENU_BUTTON_OPTIONS_ID		1
#define MAIN_MENU_BUTTON_EXIT_ID		2

#define NUM_BUTTON_FRAMES 3

struct Position
{
	int x, y;

	Position()
	{
		x = 0;
		y = 0;
	}

	Position(int x, int y)
	{
		this->x = x;
		this->y = y;
	}

	friend bool operator==(const Position& lhs, const Position& rhs)
	{
		return (lhs.x == rhs.x) && (lhs.y == rhs.x);
	}

	friend bool operator<(const Position& lhs, const Position& rhs)
	{
		return (lhs.x < rhs.x) || ((lhs.x == rhs.x) && (lhs.y < rhs.y));
	}

	Vector2 ToVector()
	{
		return { (float)x, (float)y };
	}
};

struct Position_f
{
	float x, y;

	Position_f()
	{
		x = 0;
		y = 0;
	}

	Position_f(int x, int y)
	{
		this->x = (float)x;
		this->y = (float)y;
	}

	friend bool operator==(const Position_f& lhs, const Position_f& rhs)
	{
		return (lhs.x == rhs.x) && (lhs.y == rhs.x);
	}

	friend bool operator<(const Position_f& lhs, const Position_f& rhs)
	{
		return (lhs.x < rhs.x) || ((lhs.x == rhs.x) && (lhs.y < rhs.y));
	}

	Vector2 ToVector()
	{
		return { x, y };
	}
};

struct Hitbox
{
	Rectangle area;
	Position* source;

	Hitbox()
	{
		area = { 0, 0 };
		source = new Position(0, 0);
	}

	Hitbox(Position* source, float width, float height)
	{
		this->source = source;
		area = { (float)source->x, (float)source->y, width, height };
	}

	bool IsTouching(Hitbox other)
	{
		return CheckCollisionRecs(area, other.area);
	}

	void UpdateHitbox()
	{
		area.x = (float)source->x;
		area.y = (float)source->y;
	}
};

enum ObjectType
{
	NONE,
	XP_ORB,
	ITEM,
	CHEST
};

enum InstanceType
{
	PLAYER,
	ZOMBIE,
	OBJECT
};

struct GlobalMapData
{
	int id;
	InstanceType type;
};

enum CardType
{
	SPEED = 0,
	DAMAGE,
	ATTACK_SPEED,
	HEALTH,
	PICKUP,
	THORN_AURA
};

struct CardEvent
{
	int selectedCard;
	bool cardAction;
	CardType type;
};

struct Card
{
	int id;
	Texture2D* image;
	const char* text;
	const char* description;
	Rectangle sourceRec;
	Rectangle bounds;
	int state = 0;
	CardType type;
};

struct MenuButton
{
	int id;
	const char* text;
	Rectangle sourceRec;
	Rectangle bounds;
	int state = 0;
};

struct CharacterSelectBox
{
	int id;
	const char* text;
	Rectangle sourceRec;
	Rectangle bounds;
	Texture2D* characterImage;
	int state = 0;
};

enum RenderState
{
	GAME,
	MAIN_MENU,
	OPTIONS,
	LEVEL_UP
};

enum SpellType
{
	SPELL_BOLT,
	SPELL_THORN_AURA
};

enum TextureName
{
	BACKGROUND_TEXTURE,
	PLAYER_TEXTURE,
	ZOMBIE_TEXTURE,
	XP_ORB_TEXTURE,
	CHEST_TEXTURE,
	CARD_TEXTURE,
	DAMAGE_UPGRADE_TEXTURE,
	HEALTH_UPGRADE_TEXTURE,
	MOVE_SPEED_UPGRADE_TEXTURE,
	FIRE_SPEED_UPGRADE_TEXTURE,
	PICKUP_UPGRADE_TEXTURE,
	THORN_AURA_TEXTURE,
	BUTTON_TEXTURE,
	MAIN_MENU_BACKGROUND_TEXTURE,
	ALT_MENU_BACKGROUND_TEXTURE,
	CHARACTER_SELECT_BORDER_TEXTURE
};

template<typename T>
struct Event
{
	int id;
	std::string name;
	int time;

	std::map<T, std::vector<int>> enemies;
};

// TODO: change enums to enum classes, seems to enable the use of the same enum values
enum EnemyType
{
	ENEMY_ZOMBIE
};

enum GameState
{
	RUNNING,
	PAUSED,
	PLAYER_DEAD,
	WIN
};
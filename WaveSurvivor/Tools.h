#pragma once
#include "Definitions.h"
#include "Player.h"

namespace Tools
{
	class Math
	{
	public:
		static long TickToMs(long tick);
		static int MsToS(long ms);
	};

	class Time
	{
	public:
		static long long GetCurrentEpocMs();
	};

	class ScreenSpace
	{
	public:
		static Position GetWorldToScreen(Position& target, Position p);
		static Position_f GetWorldToScreen_f(Position_f& target, Position p);
	};

	class Shape
	{
	public:
		static Position GetRectangleCenter(int startX, int startY, int width, int height);
		static Position_f GetRectangleCenter(float startX, float startY, int width, int height);
	};

	class Text
	{
	public:
		static int CenterTextX(int width, const char* text, int fontSize);
		static int CenterTextY(int height, const char* text, int fontSize);
	};

	class Random
	{
	public:
		// UNUSED TODO: remove
		static int GenerateRandomCoordinate(int start, int maxDistance);
		static Position GenerateRandomPosition(int minX, int maxX, int minY, int maxY);
		static int GenerateRandomSign();
	};
};


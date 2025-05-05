#pragma once
#include "Definitions.h"

template<typename T>
class Grid
{
	struct Cell
	{
		Rectangle area;
		std::vector<Rectangle> objects;
	};

public:
	Grid* BuildGrid(const std::vector<T>& objects);
	bool CheckCollision(Rectangle a, Rectangle b);
private:
	std::vector<Cell> cells;
};
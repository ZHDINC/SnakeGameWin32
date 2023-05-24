#pragma once
#include<vector>
#include<Windows.h>
#include"Point.h"

enum FacingDirection
{
	LEFT,
	UP,
	RIGHT,
	DOWN
};

class Snake
{
	std::vector<Point> snakePositions;
	FacingDirection currentFacingDirection = FacingDirection::LEFT;
	Point determineNewPosition(Point oldPoint, int keyboard);
	bool needToExtend = false;
public:
	Snake(int x, int y);
	void Extend();
	void MoveSnake(int keyboard);
	void DrawSnake(HDC hdc) const;
	Point HeadPosition() const;
};
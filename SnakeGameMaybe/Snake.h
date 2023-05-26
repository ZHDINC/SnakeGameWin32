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
	bool collision = false;
public:
	Snake(int x, int y);
	void Extend();
	bool MoveSnake(int keyboard);
	void DrawSnake(HDC hdc) const;
	std::vector<Point> GetSnakePositions() const;
	Point HeadPosition() const;
};
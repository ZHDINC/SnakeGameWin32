#pragma once
#include<Windows.h>
#include<random>
#include"Point.h"

class Apple
{
	int x;
	int y;
	std::random_device rd;
	std::mt19937 rng;
	std::uniform_int_distribution<> xdistribution;
	std::uniform_int_distribution<> ydistribution;
public:
	Apple() :
		rng(rd()),
		xdistribution(0, 35),
		ydistribution(0, 20)
	{

	}

	Point GetApplePosition() const
	{
		return Point{ x, y };
	}

	void Update()
	{
		x = xdistribution(rng) * 25;
		y = ydistribution(rng) * 25;
	}
	void Draw(HDC hdc)
	{
		Rectangle(hdc, x, y, x + 25, y + 25);
	}
};
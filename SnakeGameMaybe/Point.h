#pragma once

struct Point
{
	int x, y;
	void Update(Point p)
	{
		x = p.x;
		y = p.y;
	}
};
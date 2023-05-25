#include"Snake.h"

Point Snake::determineNewPosition(Point oldPoint, int keyboard)
{
	Point newPoint = oldPoint;
	switch(currentFacingDirection)
	{
	case FacingDirection::LEFT:
		switch (keyboard)
		{
		case VK_LEFT:
			newPoint.y += 25;
			currentFacingDirection = FacingDirection::DOWN;
			break;
		case VK_RIGHT:
			newPoint.y -= 25;
			currentFacingDirection = FacingDirection::UP;
			break;
		default:
			newPoint.x -= 25;
			break;
		}
		break;
	case FacingDirection::RIGHT:
		switch (keyboard)
		{
		case VK_LEFT:
			newPoint.y -= 25;
			currentFacingDirection = FacingDirection::UP;
			break;
		case VK_RIGHT:
			newPoint.y += 25;
			currentFacingDirection = FacingDirection::DOWN;
			break;
		default:
			newPoint.x += 25;
			break;
		}
		break;
	case FacingDirection::UP:
		switch (keyboard)
		{
		case VK_LEFT:
			newPoint.x -= 25;
			currentFacingDirection = FacingDirection::LEFT;
			break;
		case VK_RIGHT:
			newPoint.x += 25;
			currentFacingDirection = FacingDirection::RIGHT;
			break;
		default:
			newPoint.y -= 25;
			break;
		}
		break;
	case FacingDirection::DOWN:
		switch (keyboard)
		{
		case VK_RIGHT:
			newPoint.x -= 25;
			currentFacingDirection = FacingDirection::LEFT;
			break;
		case VK_LEFT:
			newPoint.x += 25;
			currentFacingDirection = FacingDirection::RIGHT;
			break;
		default:
			newPoint.y += 25;
			break;
		}
		break;
	}
	return newPoint;
}

Snake::Snake(int x, int y)
{
	snakePositions.push_back({ x, y });
	snakePositions.push_back({ x + 25, y });
	snakePositions.push_back({ x + 50, y });
}

void Snake::Extend()
{
	needToExtend = true;
}

bool Snake::MoveSnake(int keyboard)
{
	if (collision) return collision;
	Point temp = snakePositions[0];
	Point newPosition = determineNewPosition(temp, keyboard);
	for (Point p : snakePositions)
	{
		if (newPosition.x == p.x && newPosition.y == p.y)
		{
			collision = true;
			return collision;
		}
	}
	if (!collision)
	{
		Point extensionPoint;
		for (int i = 0; i < snakePositions.size(); i++)
		{
			snakePositions[i].Update(newPosition);
			newPosition = temp;
			if (i != snakePositions.size() - 1)
				temp = snakePositions[i + 1];
			if (needToExtend && (i == snakePositions.size() - 1))
			{
				extensionPoint = snakePositions[i];

			}
		}
		if (needToExtend)
		{
			snakePositions.push_back(extensionPoint);
			needToExtend = false;
		}
	}
	return collision;
}

void Snake::DrawSnake(HDC hdc) const
{
	for (int i = 0; i < snakePositions.size(); i++)
	{
		Rectangle(hdc, snakePositions[i].x, snakePositions[i].y, snakePositions[i].x + 25, snakePositions[i].y + 25);
	}
}

Point Snake::HeadPosition() const
{
	Point headPosition = snakePositions[0];
	return headPosition;
}

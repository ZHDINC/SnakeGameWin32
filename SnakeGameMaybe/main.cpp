#include<Windows.h>
#include"Snake.h"
#include"Apple.h"
#include"Point.h"
#include<random>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
constexpr int ITERATION = 1000;
constexpr int windowWidth = 900;
constexpr int windowHeight = 550;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	WNDCLASS wndclass = { };

	wndclass.lpszClassName = L"Snake";
	wndclass.lpfnWndProc = WndProc;
	wndclass.hInstance = hInstance;
	wndclass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wndclass.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wndclass.lpszMenuName = nullptr;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;

	RegisterClass(&wndclass);

	HWND hwnd = CreateWindow(wndclass.lpszClassName, L"Snake", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, windowWidth, windowHeight, nullptr, nullptr, hInstance, nullptr);
	SetTimer(hwnd, ITERATION, 250, nullptr);
	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);
	MSG msg;
	while (GetMessage(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

bool boundaryGameOver(const Snake& snake)
{
	Point headPosition = snake.HeadPosition();
	if (headPosition.x < 0 || headPosition.x > windowWidth)
	{
		return true;
	}
	if (headPosition.y < 0 || headPosition.y > windowHeight)
	{
		return true;
	}
	return false;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	static Snake snake(0,0);
	static Apple apple;
	static int eatenAppleCount = 0;
	static int speed = 250;
	static bool canMove = true;
	HPEN hpen;
	HBRUSH hbrush;
	HDC hdc;
	PAINTSTRUCT ps;
	static bool gameoverstate = false;
	std::wstring gameovertext = L"Game Over YEAHHHHHHHHHH";
	switch (message)
	{
	case WM_CREATE:
	{
		std::random_device rd;
		std::mt19937 rng(rd());
		std::uniform_int_distribution<> xdistribution(10, 30);
		std::uniform_int_distribution<> ydistribution(5, 15);
		snake = Snake{ xdistribution(rng) * 25, ydistribution(rng) * 25 };
		apple.Update(snake);
		return 0;
	}
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		hpen = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
		hbrush = CreateSolidBrush(RGB(0, 255, 0));
		SelectObject(hdc, hpen);
		SelectObject(hdc, hbrush);
		snake.Draw(hdc);
		hpen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
		hbrush = CreateSolidBrush(RGB(255, 0, 0));
		SelectObject(hdc, hpen);
		SelectObject(hdc, hbrush);
		apple.Draw(hdc);
		SelectObject(hdc, GetStockObject(BLACK_PEN));
		SelectObject(hdc, GetStockObject(WHITE_BRUSH));
		if (gameoverstate)
		{
			TextOut(hdc, 300, 300, gameovertext.c_str(), gameovertext.length());
		}
		EndPaint(hwnd, &ps);
		DeleteObject(hpen);
		DeleteObject(hbrush);
		return 0;
	case WM_KEYDOWN:
		if (canMove)
		{
			gameoverstate = snake.Move(wparam);
			if (!gameoverstate)
			{
				gameoverstate = boundaryGameOver(snake);
			}
			canMove = false;
		}
		return 0;
	case WM_KILLFOCUS:
		KillTimer(hwnd, ITERATION);
		return 0;
	case WM_SETFOCUS:
		SetTimer(hwnd, ITERATION, speed, nullptr);
		return 0;
	case WM_TIMER:
		if (gameoverstate)
		{
			KillTimer(hwnd, ITERATION);
		}
		if (!canMove)
		{
			canMove = true;
		}
		else
		{
			gameoverstate = snake.Move(0);
			if (!gameoverstate)
			{
				gameoverstate = boundaryGameOver(snake);
			}
		}
		if (snake.HeadPosition().x == apple.GetPosition().x && snake.HeadPosition().y == apple.GetPosition().y)
		{
			snake.Extend();
			apple.Update(snake);
			eatenAppleCount++;
			if (eatenAppleCount % 5 == 0)
			{
				speed = 250 - eatenAppleCount;
				if (speed < 50)
				{
					speed = 50;
				}
				SetTimer(hwnd, ITERATION, speed, nullptr);
			}
		}
		InvalidateRect(hwnd, NULL, TRUE);
		return 0;
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, message, wparam, lparam);
}
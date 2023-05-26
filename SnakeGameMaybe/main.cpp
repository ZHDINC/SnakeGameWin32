#include<Windows.h>
#include"Snake.h"
#include"Apple.h"
#include"Point.h"

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
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
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
	static Snake snake(300, 300);
	static Apple apple;
	static bool canMove = true;
	HPEN hpen;
	HDC hdc;
	PAINTSTRUCT ps;
	static bool gameoverstate = false;
	std::wstring gameovertext = L"Game Over YEAHHHHHHHHHH";
	switch (message)
	{
	case WM_CREATE:
		apple.Update(snake);
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		snake.DrawSnake(hdc);
		hpen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
		SelectObject(hdc, hpen);
		apple.Draw(hdc);
		SelectObject(hdc, GetStockObject(BLACK_PEN));
		if (gameoverstate)
		{
			TextOut(hdc, 300, 300, gameovertext.c_str(), gameovertext.length());
		}
		EndPaint(hwnd, &ps);
		DeleteObject(hpen);
		return 0;
	case WM_KEYDOWN:
		if (canMove)
		{
			gameoverstate = snake.MoveSnake(wparam);
			if (!gameoverstate)
			{
				gameoverstate = boundaryGameOver(snake);
			}
			canMove = false;
		}
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
			gameoverstate = snake.MoveSnake(0);
			if (!gameoverstate)
			{
				gameoverstate = boundaryGameOver(snake);
			}
		}
		if (snake.HeadPosition().x == apple.GetApplePosition().x && snake.HeadPosition().y == apple.GetApplePosition().y)
		{
			snake.Extend();
			apple.Update(snake);
		}
		InvalidateRect(hwnd, NULL, TRUE);
		return 0;
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, message, wparam, lparam);
}
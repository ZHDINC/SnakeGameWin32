#include<Windows.h>
#include"Snake.h"
#include"Apple.h"
#include"Point.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
constexpr int ITERATION = 1000;

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

	HWND hwnd = CreateWindow(wndclass.lpszClassName, L"Snake", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, nullptr, nullptr, hInstance, nullptr);
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

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	static Snake snake(300, 300);
	static Apple apple;
	static bool canMove = true;
	HPEN hpen;
	HDC hdc;
	PAINTSTRUCT ps;
	switch (message)
	{
	case WM_CREATE:
		apple.Update();
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		snake.DrawSnake(hdc);
		hpen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
		SelectObject(hdc, hpen);
		apple.Draw(hdc);
		SelectObject(hdc, GetStockObject(BLACK_PEN));
		EndPaint(hwnd, &ps);
		DeleteObject(hpen);
		return 0;
	case WM_KEYDOWN:
		if (canMove)
		{
			snake.MoveSnake(wparam);
			canMove = false;
		}
		return 0;
	case WM_TIMER:
		if (!canMove)
		{
			canMove = true;
		}
		else
		{
			snake.MoveSnake(0);
		}
		if (snake.HeadPosition().x == apple.GetApplePosition().x && snake.HeadPosition().y == apple.GetApplePosition().y)
		{
			snake.Extend();
			apple.Update();
		}
		InvalidateRect(hwnd, NULL, TRUE);
		return 0;
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, message, wparam, lparam);
}
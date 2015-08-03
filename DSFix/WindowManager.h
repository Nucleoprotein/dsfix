#pragma once

#include <Windows.h>

class WindowManager
{
	bool captureCursor, cursorVisible;

	bool borderlessFullscreen;
	RECT prevWindowRect;
	long prevStyle, prevExStyle;

	static HWND hWnd;
	static WNDPROC oldWndProc;

	static DWORD WINAPI FindWindowThread(LPVOID lpThreadParameter);
	static LRESULT CALLBACK DSFixWndProc(HWND, UINT, WPARAM, LPARAM);

public:
	static WindowManager& get()
	{
		static WindowManager instance;
		return instance;
	}

	WindowManager();

	HWND GetGameWindow() { return hWnd; };

	void applyCursorCapture();
	void toggleCursorCapture();
	void toggleCursorVisibility();
	void toggleBorderlessFullscreen();
	void resize(unsigned clientW, unsigned clientH);

};

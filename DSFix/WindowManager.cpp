#include "stdafx.h"

#include "WindowManager.h"
#include "KeyActions.h"
#include "SaveManager.h"

HWND WindowManager::hWnd;
WNDPROC WindowManager::oldWndProc;

DWORD WINAPI WindowManager::FindWindowThread(LPVOID lpThreadParameter)
{
	while (!oldWndProc)
	{
		hWnd = ::FindWindowA("DARK SOULS", "DARK SOULS");

		if (hWnd)
			oldWndProc = (WNDPROC)SetWindowLong(hWnd, GWL_WNDPROC, (LONG)DSFixWndProc);

		Sleep(1);
	}

	CloseHandle(GetCurrentThread());
	return 0;
}

LRESULT CALLBACK WindowManager::DSFixWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// always tick SaveManager
	SaveManager::get().tick();

	switch (uMsg)
	{
	case WM_ACTIVATE:
	{
		switch (LOWORD(wParam))
		{
		case WA_INACTIVE:
				return TRUE;
			break;
		}
	}

	default:
		WindowManager::get().applyCursorCapture();
		KeyActions::get().processIO();
		return CallWindowProc(oldWndProc, hWnd, uMsg, wParam, lParam);;
	}
}

WindowManager::WindowManager()
	:captureCursor(false), cursorVisible(true), borderlessFullscreen(false)
{
	CreateThread(NULL, NULL, FindWindowThread, NULL, NULL, NULL);
}

WindowManager::~WindowManager()
{
	if (oldWndProc)
		SetWindowLong(hWnd, GWL_WNDPROC, (LONG)oldWndProc);
}

void WindowManager::applyCursorCapture()
{
	if (captureCursor)
	{
		RECT clientrect;
		::GetClientRect(hWnd, &clientrect);
		::ClientToScreen(hWnd, (LPPOINT)&clientrect.left);
		::ClientToScreen(hWnd, (LPPOINT)&clientrect.right);
		::ClipCursor(&clientrect);
	}
	else
	{
		::ClipCursor(NULL);
	}
}

void WindowManager::toggleCursorCapture()
{
	captureCursor = !captureCursor;
}

void WindowManager::toggleCursorVisibility()
{
	cursorVisible = !cursorVisible;
	::ShowCursor(cursorVisible);
}

void WindowManager::toggleBorderlessFullscreen()
{
	borderlessFullscreen = !borderlessFullscreen;
	if (borderlessFullscreen)
	{
		// store previous rect
		::GetClientRect(hWnd, &prevWindowRect);
		// set styles
		LONG lStyle = ::GetWindowLong(hWnd, GWL_STYLE);
		prevStyle = lStyle;
		lStyle &= ~(WS_CAPTION | WS_THICKFRAME | WS_MINIMIZE | WS_MAXIMIZE | WS_SYSMENU);
		::SetWindowLong(hWnd, GWL_STYLE, lStyle);
		LONG lExStyle = ::GetWindowLong(hWnd, GWL_EXSTYLE);
		prevExStyle = lExStyle;
		lExStyle &= ~(WS_EX_DLGMODALFRAME | WS_EX_CLIENTEDGE | WS_EX_STATICEDGE);
		::SetWindowLong(hWnd, GWL_EXSTYLE, lExStyle);
		// adjust size & position
		HMONITOR monitor = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);
		MONITORINFO info;
		info.cbSize = sizeof(MONITORINFO);
		GetMonitorInfo(monitor, &info);
		int monitorWidth = info.rcMonitor.right - info.rcMonitor.left;
		int monitorHeight = info.rcMonitor.bottom - info.rcMonitor.top;
		::SetWindowPos(hWnd, NULL, info.rcMonitor.left, info.rcMonitor.top, monitorWidth, monitorHeight, SWP_FRAMECHANGED | SWP_NOZORDER | SWP_NOOWNERZORDER);
	}
	else
	{
		// restore previous window
		::SetWindowLong(hWnd, GWL_STYLE, prevStyle);
		::SetWindowLong(hWnd, GWL_EXSTYLE, prevExStyle);
		RECT desiredRect = prevWindowRect;
		::AdjustWindowRect(&desiredRect, prevStyle, false);
		int wWidth = desiredRect.right - desiredRect.left, wHeight = desiredRect.bottom - desiredRect.top;
		::SetWindowPos(hWnd, NULL, prevWindowRect.left, prevWindowRect.top, wWidth, wHeight, SWP_FRAMECHANGED | SWP_NOZORDER | SWP_NOOWNERZORDER);
	}
}

void WindowManager::resize(unsigned clientW, unsigned clientH)
{
	// Store current window rect
	::GetClientRect(hWnd, &prevWindowRect);
	// Get monitor size
	HMONITOR monitor = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);
	MONITORINFO info;
	info.cbSize = sizeof(MONITORINFO);
	GetMonitorInfo(monitor, &info);
	int monitorWidth = info.rcMonitor.right - info.rcMonitor.left;
	int monitorHeight = info.rcMonitor.bottom - info.rcMonitor.top;

	// How much do we overlap or are smaller than the actual screen size
	int widthDiff = monitorWidth - (clientW ? clientW : prevWindowRect.right);
	int heightDiff = monitorHeight - (clientH ? clientH : prevWindowRect.bottom);

	RECT desiredRect;
	desiredRect.left = widthDiff / 2;
	desiredRect.top = heightDiff / 2;
	desiredRect.right = monitorWidth - (widthDiff / 2);
	desiredRect.bottom = monitorHeight - (heightDiff / 2);
	LONG lStyle = ::GetWindowLong(hWnd, GWL_STYLE);
	::AdjustWindowRect(&desiredRect, lStyle, false);
	::SetWindowPos(hWnd, NULL, desiredRect.left, desiredRect.top, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
}

#include "stdafx.h"

#include "WindowManager.h"

DWORD WINAPI WindowManager::FindWindowThread(LPVOID lpThreadParameter)
{
	HWND* pWnd = (HWND*)lpThreadParameter;
	while (1)
	{
		HWND hWnd = ::FindWindowA("DARK SOULS", NULL);
		if (hWnd)
		{
			*pWnd = hWnd;
			break;
		}
		Sleep(1);
	}

	CloseHandle(GetCurrentThread());
	return 0;
}

WindowManager::WindowManager()
	:captureCursor(false), cursorVisible(true), borderlessFullscreen(false)
{
	CreateThread(NULL, NULL, FindWindowThread, &hWnd, NULL, NULL);
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

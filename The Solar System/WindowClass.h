#ifndef WINDOWCLASS_H
#define WINDOWCLASS_H

// INCLUDES //
#include <Windows.h>

class WindowClass
{
public:
	WindowClass();
	WindowClass(const WindowClass&);
	~WindowClass();

	bool Initialize(bool full_screen);
	void FullScreenON();
	void FullScreenOFF();
	void Shutdown();
	HWND GetWindowHandle();
	void GetClientAreaRect(RECT&);
	bool IsCursorShown();

private:
	bool m_FullScreen;
	const char *m_ClassName;
	HWND m_hwnd;
	HINSTANCE m_hInstance;
};

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

#endif
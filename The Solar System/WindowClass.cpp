#include "WindowClass.h"


WindowClass::WindowClass()
{
}

WindowClass::WindowClass(const WindowClass& other)
{
}

WindowClass::~WindowClass()
{
}

bool WindowClass::Initialize(bool full_screen)
{
	unsigned int
		WindowX, WindowY,
		WindowWidth, WindowHeight,
		ScreenWidth, ScreenHeight;
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	DWORD WindowStyle;


	// Set the screen mode
	m_FullScreen = full_screen;
	
	// Set the window class name
	m_ClassName = "Engine Class";

	// Get the instance of the application
	m_hInstance = GetModuleHandle(NULL);

	ScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	ScreenHeight = GetSystemMetrics(SM_CYSCREEN);

	// Fill the window class object
	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = m_hInstance;
	wc.lpszClassName = m_ClassName;
	wc.hIcon = (HICON)LoadImage(
		m_hInstance, "Data/Icon.ico", IMAGE_ICON,
		GetSystemMetrics(SM_CXICON), GetSystemMetrics(SM_CYICON), LR_LOADFROMFILE);
	wc.hIconSm = (HICON)LoadImage(
		m_hInstance, "Data/Icon.ico", IMAGE_ICON,
		GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON), LR_LOADFROMFILE);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(DEFAULT_PALETTE);
	wc.cbSize = sizeof(WNDCLASSEX);

	// Register the window class
	if (!RegisterClassEx(&wc))
		return false;

	if (m_FullScreen)
	{
		// If full screen set the screen to maximum size of the users desktop and 32bit.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)ScreenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)ScreenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display settings to full screen.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// Set window size
		WindowWidth = ScreenWidth;
		WindowHeight= ScreenHeight;

		// Set window position
		WindowX = WindowY = 0;

		// Set full screen window style
		WindowStyle = WS_POPUP | WS_SYSMENU;
	}
	else
	{
		// Go through all display modes to find the 
		// right resolution
		for(int iModeNum = 0; EnumDisplaySettings(NULL, iModeNum, &dmScreenSettings) != 0; iModeNum++)
		{			
			// Set window size
			WindowWidth = dmScreenSettings.dmPelsWidth;
			WindowHeight = dmScreenSettings.dmPelsHeight;	

			// If the resolution in the current display mode is 
			// in the same ratio of the screen resolution and
			// not the same, it breaks
			if(WindowHeight / (float)WindowWidth == ScreenHeight / (float)ScreenWidth &&
				WindowHeight != ScreenHeight && WindowWidth!= ScreenWidth)
				break;
		}

		// Set window position
		WindowX = (ScreenWidth - WindowWidth) / 2.0f;
		WindowY = (ScreenHeight - WindowHeight) / 2.0f;

		// Set windowed style
		WindowStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU;
	}

	m_hwnd = CreateWindowEx(
		0,                              // Optional window styles.
		m_ClassName,                    // Window class
		"Engine",						// Window text
		WindowStyle,					// Window style

		// Size and position
		WindowX, WindowY, WindowWidth, WindowHeight,

		NULL,       // Parent window    
		NULL,       // Menu
		m_hInstance,  // Instance handle
		NULL          // Additional application data
		);

	if (m_hwnd == NULL)
		return false;

	// Bring the window up on the screen and set it as main focus.
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	// Hide the mouse cursor.
	ShowCursor(false);
	SetCursorPos(WindowX + (WindowWidth) / 2.0f, WindowY + (WindowHeight) / 2.0f);


	return true;
}

void WindowClass::FullScreenON()
{
	unsigned int
		screenWidth, screenHeight;
	DEVMODE dmScreenSettings;

	if (!m_FullScreen)
	{
		screenWidth = GetSystemMetrics(SM_CXSCREEN);
		screenHeight = GetSystemMetrics(SM_CYSCREEN);

		// If full screen set the screen to maximum size of the users desktop and 32bit.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display settings to full screen.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// Change window style
		SetWindowLong(m_hwnd, GWL_STYLE, WS_POPUP | WS_SYSMENU);

		// Change window position
		SetWindowPos(m_hwnd, HWND_TOP, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), SWP_SHOWWINDOW);

		m_FullScreen = true;
	}

	return;
}

void WindowClass::FullScreenOFF()
{
	unsigned int 
		screenWidth, screenHeight,
		x, y, width, height;
	DEVMODE dmScreenSettings;


	if (m_FullScreen)
	{
		ChangeDisplaySettings(NULL, 0);

		screenWidth = GetSystemMetrics(SM_CXSCREEN);
		screenHeight = GetSystemMetrics(SM_CYSCREEN);

		// Go through all display modes to find the 
		// right resolution
		for(int iModeNum = 0; EnumDisplaySettings(NULL, iModeNum, &dmScreenSettings) != 0; iModeNum++)
		{			
			// Set window size
			width = dmScreenSettings.dmPelsWidth;
			height = dmScreenSettings.dmPelsHeight;	

			// If the resolution in the current display mode is 
			// in the same ratio of the screen resolution and
			// not the same, it breaks
			if(height / (float)width == screenHeight / (float)screenWidth &&
				height != screenHeight && width!= screenWidth)
				break;
		}

		x = (screenWidth - width) / 2.0f;
		y = (screenHeight - height) / 2.0f;

		// Change window style
		SetWindowLong(m_hwnd, GWL_STYLE, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU);
		
		// Change window position
		SetWindowPos(m_hwnd, HWND_TOP, x, y, width, height, SWP_SHOWWINDOW);
		
		m_FullScreen = false;

	}
	
	return;
}

void WindowClass::Shutdown()
{
	// Show the mouse cursor.
	ShowCursor(true);

	// Fix the display settings if leaving full screen mode.
	if (m_FullScreen)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	// Remove the window.
	m_hwnd = NULL;

	// Remove the application instance.
	UnregisterClass(m_ClassName, m_hInstance);
	m_hInstance = NULL;


	return;
}

HWND WindowClass::GetWindowHandle()
{
	return m_hwnd;
}

void WindowClass::GetClientAreaRect(RECT& rect)
{
	unsigned int thinBorder, thickBorder;
	RECT clientRect, windowRect;


	// Get client area rectangle
	GetClientRect(m_hwnd, &clientRect);

	// Get window rectangle
	GetWindowRect(m_hwnd, &windowRect);

	thinBorder = ((windowRect.right - windowRect.left) - clientRect.right) / 2.0f;

	thickBorder = ((windowRect.bottom - windowRect.top) - clientRect.bottom) - thinBorder;

	rect.top = windowRect.top + thickBorder;
	rect.bottom = rect.top + clientRect.bottom;
	rect.left = windowRect.left + thinBorder;
	rect.right = rect.left + clientRect.right;


	return;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_KEYDOWN:
		{
			switch (wParam)
			{
				case VK_ESCAPE:
				{
					DestroyWindow(hwnd);
					break;
				}
			}

			break;
		}

		case WM_CLOSE:
		{
			DestroyWindow(hwnd);
			break;
		}

		case WM_DESTROY:
		{
			PostQuitMessage(0);
			break;
		}
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}


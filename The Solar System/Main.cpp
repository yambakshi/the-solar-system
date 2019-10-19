// INCLUDES //
#include <Windows.h>

// MY INCLUDES //
#include "ApplicationClass.h"

int WINAPI WinMain(HINSTANCE hInstance,	HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// Create the application object
	ApplicationClass *m_Application = new ApplicationClass;
	if (!m_Application)
		return 0;

	// Initialize the application object
	if (!m_Application->Initialize())
		return 0;

	// Run the application
	m_Application->Run();

	// Shutdown the application	
	m_Application->Shutdown();
	delete m_Application;
	m_Application = 0;


	return 0;
}
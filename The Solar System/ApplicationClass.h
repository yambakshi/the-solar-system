#ifndef APPLICATIONCLASS_H
#define APPLICATIONCLASS_H

// MY INCLUDES //
#include "WindowClass.h"
#include "InputClass.h"
#include "DirectXClass.h"
#include "CameraClass.h"

#include "SphereModelClass.h"
#include "SunClass.h"
#include "PlanetClass.h"
#include "EarthClass.h"
#include "LightClass.h"

#include "EarthShaderClass.h"
#include "TextureShaderClass.h"
#include "BumpMapShaderClass.h"

// GLOBALS //
static bool FULL_SCREEN = true;
static bool SHOW_CURSOR = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

class ApplicationClass
{
public:
	ApplicationClass();
	ApplicationClass(const ApplicationClass&);
	~ApplicationClass();

	bool Initialize();
	void Run();
	void Shutdown();

private:
	// Methods
	bool InitializeFramework();
	bool InitializeObjects();
	bool InitializeShaders();
	void Update(MSG&);
	void Render();
	void RenderSun();
	void RenderEarth();

	// Members
	float m_scaleFactor;
	WindowClass *m_Window;
	InputClass *m_Input;
	DirectXClass *m_Device;
	CameraClass *m_Camera;

	LightClass *m_Light;
	SphereModelClass *m_Sphere;
	SunClass *m_Sun;
	EarthClass *m_Earth;
	PlanetClass *m_Moon;

	EarthShaderClass *m_EarthShader;
	BumpMapShaderClass *m_BumpMapShader;
	TextureShaderClass *m_TextureShader;
};

#endif
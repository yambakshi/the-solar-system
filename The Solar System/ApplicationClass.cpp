#include "ApplicationClass.h"


ApplicationClass::ApplicationClass()
{
	m_Window = 0;
	m_Input = 0;
	m_Device = 0;
	m_Camera = 0;

	m_Light = 0;
	m_Sphere = 0;
	m_Sun = 0;
	m_Earth = 0;
	m_Moon = 0;

	m_EarthShader = 0;
	m_BumpMapShader = 0;
	m_TextureShader = 0;
}

ApplicationClass::ApplicationClass(const ApplicationClass& other)
{
}

ApplicationClass::~ApplicationClass()
{
}

bool ApplicationClass::Initialize()
{
	if (!InitializeFramework())
		return false;

	if (!InitializeObjects())
		return false;

	if (!InitializeShaders())
		return false;
	
	// Initialize the scalar factor
	m_scaleFactor = 0.5f;

	return true;
}

bool ApplicationClass::InitializeFramework()
{
	RECT windowRect;


	// Create window object
	m_Window = new WindowClass;
	if (!m_Window)
		return false;

	// Initialize window object
	if (!m_Window->Initialize(FULL_SCREEN))
		return false;

	// Create input object
	m_Input = new InputClass;
	if (!m_Input)
		return false;

	// Initialize window object
	m_Input->Initialize();

	GetWindowRect(m_Window->GetWindowHandle(), &windowRect);

	m_Device = new DirectXClass;
	if (!m_Device)
		return false;

	if (!m_Device->Initialize(m_Window->GetWindowHandle(),
		(windowRect.right - windowRect.left), (windowRect.bottom - windowRect.top),
		VSYNC_ENABLED, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR))
		return false;


	return true;
}

bool ApplicationClass::InitializeObjects()
{
	// Create the camera object
	m_Camera = new CameraClass;
	if (!m_Camera)
		return false;

	// Set the camera position
	m_Camera->SetPosition(0.0f, 100.0f, 0.0f);
	m_Camera->SetRotation(90.0f, 0.0f, 0.0f);

	// Create the light object
	m_Light = new LightClass;
	if (!m_Light)
		return false;

	// Initialize light
	m_Light->SetDirection(0.0f, 0.0f, 1.0f);
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetAmbientColor(0.2f, 0.2f, 0.2f, 1.0f);
	m_Light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetSpecularPower(16.0f);

	// Create the earth object
	m_Sphere = new SphereModelClass;
	if (!m_Sphere)
		return false;
	
	// Initialize the model object
	if (!m_Sphere->Initialize(m_Device->GetDevice()))
		return false;

	// Create the sun object
	m_Sun = new SunClass;
	if (!m_Sun)
		return false;

	// Initialize the sun object
	if (!m_Sun->Initialize(m_Device->GetDevice(), m_Sphere, "Data/Textures/The Solar System/Sun.dds"))
		return false;

	// Create the earth object
	m_Earth = new EarthClass;
	if (!m_Earth)
		return false;

	// Initialize the earth object
	if (!m_Earth->Initialize(m_Device->GetDevice(), m_Sphere))
		return false;	
	
	// Create the moon object
	m_Moon = new PlanetClass;
	if (!m_Moon)
		return false;

	// Initialize the moon object
	if (!m_Moon->Initialize(m_Device->GetDevice(), m_Sphere,
		"Data/Textures/The Solar System/Earth/Moon.dds", "Data/Bumpmaps/The Solar System/Earth/Moon.dds"))
		return false;


	return true;
}

bool ApplicationClass::InitializeShaders()
{
	// Create the texture shader object
	m_EarthShader = new EarthShaderClass;
	if (!m_EarthShader)
		return false;

	// Initialize the texture shader object
	if (!m_EarthShader->Initialize(m_Device->GetDevice(), m_Window->GetWindowHandle()))
		return false;

	// Create the texture shader object
	m_BumpMapShader = new BumpMapShaderClass;
	if (!m_BumpMapShader)
		return false;

	// Initialize the texture shader object
	if (!m_BumpMapShader->Initialize(m_Device->GetDevice(), m_Window->GetWindowHandle()))
		return false;

	// Create the texture shader object
	m_TextureShader = new TextureShaderClass;
	if (!m_TextureShader)
		return false;

	// Initialize the texture shader object
	if (!m_TextureShader->Initialize(m_Device->GetDevice(), m_Window->GetWindowHandle()))
		return false;


	return true;
}

void ApplicationClass::Run()
{
	MSG msg;


	// Initialziethe message object
	ZeroMemory(&msg, sizeof(MSG));
	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (msg.message == WM_QUIT)
			break;
		else
		{
			Update(msg);
			Render();
		}
	}


	return;
}

void ApplicationClass::Update(MSG &msg)
{
	RECT windowRect;


	m_Input->Update(msg);

	if (m_Input->IsKeyDown((unsigned int)'F'))
	{
		m_Input->KeyUp((unsigned int)'F');
		
		// Toggle full screen and windowed mode
		if (FULL_SCREEN)
		{
			FULL_SCREEN = false;
			m_Window->FullScreenOFF();
		}
		else
		{
			FULL_SCREEN = true;
			m_Window->FullScreenON();
		}

		// Get window rectangle
		GetWindowRect(m_Window->GetWindowHandle(), &windowRect);

		m_Device->ResizeBackBuffer((windowRect.right - windowRect.left), (windowRect.bottom - windowRect.top),
			FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	}

	if (m_Input->IsKeyDown((unsigned int)'C'))
	{
		// Get window rectangle
		GetWindowRect(m_Window->GetWindowHandle(), &windowRect);

		m_Input->KeyUp((unsigned int)'C');

		if (SHOW_CURSOR)
			SHOW_CURSOR = false;
		else
			SHOW_CURSOR = true;

		SetCursorPos(windowRect.left + (windowRect.right - windowRect.left) / 2.0f, windowRect.top + (windowRect.bottom - windowRect.top) / 2.0f);
		ShowCursor(SHOW_CURSOR);
	}


	return;
}

void ApplicationClass::Render()
{	
	RECT windowRect;

	if (!SHOW_CURSOR)
	{
		// Get window rectangle
		GetWindowRect(m_Window->GetWindowHandle(), &windowRect);

		// Create the view matrix
		m_Camera->Render(m_Input,
			windowRect.left + (windowRect.right - windowRect.left) / 2.0f, windowRect.top + (windowRect.bottom - windowRect.top) / 2.0f);
	}


	// Begin rendering
	m_Device->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Render sun
	RenderSun();

	// Render earth and moon
	RenderEarth();

	m_Device->EndScene();

	return;
}

void ApplicationClass::RenderSun()
{
	D3DXMATRIX
		scale, rotation,
		worldMatrix, viewMatrix, projectionMatrix;


	// Get the matrices
	m_Device->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Device->GetProjectionMatrix(projectionMatrix);
	
	// Scales the sun by the scale factor
	// Sun's diameter = 109 * earth's diameter
	D3DXMatrixScaling(&scale, 109.0f * m_scaleFactor, 109.0f * m_scaleFactor, 109.0f * m_scaleFactor);

	// Apply the matrices in the correct order
	worldMatrix = worldMatrix * scale;
	
	// Render the sun
	m_Sun->Render(m_Device->GetDevice());

	m_TextureShader->Render(m_Device->GetDevice(), m_Sun->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_Sun->GetTexture());


	return;
}

void ApplicationClass::RenderEarth()
{
	static float
		aroundSun = 0.0f,
		earthRot = 0.0f,
		aroundEarth = 0.0f;
	float sunRadius, earthDiameter;
	D3DXVECTOR3 lightDirectionEarth, lightDirectionMoon, earthPos;
	D3DXMATRIX 
		worldMatrix, viewMatrix, projectionMatrix,
		rotationAroundSun, translateToEarth, translation, rotation, scale;


	// Get the matrices
	m_Device->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Device->GetProjectionMatrix(projectionMatrix);

	// Calculate the sun radius
	sunRadius = ((m_Sun->GetModelSize().x * 109.0f) / 2.0f);

	// EARTH
	// Translation matrix to earth's correct place in the orbit around the sun
	D3DXMatrixTranslation(&translation, (sunRadius + 50) * m_scaleFactor, 0.0f, 0.0f);

	// Rotation matrix to earth's rotation around itself
	D3DXMatrixRotationY(&rotation, earthRot*(D3DX_PI / 180.0f));
	
	// Increase the rotation angle of the earth around itself
	earthRot += (3.0f * m_scaleFactor);
	if (earthRot > 360.0f)
		earthRot -= 360.0f;

	// Scales the world to the correct earth size
	D3DXMatrixScaling(&scale, 1.0f * m_scaleFactor, 1.0f * m_scaleFactor, 1.0f * m_scaleFactor);

	// Rotation matrix to earth's rotation around itself
	D3DXMatrixRotationY(&rotationAroundSun, aroundSun*(D3DX_PI / 180.0f));

	// Increase the rotation angle of the earth around itself
	aroundSun += (0.1f * m_scaleFactor);
	if (aroundSun > 360.0f)
		aroundSun -= 360.0f;

	// Apply the matrices in the correct order
	worldMatrix = worldMatrix * scale * rotation * translation * rotationAroundSun;

	// Calculate earth pos
	D3DXVec3TransformCoord(&earthPos, &D3DXVECTOR3(0.0f, 0.0f, 0.0f), &worldMatrix);

	// Set the light Direction 
	D3DXVec3Normalize(&lightDirectionEarth, &earthPos);

	// Render earth
	m_Earth->Render(m_Device->GetDevice());

	m_EarthShader->Render(m_Device->GetDevice(), m_Earth->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_Earth->GetTexturesArray(),
		lightDirectionEarth, m_Light->GetDiffuseColor());

	// MOON
	// Get the original world matrix
	m_Device->GetWorldMatrix(worldMatrix);

	// Get earth's diameter
	earthDiameter = m_Earth->GetModelSize().x;

	// Translation matrix to the earth's location
	D3DXMatrixTranslation(&translateToEarth, earthPos.x, earthPos.y, earthPos.z);

	// Rotate the world
	D3DXMatrixRotationY(&rotation, aroundEarth*(D3DX_PI / 180.0f));

	// Increase the rotation angle of the moon around earth
	aroundEarth += (1.0f * m_scaleFactor);
	if (aroundEarth > 360.0f)
		aroundEarth -= 360.0f;

	// Translation matrix to the correct place in the orbit around earth
	D3DXMatrixTranslation(&translation, 3 * earthDiameter, 0.0f, 0.0f);

	// Scales the world to the correct moon size
	// Moon's diameter = 0.273 * earth's diameter
	D3DXMatrixScaling(&scale, 0.273f * m_scaleFactor, 0.273f * m_scaleFactor, 0.273f * m_scaleFactor);

	// Apply the matrices in the correct order
	worldMatrix = worldMatrix * scale * translation * rotation * translateToEarth;

	// Calculate the light direction relative to the moon
	D3DXVec3TransformCoord(&lightDirectionMoon, &D3DXVECTOR3(0.0f, 0.0f, 0.0f), &worldMatrix);

	// Set the light Direction 
	D3DXVec3Normalize(&lightDirectionMoon, &lightDirectionMoon);

	// Render the moon
	m_Moon->Render(m_Device->GetDevice());

	m_BumpMapShader->Render(m_Device->GetDevice(), m_Moon->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_Moon->GetTexturesArray(),
		lightDirectionMoon, m_Light->GetDiffuseColor(), m_Light->GetAmbientColor());
		

	return;
}

void ApplicationClass::Shutdown()
{
	// Framework
	if (m_Window)
	{
		m_Window->Shutdown();
		delete m_Window;
		m_Window = 0;
	}

	if (m_Input)
	{
		delete m_Input;
		m_Input = 0;
	}

	if (m_Device)
	{
		m_Device->Shutdown();
		delete m_Device;
		m_Device = 0;
	}

	// Objects
	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}


	return;
}
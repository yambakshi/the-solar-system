#include "CameraClass.h"


CameraClass::CameraClass()
{
	m_position.x = 0.0f;
	m_position.y = 0.0f;
	m_position.z = 0.0f;

	m_rotation.x = 0.0f;
	m_rotation.y = 0.0f;
	m_rotation.z = 0.0f;

	m_moveSpeed = 0.5f;
	m_rotationSpeed = 0.1f;	
}

CameraClass::CameraClass(const CameraClass& other)
{
}

void CameraClass::SetPosition(float x, float y, float z)
{
	m_position.x = x;
	m_position.y = y;
	m_position.z = z;


	return;
}

void CameraClass::SetRotation(float x, float y, float z)
{
	m_rotation.x = x;
	m_rotation.y = y;
	m_rotation.z = z;


	return;
}

D3DXVECTOR3 CameraClass::GetPosition()
{
	return m_position;
}

D3DXVECTOR3 CameraClass::GetRotation()
{
	return m_rotation;
}

void CameraClass::Render(InputClass *input, unsigned short windowCenterX, unsigned short windowCenterY)
{
	POINT cursorPos;
	float yaw, pitch, roll;
	D3DXVECTOR3 moveVector;
	D3DXMATRIX rotationMatrix;


	// Calculate rotation
	if (input->GetCurrentMessage().message == WM_MOUSEMOVE)
	{
		GetCursorPos(&cursorPos);
		if (cursorPos.x != windowCenterX || cursorPos.y != windowCenterY)
		{
			m_rotation.y += m_rotationSpeed * (cursorPos.x - windowCenterX);
			m_rotation.x += m_rotationSpeed * (cursorPos.y - windowCenterY);

			if (m_rotation.y > 360.0f)
				m_rotation.y -= 360.0f;

			if (m_rotation.x > 90.0f)
				m_rotation.x = 90.0f;
			else if (m_rotation.x < -90.0f)
				m_rotation.x = -90.0f;

			SetCursorPos(windowCenterX, windowCenterY);
		}
	}

	// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
	pitch = m_rotation.x * (D3DX_PI / 180.0f);
	yaw = m_rotation.y * (D3DX_PI / 180.0f);
	roll = m_rotation.z * (D3DX_PI / 180.0f); 

	// Create the rotation matrix from the yaw, pitch, and roll values.
	D3DXMatrixRotationYawPitchRoll(&rotationMatrix, yaw, pitch, roll);

	// Setup the vector that points upwards.
	m_up.x = 0.0f;
	m_up.y = 1.0f;
	m_up.z = 0.0f;

	// Setup where the camera is looking by default.
	m_target.x = 0.0f;
	m_target.y = 0.0f;
	m_target.z = 1.0f;

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	D3DXVec3TransformCoord(&m_target, &m_target, &rotationMatrix);
	D3DXVec3TransformCoord(&m_up, &m_up, &rotationMatrix);

	moveVector = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	if (input->IsKeyDown((unsigned int)'W'))
		moveVector.z += m_moveSpeed;
	else if (input->IsKeyDown((unsigned int)'S'))
		moveVector.z -= m_moveSpeed;
	if (input->IsKeyDown((unsigned int)'D'))
		moveVector.x += m_moveSpeed;
	else if (input->IsKeyDown((unsigned int)'A'))
		moveVector.x -= m_moveSpeed;
	
	D3DXVec3TransformCoord(&moveVector, &moveVector, &rotationMatrix);
	m_position += moveVector;

	// Translate the rotated camera position to the location of the viewer.
	m_target = m_position + m_target;

	// Finally create the view matrix from the three updated vectors.
	D3DXMatrixLookAtLH(&m_viewMatrix, &m_position, &m_target, &m_up);
	

	return;
}

void CameraClass::Render2D()
{
	D3DXVECTOR3 position, target, up;


	position = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	target = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

	up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	target = position + target;

	D3DXMatrixLookAtLH(&m_2DViewMatrix, &position, &target, &up); 


	return;
}

void CameraClass::RenderReflection(float height)
{	
	D3DXVECTOR3 reflectionUp, reflectionPosition, reflectionTarget;
	

	// Setup the vector that points upwards.
	reflectionUp.x = 0.0f;
	reflectionUp.y = 1.0f;
	reflectionUp.z = 0.0f;
	
	// Setup the position of the camera in the world.
	// For planar reflection invert the Y position of the camera.
	reflectionPosition.x = m_position.x;
	reflectionPosition.y = -m_position.y + (height * 2.0f);
	reflectionPosition.z = m_position.z;

	// Setup where the camera is looking.
	reflectionTarget.x = m_target.x;
	reflectionTarget.y = -m_target.y + (height * 2.0f);
	reflectionTarget.z = m_target.z;
	
	// Create the reflection view matrix from the three vectors.
	D3DXMatrixLookAtLH(&m_reflectionViewMatrix, &reflectionPosition, &reflectionTarget, &reflectionUp);
	

	return;
}

void CameraClass::GetViewMatrix(D3DXMATRIX& viewMatrix)
{
	viewMatrix = m_viewMatrix;


	return;
}

void CameraClass::Get2DViewMatrix(D3DXMATRIX& viewMatrix2D)
{
	viewMatrix2D = m_2DViewMatrix;

	return;
}

void CameraClass::GetReflectionViewMatrix(D3DXMATRIX& reflectionViewMatrix)
{
	reflectionViewMatrix = m_reflectionViewMatrix;

	return;
}

CameraClass::~CameraClass()
{
}

#ifndef CAMERACLASS_H
#define CAMERACLASS_H

// INCLUDES //
#include <d3dx10math.h>

// My INCLUDES //
#include "InputClass.h"

class CameraClass
{
public:
	CameraClass();
	CameraClass(const CameraClass&);
	~CameraClass();

	void SetPosition(float, float, float);
	void SetRotation(float, float, float);

	D3DXVECTOR3 GetPosition();
	D3DXVECTOR3 GetRotation();

	void Render(InputClass *input, unsigned short windowCenterX, unsigned short windowCenterY);
	void GetViewMatrix(D3DXMATRIX& viewMatrix);

	void RenderReflection(float);
	void GetReflectionViewMatrix(D3DXMATRIX& reflectionViewMatrix);

	void Render2D();
	void Get2DViewMatrix(D3DXMATRIX& viewMatrix2D);

private:
	float
		m_rotationSpeed,
		m_moveSpeed;
	D3DXVECTOR3 
		m_position, m_target, m_up,		
		m_rotation;
	D3DXMATRIX 
		m_viewMatrix,
		m_2DViewMatrix,
		m_reflectionViewMatrix;
};

#endif


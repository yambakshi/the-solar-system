#ifndef LIGHTCLASS_H
#define LIGHTCLASS_H

// INCLUDES //
#include <D3DX10.h>

class LightClass
{
public:
	LightClass();
	LightClass(const LightClass&);
	~LightClass();

	void SetDirection(float x, float y, float z);
	void SetDiffuseColor(float r, float g, float b, float a);
	void SetAmbientColor(float r, float g, float b, float a);
	void SetSpecularColor(float r, float g, float b, float a);
	void SetSpecularPower(float power);

	D3DXVECTOR3 GetDirection();
	D3DXCOLOR GetDiffuseColor();
	D3DXCOLOR GetAmbientColor();
	D3DXCOLOR GetSpecularColor();
	float GetSpecularPower();

private:
	D3DXVECTOR3 m_lightDirection;
	D3DXCOLOR m_diffuseColor, m_ambientColor, m_specularColor;
	float m_specularPower;
};

#endif
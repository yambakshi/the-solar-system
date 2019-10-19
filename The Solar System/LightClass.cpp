#include "LightClass.h"


LightClass::LightClass()
{
}

LightClass::LightClass(const LightClass& other)
{
}

LightClass::~LightClass()
{
}

void LightClass::SetDirection(float x, float y, float z)
{
	m_lightDirection = D3DXVECTOR3(x, y, z);
	return;
}

void LightClass::SetDiffuseColor(float r, float g, float b, float a)
{
	m_diffuseColor = D3DXCOLOR(r, g, b, a);
	return;
}

void LightClass::SetAmbientColor(float r, float g, float b, float a)
{
	m_ambientColor = D3DXCOLOR(r, g, b, a);
	return;
}

void LightClass::SetSpecularColor(float r, float g, float b, float a)
{
	m_specularColor = D3DXCOLOR(r, g, b, a);
	return;
}

void LightClass::SetSpecularPower(float power)
{
	m_specularPower = power;
	return;
}

D3DXVECTOR3 LightClass::GetDirection()
{
	return m_lightDirection;
}

D3DXCOLOR LightClass::GetDiffuseColor()
{
	return m_diffuseColor;
}

D3DXCOLOR LightClass::GetAmbientColor()
{
	return m_ambientColor;
}

D3DXCOLOR LightClass::GetSpecularColor()
{
	return m_specularColor;
}

float LightClass::GetSpecularPower()
{
	return m_specularPower;
}

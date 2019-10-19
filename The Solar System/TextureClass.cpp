#include "TextureClass.h"


TextureClass::TextureClass()
{
	m_texture = 0;
}

TextureClass::TextureClass(const TextureClass& other){}

bool TextureClass::Initialize(ID3D10Device* device, LPCSTR filename)
{
	if(FAILED(D3DX10CreateShaderResourceViewFromFile(device, filename, NULL, NULL, &m_texture, NULL)))
		return false;

	return true;
}

void TextureClass::Shutdown()
{
	if(m_texture)
	{
		m_texture->Release();
		m_texture = 0;
	}

	return;
}

ID3D10ShaderResourceView* TextureClass::GetTexture()
{
	return m_texture;
}

TextureClass::~TextureClass()
{
}

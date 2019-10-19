#ifndef TEXTURECLASS_H
#define TEXTURECLASS_H

#include <D3D10.h>
#include <D3DX10.h>

class TextureClass
{
public:
	TextureClass();
	TextureClass(const TextureClass&);
	~TextureClass();

	bool Initialize(ID3D10Device*, LPCSTR);
	void Shutdown();

	ID3D10ShaderResourceView* GetTexture();

private:
	ID3D10ShaderResourceView* m_texture;
};

#endif


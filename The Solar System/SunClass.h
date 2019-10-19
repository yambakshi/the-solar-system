#ifndef SUNCLASS_H
#define SUNCLASS_H

// INCLUDES //
#include <D3DX10.h>
#include <fstream>
using namespace std;

// MY INCLUDES //
#include "SphereModelClass.h"
#include "TextureClass.h"

class SunClass
{
private:
	struct SunType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture; 
		D3DXVECTOR3 normal;
		D3DXVECTOR3 tangent;
		D3DXVECTOR3 binormal;
	};
public:
	SunClass();
	SunClass(const SunClass&);
	~SunClass();

	bool Initialize(ID3D10Device *device, SphereModelClass* sphere, char *textureFilename);
	void Render(ID3D10Device *device);
	void Shutdown();

	ID3D10ShaderResourceView* GetTexture();
	unsigned long GetIndexCount();
	D3DXVECTOR3 GetModelSize();
	D3DXVECTOR3 GetModelCenter();

private:
	// Methods
	bool LoadTexture(ID3D10Device*, char*);
	void ReleaseTexture();

	// Members
	unsigned long m_VertexCount, m_IndexCount;
	SunType *m_Model;
	TextureClass *m_Texture;
	D3DXVECTOR3 m_ModelSize, m_ModelCenter;
	ID3D10Buffer *m_VertexBuffer, *m_IndexBuffer;
};

#endif
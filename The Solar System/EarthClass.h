#ifndef EARTHCLASS_H
#define EARTHCLASS_H

// INCLUDES //
#include <fstream>
#include <D3DX10.h>
using namespace std;

// MY INCLUDES //
#include "SphereModelClass.h"
#include "TextureClass.h"

class EarthClass
{
private:
	struct EarthType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
		D3DXVECTOR3 normal;
		D3DXVECTOR3 tangent;
		D3DXVECTOR3 binormal;
	};

public:
	EarthClass();
	EarthClass(const EarthClass&);
	~EarthClass();

	bool Initialize(ID3D10Device *device, SphereModelClass *sphere);
	void Render(ID3D10Device *device);
	void Shutdown();

	ID3D10ShaderResourceView** GetTexturesArray();
	unsigned long GetIndexCount();
	D3DXVECTOR3 GetModelCenter();
	D3DXVECTOR3 GetModelSize();	

private:
	// Methods
	bool LoadTextures(ID3D10Device*, char*, char*, char*, char*, char*);
	void ReleaseTexture();

	// Members
	unsigned long m_VertexCount, m_IndexCount;
	EarthType *m_Model;
	D3DXVECTOR3 m_ModelCenter, m_ModelSize;
	ID3D10ShaderResourceView **m_TexturesArray;
	ID3D10Buffer *m_VertexBuffer, *m_IndexBuffer;

};

#endif
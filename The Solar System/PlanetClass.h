#ifndef PLANETCLASS_H
#define PLANETCLASS_H

// INCLUDES //
#include <fstream>
#include <D3DX10.h>
using namespace std;

// MY INCLUDES //
#include "SphereModelClass.h"
#include "TextureClass.h"

class PlanetClass
{
private:
	struct PlanetType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
		D3DXVECTOR3 normal;
		D3DXVECTOR3 tangent;
		D3DXVECTOR3 binormal;
	};

public:
	PlanetClass();
	PlanetClass(const PlanetClass&);
	~PlanetClass();

	bool Initialize(ID3D10Device *device, SphereModelClass* sphere, char *textureFilename, char *bumpMapFilename);
	void Render(ID3D10Device *device);
	void Shutdown();

	ID3D10ShaderResourceView** GetTexturesArray();
	unsigned long GetIndexCount();
	D3DXVECTOR3 GetModelSize();
	D3DXVECTOR3 GetModelCenter();

private:
	// Methods
	bool LoadTextures(ID3D10Device*, char*, char*);
	void ReleaseTexture();

	// Members
	unsigned long m_VertexCount, m_IndexCount;
	PlanetType *m_Model;
	ID3D10ShaderResourceView **m_TexturesArray;
	D3DXVECTOR3 m_ModelSize, m_ModelCenter;
	ID3D10Buffer *m_VertexBuffer, *m_IndexBuffer;

};

#endif
#ifndef SPHEREMODELCLASS_H
#define SPHEREMODELCLASS_H

// INCLUDES //
#include <fstream>
#include <D3DX10.h>
using namespace std;

class SphereModelClass
{
public:
	struct SphereModelType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
		D3DXVECTOR3 normal;
		D3DXVECTOR3 tangent;
		D3DXVECTOR3 binormal;
	}; 
	struct TempVertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
		D3DXVECTOR3 normal;
	};
public:
	SphereModelClass();
	SphereModelClass(const SphereModelClass&);
	~SphereModelClass();

	bool Initialize(ID3D10Device *device);
	void Shutdown();

	D3DXVECTOR3 GetModelCenter();
	D3DXVECTOR3 GetModelSize();
	unsigned long GetVerticesCount();
	ID3D10Buffer* GetVertexBufferPtr();
	ID3D10Buffer* GetIndexBufferPtr();

	SphereModelType *m_Model;

private:
	// Methods
	bool LoadModelFromTXT(char*); 
	bool InitializeBuffers(ID3D10Device*);
	void CalculateModelVectors();
	void CalculateTangentBinormal(TempVertexType, TempVertexType, TempVertexType, D3DXVECTOR3&, D3DXVECTOR3&);
	void CalculateNormal(D3DXVECTOR3, D3DXVECTOR3, D3DXVECTOR3&);
	void CalculateModelSize();

	// Members
	unsigned long m_VertexCount, m_IndexCount;
	D3DXVECTOR3 m_ModelCenter, m_ModelSize;
	ID3D10Buffer *m_VertexBuffer, *m_IndexBuffer;
};

#endif
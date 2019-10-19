#include "PlanetClass.h"


PlanetClass::PlanetClass()
{
	m_Model = 0;
	m_TexturesArray = 0;
	m_VertexBuffer = 0;
	m_IndexBuffer = 0;
}

PlanetClass::PlanetClass(const PlanetClass& other)
{
}

PlanetClass::~PlanetClass()
{
}

bool PlanetClass::Initialize(ID3D10Device *device, SphereModelClass* sphere, char *textureFilename, char *bumpMapFilename)
{
	m_VertexCount = m_IndexCount = sphere->GetVerticesCount();
	m_ModelCenter = sphere->GetModelCenter();
	m_ModelSize = sphere->GetModelSize();
	m_Model = (PlanetType*)sphere->m_Model;
	m_VertexBuffer = sphere->GetVertexBufferPtr();
	m_IndexBuffer = sphere->GetIndexBufferPtr();

	if (!LoadTextures(device, textureFilename, bumpMapFilename))
		return false;


	return true;
}

bool PlanetClass::LoadTextures(ID3D10Device *device, char *textureFilename, char *bumpMapFilename)
{
	HRESULT result;

	// Create the texture object
	m_TexturesArray = new ID3D10ShaderResourceView*[2];
	if (!m_TexturesArray)
		return false;
	
	// Load the texture in
	result = D3DX10CreateShaderResourceViewFromFile(device, textureFilename, NULL, NULL, &m_TexturesArray[0], NULL);
	if (FAILED(result))
		return false;

	// Load the texture in
	result = D3DX10CreateShaderResourceViewFromFile(device, bumpMapFilename, NULL, NULL, &m_TexturesArray[1], NULL);
	if (FAILED(result))
		return false;

	return true;
}

void PlanetClass::Render(ID3D10Device *device)
{
	unsigned int stride, offset;

	// Set stride and offset
	stride = sizeof(PlanetType);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it could be rendered
	device->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it could be rendered
	device->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	device->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	return;
}

void PlanetClass::Shutdown()
{
	m_Model = 0;

	ReleaseTexture();


	return;
}

void PlanetClass::ReleaseTexture()
{
	if (m_TexturesArray)
	{
		delete[] m_TexturesArray;
		m_TexturesArray = 0;
	}

	return;
}

unsigned long PlanetClass::GetIndexCount()
{
	return m_IndexCount;
}

ID3D10ShaderResourceView** PlanetClass::GetTexturesArray()
{
	return m_TexturesArray;
}

D3DXVECTOR3 PlanetClass::GetModelSize()
{
	return m_ModelSize;
}

D3DXVECTOR3 PlanetClass::GetModelCenter()
{
	return m_ModelCenter;
}

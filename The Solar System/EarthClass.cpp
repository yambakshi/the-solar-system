#include "EarthClass.h"


EarthClass::EarthClass()
{
	m_Model = 0;
	m_TexturesArray = 0;
	m_VertexBuffer = 0;
	m_IndexBuffer = 0;
}

EarthClass::EarthClass(const EarthClass& other)
{
}

EarthClass::~EarthClass()
{
}

bool EarthClass::Initialize(ID3D10Device *device, SphereModelClass *sphere)
{
	m_VertexCount = m_IndexCount = sphere->GetVerticesCount();
	m_ModelCenter = sphere->GetModelCenter();
	m_ModelSize = sphere->GetModelSize();
	m_Model = (EarthType*)sphere->m_Model;
	m_VertexBuffer = sphere->GetVertexBufferPtr();
	m_IndexBuffer = sphere->GetIndexBufferPtr();

	if (!LoadTextures(device, 
		"Data/Textures/The Solar System/Earth/Earth.dds", 
		"Data/Textures/The Solar System/Earth/Earth_Night.dds",
		"Data/Textures/The Solar System/Earth/Earth_Clouds.dds",
		"Data/Bumpmaps/The Solar System/Earth/Earth.dds",
		"Data/Specmaps/The Solar System/Earth/Earth.dds"))
		return false;


	return true;
}

bool EarthClass::LoadTextures(ID3D10Device *device, char *textureFilename, char *nightTextureFilename, char *cloudsTextureFileName, char *bumpMapFilename, char *specMapFilename)
{
	HRESULT result;

	// Create the texture object
	m_TexturesArray = new ID3D10ShaderResourceView*[5];
	if (!m_TexturesArray)
		return false;
	
	// Load the texture in
	result = D3DX10CreateShaderResourceViewFromFile(device, textureFilename, NULL, NULL, &m_TexturesArray[0], NULL);
	if (FAILED(result))
		return false;

	// Load the texture in
	result = D3DX10CreateShaderResourceViewFromFile(device, nightTextureFilename, NULL, NULL, &m_TexturesArray[1], NULL);
	if (FAILED(result))
		return false;

	// Load the texture in
	result = D3DX10CreateShaderResourceViewFromFile(device, cloudsTextureFileName, NULL, NULL, &m_TexturesArray[2], NULL);
	if (FAILED(result))
		return false;

	// Load the texture in
	result = D3DX10CreateShaderResourceViewFromFile(device, bumpMapFilename, NULL, NULL, &m_TexturesArray[3], NULL);
	if (FAILED(result))
		return false;

	// Load the texture in
	result = D3DX10CreateShaderResourceViewFromFile(device, specMapFilename, NULL, NULL, &m_TexturesArray[4], NULL);
	if (FAILED(result))
		return false;

	return true;
}

void EarthClass::Render(ID3D10Device *device)
{
	unsigned int stride, offset;

	// Set stride and offset
	stride = sizeof(EarthType);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it could be rendered
	device->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it could be rendered
	device->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	device->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	return;
}

void EarthClass::Shutdown()
{
	m_Model = 0;

	ReleaseTexture();


	return;
}

void EarthClass::ReleaseTexture()
{
	if (m_TexturesArray)
	{
		delete[] m_TexturesArray;
		m_TexturesArray = 0;
	}

	return;
}

unsigned long EarthClass::GetIndexCount()
{
	return m_IndexCount;
}

ID3D10ShaderResourceView** EarthClass::GetTexturesArray()
{
	return m_TexturesArray;
}

D3DXVECTOR3 EarthClass::GetModelSize()
{
	return m_ModelSize;
}

D3DXVECTOR3 EarthClass::GetModelCenter()
{
	return m_ModelCenter;
}
#include "SunClass.h"


SunClass::SunClass()
{
	m_Model = 0;
	m_Texture = 0;
	m_VertexBuffer = 0;
	m_IndexBuffer = 0;
}

SunClass::SunClass(const SunClass& other)
{
}

SunClass::~SunClass()
{
}

bool SunClass::Initialize(ID3D10Device *device, SphereModelClass* sphere, char *textureFilename)
{
	m_VertexCount = m_IndexCount = sphere->GetVerticesCount();
	m_ModelCenter = sphere->GetModelCenter();
	m_ModelSize = sphere->GetModelSize();
	m_Model = (SunType*)sphere->m_Model;
	m_VertexBuffer = sphere->GetVertexBufferPtr();
	m_IndexBuffer = sphere->GetIndexBufferPtr();

	if (!LoadTexture(device, textureFilename))
		return false;


	return true;
}

bool SunClass::LoadTexture(ID3D10Device *device, char *textureFilename)
{
	// Create the texture object
	m_Texture = new TextureClass;
	if (!m_Texture)
		return false;

	// Initialize the texture object
	if (!m_Texture->Initialize(device, textureFilename))
		return false;


	return true;
}

void SunClass::Render(ID3D10Device *device)
{
	unsigned int stride, offset;

	// Set stride and offset
	stride = sizeof(SunType);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it could be rendered
	device->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it could be rendered
	device->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	device->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	return;
}

void SunClass::Shutdown()
{
	m_Model = 0;

	ReleaseTexture();


	return;
}

void SunClass::ReleaseTexture()
{
	if (m_Texture)
	{
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = 0;
	}

	return;
}

unsigned long SunClass::GetIndexCount()
{
	return m_IndexCount;
}

ID3D10ShaderResourceView* SunClass::GetTexture()
{
	return m_Texture->GetTexture();
}

D3DXVECTOR3 SunClass::GetModelSize()
{
	return m_ModelSize;
}

D3DXVECTOR3 SunClass::GetModelCenter()
{
	return m_ModelCenter;
}
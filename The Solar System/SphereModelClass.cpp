#include "SphereModelClass.h"


SphereModelClass::SphereModelClass()
{
	m_Model = 0;
}

SphereModelClass::SphereModelClass(const SphereModelClass& other)
{
}

SphereModelClass::~SphereModelClass()
{
}

bool SphereModelClass::Initialize(ID3D10Device *device)
{
	if (!LoadModelFromTXT("Data/Models/Sphere.txt"))
		return false;

	// Calculate the normal, tangent, and binormal vectors for the model.
	CalculateModelVectors();

	CalculateModelSize();

	if (!InitializeBuffers(device))
		return false;

	return true;
}

bool SphereModelClass::LoadModelFromTXT(char* modelFilename)
{
	char input;
	int i;
	ifstream file;


	// Count the vertices, texCoords, normals and faces
	file.open(modelFilename, ios::binary);
	if (file.fail())
		return false;

	// Go to the vertex count
	file.get(input);
	while (input != ':')
		file.get(input);

	// Get the vertex count
	file >> m_VertexCount;

	// Set the index count
	m_IndexCount = m_VertexCount;

	// Create the model
	m_Model = new SphereModelType[m_VertexCount];
	if (!m_Model)
		return false;

	// Go to the data
	file.get(input);
	while (input != ':')
		file.get(input);

	// Fill the model array
	i = 0;
	while (!file.eof())
	{
		file >>
			m_Model[i].position.x >> m_Model[i].position.y >> m_Model[i].position.z >>
			m_Model[i].texture.x >> m_Model[i].texture.y >>
			m_Model[i].normal.x >> m_Model[i].normal.y >> m_Model[i].normal.z;

		i++;
	}

	file.close();


	return true;
}

void SphereModelClass::CalculateModelVectors()
{
	int faceCount, i, index;
	TempVertexType vertex1, vertex2, vertex3;
	D3DXVECTOR3 tangent, binormal, normal;


	// Calculate the number of faces in the model.
	faceCount = m_VertexCount / 3;

	// Initialize the index to the model data.
	index = 0;

	// Go through all the faces and calculate the the tangent, binormal, and normal vectors.
	for (i = 0; i<faceCount; i++)
	{
		// Get the three vertices for this face from the model.
		vertex1.position = m_Model[index].position;
		vertex1.texture = m_Model[index].texture;
		vertex1.normal = m_Model[index].normal;
		index++;

		vertex2.position = m_Model[index].position;
		vertex2.texture = m_Model[index].texture;
		vertex2.normal = m_Model[index].normal;
		index++;

		vertex3.position = m_Model[index].position;
		vertex3.texture = m_Model[index].texture;
		vertex3.normal = m_Model[index].normal;
		index++;

		// Calculate the tangent and binormal of that face.
		CalculateTangentBinormal(vertex1, vertex2, vertex3, tangent, binormal);

		// Calculate the new normal using the tangent and binormal.
		CalculateNormal(tangent, binormal, normal);

		// Store the normal, tangent, and binormal for this face back in the model structure.
		//m_Model[index - 1].normal = normal;
		m_Model[index - 1].tangent = tangent;
		m_Model[index - 1].binormal = binormal;

		//m_Model[index - 2].normal = normal;
		m_Model[index - 2].tangent = tangent;
		m_Model[index - 2].binormal = binormal;

		//m_Model[index - 3].normal = normal;
		m_Model[index - 3].tangent = tangent;
		m_Model[index - 3].binormal = binormal;
	}

	return;
}

void SphereModelClass::CalculateTangentBinormal(TempVertexType vertex1, TempVertexType vertex2, TempVertexType vertex3,
	D3DXVECTOR3& tangent, D3DXVECTOR3& binormal)
{
	float vector1[3], vector2[3];
	float tuVector[2], tvVector[2];
	float den;
	float length;


	// Calculate the two vectors for this face.
	vector1[0] = vertex2.position.x - vertex1.position.x;
	vector1[1] = vertex2.position.y - vertex1.position.y;
	vector1[2] = vertex2.position.z - vertex1.position.z;

	vector2[0] = vertex3.position.x - vertex1.position.x;
	vector2[1] = vertex3.position.y - vertex1.position.y;
	vector2[2] = vertex3.position.z - vertex1.position.z;

	// Calculate the tu and tv texture space vectors.
	tuVector[0] = vertex2.texture.x - vertex1.texture.x;
	tvVector[0] = vertex2.texture.y - vertex1.texture.y;

	tuVector[1] = vertex3.texture.x - vertex1.texture.x;
	tvVector[1] = vertex3.texture.y - vertex1.texture.y;

	// Calculate the denominator of the tangent/binormal equation.
	den = 1.0f / (tuVector[0] * tvVector[1] - tuVector[1] * tvVector[0]);

	// Calculate the cross products and multiply by the coefficient to get the tangent and binormal.
	tangent.x = (tvVector[1] * vector1[0] - tvVector[0] * vector2[0]) * den;
	tangent.y = (tvVector[1] * vector1[1] - tvVector[0] * vector2[1]) * den;
	tangent.z = (tvVector[1] * vector1[2] - tvVector[0] * vector2[2]) * den;

	binormal.x = (tuVector[0] * vector2[0] - tuVector[1] * vector1[0]) * den;
	binormal.y = (tuVector[0] * vector2[1] - tuVector[1] * vector1[1]) * den;
	binormal.z = (tuVector[0] * vector2[2] - tuVector[1] * vector1[2]) * den;

	// Calculate the length of this normal.
	length = sqrt((tangent.x * tangent.x) + (tangent.y * tangent.y) + (tangent.z * tangent.z));

	// Normalize the normal and then store it
	tangent.x = tangent.x / length;
	tangent.y = tangent.y / length;
	tangent.z = tangent.z / length;

	// Calculate the length of this normal.
	length = sqrt((binormal.x * binormal.x) + (binormal.y * binormal.y) + (binormal.z * binormal.z));

	// Normalize the normal and then store it
	binormal.x = binormal.x / length;
	binormal.y = binormal.y / length;
	binormal.z = binormal.z / length;

	return;
}

void SphereModelClass::CalculateNormal(D3DXVECTOR3 tangent, D3DXVECTOR3 binormal, D3DXVECTOR3& normal)
{
	float length;


	// Calculate the cross product of the tangent and binormal which will give the normal vector.
	normal.x = (tangent.y * binormal.z) - (tangent.z * binormal.y);
	normal.y = (tangent.z * binormal.x) - (tangent.x * binormal.z);
	normal.z = (tangent.x * binormal.y) - (tangent.y * binormal.x);

	// Calculate the length of the normal.
	length = sqrt((normal.x * normal.x) + (normal.y * normal.y) + (normal.z * normal.z));

	// Normalize the normal.
	normal.x = normal.x / length;
	normal.y = normal.y / length;
	normal.z = normal.z / length;

	return;
}

void SphereModelClass::CalculateModelSize()
{
	float
		maxX, maxY, maxZ,
		minX, minY, minZ;



	maxX = maxY = maxZ = 0;
	minX = minY = minZ = 0;
	for (int i = 0; i < m_VertexCount; i++)
	{
		if (maxX < m_Model[i].position.x)
			maxX = m_Model[i].position.x;
		if (minX > m_Model[i].position.x)
			minX = m_Model[i].position.x;

		if (maxY < m_Model[i].position.y)
			maxY = m_Model[i].position.y;
		if (minY > m_Model[i].position.y)
			minY = m_Model[i].position.y;

		if (maxZ < m_Model[i].position.z)
			maxZ = m_Model[i].position.z;
		if (minZ > m_Model[i].position.z)
			minZ = m_Model[i].position.z;
	}

	m_ModelSize = D3DXVECTOR3(maxX - minX, maxY - minY, maxZ - minZ);
	m_ModelCenter = D3DXVECTOR3(minX + m_ModelSize.x / 2.0f, minY + m_ModelSize.y / 2.0f, minZ + m_ModelSize.z / 2.0f);


	return;
}

bool SphereModelClass::InitializeBuffers(ID3D10Device *device)
{
	HRESULT result;
	unsigned long *indices;
	D3D10_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D10_SUBRESOURCE_DATA vertexData, indexData;


	// Create the indices array
	indices = new unsigned long[m_IndexCount];
	if (!indices)
		return false;

	// Fill the indices array
	for (int i = 0; i < m_IndexCount; i++)
		indices[i] = i;

	// Set up the description of the vertex buffer.
	ZeroMemory(&vertexBufferDesc, sizeof(D3D10_BUFFER_DESC));
	vertexBufferDesc.Usage = D3D10_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(SphereModelType)* m_VertexCount;
	vertexBufferDesc.BindFlags = D3D10_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = m_Model;

	// Now finally create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_VertexBuffer);
	if (FAILED(result))
		return false;

	// Set up the description of the vertex buffer.
	ZeroMemory(&indexBufferDesc, sizeof(D3D10_BUFFER_DESC));
	indexBufferDesc.Usage = D3D10_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long)* m_IndexCount;
	indexBufferDesc.BindFlags = D3D10_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	// Give the subresource structure a pointer to the vertex data.
	indexData.pSysMem = indices;

	// Now finally create the vertex buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_IndexBuffer);
	if (FAILED(result))
		return false;

	delete[] indices;
	indices = 0;


	return true;
}

void SphereModelClass::Shutdown()
{
	if (m_Model)
	{
		delete[] m_Model;
		m_Model = 0;
	}

	return;
}

D3DXVECTOR3 SphereModelClass::GetModelSize()
{
	return m_ModelSize;
}

D3DXVECTOR3 SphereModelClass::GetModelCenter()
{
	return m_ModelCenter;
}

unsigned long SphereModelClass::GetVerticesCount()
{
	return m_VertexCount;
}

ID3D10Buffer* SphereModelClass::GetVertexBufferPtr()
{
	return m_VertexBuffer;
}

ID3D10Buffer* SphereModelClass::GetIndexBufferPtr()
{
	return m_IndexBuffer;
}
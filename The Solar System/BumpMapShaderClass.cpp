#include "BumpMapShaderClass.h"


BumpMapShaderClass::BumpMapShaderClass()
{
	m_Effect = 0;
	m_Technique = 0;
	m_Layout = 0;

	m_worldMatrixPtr = 0;
	m_viewMatrixPtr = 0;
	m_projectionMatrixPtr = 0;
	m_texturesArrayPtr = 0;

	m_lightDirectionPtr = 0;
	m_diffuseColorPtr = 0;
	m_ambientColorPtr = 0;
}

BumpMapShaderClass::BumpMapShaderClass(const BumpMapShaderClass& other)
{

}

BumpMapShaderClass::~BumpMapShaderClass()
{
}

bool BumpMapShaderClass::Initialize(ID3D10Device *device, HWND hwnd)
{
	if (!InitializeShader(device, hwnd, "Data/Shaders/BumpMapShader.fx"))
		return false;

	return true;
}

bool BumpMapShaderClass::InitializeShader(ID3D10Device *device, HWND hwnd, char *shaderFilename)
{
	HRESULT result;
	ID3D10Blob *errorMessage;
	D3D10_INPUT_ELEMENT_DESC polygonLayout[5];
	unsigned int numElements;
	D3D10_PASS_DESC passDesc;


	// Create the effect
	result = D3DX10CreateEffectFromFile(shaderFilename, NULL, NULL, "fx_4_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
		device, NULL, NULL, &m_Effect, &errorMessage, NULL);

	if (FAILED(result))
	{
		if (errorMessage)
		{
			OutputErrorMessage(errorMessage, hwnd, shaderFilename);
		}

		return false;
	}

	m_Technique = m_Effect->GetTechniqueByName("BumpMapTechnique");
	if (!m_Technique)
		return false;

	// Now setup the layout of the data that goes into the shader.
	// This setup needs to match the VertexType stucture in the ModelClass and in the shader.
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D10_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "TEXCOORD";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D10_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D10_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;
	
	polygonLayout[2].SemanticName = "NORMAL";
	polygonLayout[2].SemanticIndex = 0;
	polygonLayout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[2].InputSlot = 0;
	polygonLayout[2].AlignedByteOffset = D3D10_APPEND_ALIGNED_ELEMENT;
	polygonLayout[2].InputSlotClass = D3D10_INPUT_PER_VERTEX_DATA;
	polygonLayout[2].InstanceDataStepRate = 0;

	polygonLayout[3].SemanticName = "TANGENT";
	polygonLayout[3].SemanticIndex = 0;
	polygonLayout[3].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[3].InputSlot = 0;
	polygonLayout[3].AlignedByteOffset = D3D10_APPEND_ALIGNED_ELEMENT;
	polygonLayout[3].InputSlotClass = D3D10_INPUT_PER_VERTEX_DATA;
	polygonLayout[3].InstanceDataStepRate = 0;

	polygonLayout[4].SemanticName = "BINORMAL";
	polygonLayout[4].SemanticIndex = 0;
	polygonLayout[4].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[4].InputSlot = 0;
	polygonLayout[4].AlignedByteOffset = D3D10_APPEND_ALIGNED_ELEMENT;
	polygonLayout[4].InputSlotClass = D3D10_INPUT_PER_VERTEX_DATA;
	polygonLayout[4].InstanceDataStepRate = 0;

	// Get a count of the elements in the layout.
	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// Get the description of the first pass described in the shader technique.
	m_Technique->GetPassByIndex(0)->GetDesc(&passDesc);

	// Create the input layout.
	result = device->CreateInputLayout(polygonLayout, numElements, passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &m_Layout);
	if (FAILED(result))
		return false;

	// Get pointers to the variables inside the shader so we can update them from this class.
	m_worldMatrixPtr = m_Effect->GetVariableByName("worldMatrix")->AsMatrix();
	m_viewMatrixPtr = m_Effect->GetVariableByName("viewMatrix")->AsMatrix();
	m_projectionMatrixPtr = m_Effect->GetVariableByName("projectionMatrix")->AsMatrix();
	m_texturesArrayPtr = m_Effect->GetVariableByName("texturesArray")->AsShaderResource();
	m_lightDirectionPtr = m_Effect->GetVariableByName("lightDirection")->AsVector();
	m_diffuseColorPtr = m_Effect->GetVariableByName("diffuseColor")->AsVector();
	m_ambientColorPtr = m_Effect->GetVariableByName("ambientColor")->AsVector();

	return true;
}

void BumpMapShaderClass::OutputErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, char *shaderFilename)
{
	char* compileErrors;
	unsigned long bufferSize, i;
	ofstream fout;


	// Get a pointer to the error message text buffer.
	compileErrors = (char*)(errorMessage->GetBufferPointer());

	// Get the length of the message.
	bufferSize = errorMessage->GetBufferSize();

	// Open a file to write the error message to.
	fout.open("shader-error.txt");

	// Write out the error message.
	for (i = 0; i<bufferSize; i++)
	{
		fout << compileErrors[i];
	}

	// Close the file.
	fout.close();

	// Release the error message.
	errorMessage->Release();
	errorMessage = 0;

	// Pop a message up on the screen to notify the user to check the text file for compile errors.
	MessageBox(hwnd, "Error compiling shader.  Check shader-error.txt for message.", shaderFilename, MB_OK);

	return;
}

void BumpMapShaderClass::Render(ID3D10Device* device, unsigned long indexCount, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, ID3D10ShaderResourceView** texturesArray,
	D3DXVECTOR3 lightDirection, D3DXCOLOR diffuseColor, D3DXCOLOR ambientColor)
{
	SetShaderParameters(worldMatrix, viewMatrix, projectionMatrix, texturesArray, lightDirection, diffuseColor, ambientColor);

	RenderShader(device, indexCount);


	return;
}

void BumpMapShaderClass::SetShaderParameters(D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, ID3D10ShaderResourceView** texturesArray,
	D3DXVECTOR3 lightDirection, D3DXCOLOR diffuseColor, D3DXCOLOR ambientColor)
{
	m_worldMatrixPtr->SetMatrix((float*)&worldMatrix);
	m_viewMatrixPtr->SetMatrix((float*)&viewMatrix);
	m_projectionMatrixPtr->SetMatrix((float*)&projectionMatrix);
	m_texturesArrayPtr->SetResourceArray(texturesArray, 0, 2);
	m_lightDirectionPtr->SetFloatVector((float*)&lightDirection);
	m_diffuseColorPtr->SetFloatVector((float*)&diffuseColor);
	m_ambientColorPtr->SetFloatVector((float*)&ambientColor);

	return;
}

void BumpMapShaderClass::RenderShader(ID3D10Device* device, unsigned long indexCount)
{
	D3D10_TECHNIQUE_DESC techniqueDesc;
	unsigned int i;


	// Set the input layout.
	device->IASetInputLayout(m_Layout);

	// Get the description structure of the technique from inside the shader so it can be used for rendering.
	m_Technique->GetDesc(&techniqueDesc);

	// Go through each pass in the technique (should be just one currently) and render the triangles.
	for (i = 0; i<techniqueDesc.Passes; ++i)
	{
		m_Technique->GetPassByIndex(i)->Apply(0);
		device->DrawIndexed(indexCount, 0, 0);
	}

	return;
}

void BumpMapShaderClass::Shutdown()
{
	ShutdownShader();

	return;
}

void BumpMapShaderClass::ShutdownShader()
{
	if (m_Effect)
	{
		m_Effect->Release(); 
		m_Effect = 0;
	}

	m_Technique = 0;

	if (m_Layout)
	{
		m_Layout->Release();
		m_Layout = 0;
	}

	m_worldMatrixPtr = 0;
	m_viewMatrixPtr = 0;
	m_projectionMatrixPtr = 0;
	m_texturesArrayPtr = 0;
	m_lightDirectionPtr = 0;
	m_diffuseColorPtr = 0;

	return;
}

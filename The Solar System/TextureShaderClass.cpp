#include "TextureShaderClass.h"


TextureShaderClass::TextureShaderClass()
{
	m_Effect = 0;
	m_Technique = 0;
	m_Layout = 0;

	m_worldMatrixPtr = 0;
	m_viewMatrixPtr = 0;
	m_projectionMatrixPtr = 0;
	m_texturePtr = 0;
}

TextureShaderClass::TextureShaderClass(const TextureShaderClass& other)
{

}

TextureShaderClass::~TextureShaderClass()
{
}

bool TextureShaderClass::Initialize(ID3D10Device *device, HWND hwnd)
{
	if (!InitializeShader(device, hwnd, "Data/Shaders/TextureShader.fx"))
		return false;

	return true;
}

bool TextureShaderClass::InitializeShader(ID3D10Device *device, HWND hwnd, char *shaderFilename)
{
	HRESULT result;
	ID3D10Blob *errorMessage;
	D3D10_INPUT_ELEMENT_DESC polygonLayout[2];
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

	m_Technique = m_Effect->GetTechniqueByName("TextureTechnique");
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
	m_texturePtr = m_Effect->GetVariableByName("shaderTexture")->AsShaderResource();


	return true;
}

void TextureShaderClass::OutputErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, char *shaderFilename)
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

void TextureShaderClass::Render(ID3D10Device* device, unsigned long indexCount, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, ID3D10ShaderResourceView* texture)
{
	SetShaderParameters(worldMatrix, viewMatrix, projectionMatrix, texture);

	RenderShader(device, indexCount);


	return;
}

void TextureShaderClass::SetShaderParameters(D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, ID3D10ShaderResourceView* texture)
{
	m_worldMatrixPtr->SetMatrix((float*)&worldMatrix);
	m_viewMatrixPtr->SetMatrix((float*)&viewMatrix);
	m_projectionMatrixPtr->SetMatrix((float*)&projectionMatrix);
	m_texturePtr->SetResource(texture);

	return;
}

void TextureShaderClass::RenderShader(ID3D10Device* device, unsigned long indexCount)
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

void TextureShaderClass::Shutdown()
{
	ShutdownShader();

	return;
}

void TextureShaderClass::ShutdownShader()
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
	m_texturePtr = 0;

	return;
}

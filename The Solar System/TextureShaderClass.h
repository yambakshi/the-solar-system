#ifndef TEXTURESHADERCLASS_H
#define TEXTURESHADERCLASS_H

// INCLUDES // 
#include <fstream>
#include <D3D10.h>
#include <D3DX10.h>

using namespace std;

class TextureShaderClass
{
public:
	TextureShaderClass();
	TextureShaderClass(const TextureShaderClass&);
	~TextureShaderClass();

	bool Initialize(ID3D10Device *device, HWND hwnd);
	void Render(ID3D10Device* device, unsigned long indexCount,
		D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix,
		ID3D10ShaderResourceView* texture);
	void Shutdown();

private:
	// Methods
	bool InitializeShader(ID3D10Device*, HWND, char*);
	void OutputErrorMessage(ID3D10Blob*, HWND, char*);
	void SetShaderParameters(D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D10ShaderResourceView*);
	void RenderShader(ID3D10Device*, unsigned long);
	void ShutdownShader();
	
	// Members
	ID3D10Effect *m_Effect;
	ID3D10EffectTechnique *m_Technique;
	ID3D10InputLayout *m_Layout;
	ID3D10EffectMatrixVariable 
		*m_worldMatrixPtr,
		*m_viewMatrixPtr,
		*m_projectionMatrixPtr;
	ID3D10EffectShaderResourceVariable *m_texturePtr;


};

#endif
#ifndef DIRECTXCLASS_H
#define DIRECTXCLASS_H

// INCLUDES //
#include <d3d10.h>
#include <d3dx10.h>

// LINKING //
#pragma comment(lib, "d3d10.lib")
#pragma comment(lib, "d3dx10.lib")
#pragma comment(lib, "dxgi.lib")

class DirectXClass
{
public:
	DirectXClass();
	DirectXClass(const DirectXClass&);
	~DirectXClass();

	bool Initialize(HWND hwnd, int windowWidth, int windowHeight, bool vsync, bool fullscreen, float screenDepth, float screenNear);
	void Shutdown();

	void BeginScene(float, float, float, float);
	void EndScene();

	void TurnZBufferOn();
	void TurnZBufferOff();

	bool ResizeBackBuffer(unsigned int width, unsigned int height, bool fullScreen, float screenDepth, float screenNear);

	ID3D10Device* GetDevice();

	void GetProjectionMatrix(D3DXMATRIX&);
	void GetWorldMatrix(D3DXMATRIX&);
	void GetOrthoMatrix(D3DXMATRIX&);

	void GetVideoCardInfo(char*, int&);

private:
	bool m_vsync_enabled;
	int m_videoCardMemory;
	char m_videoCardDescription[128];
	IDXGISwapChain* m_swapChain;
	ID3D10Device* m_device;
	ID3D10RenderTargetView* m_renderTargetView;
	ID3D10Texture2D* m_depthStencilBuffer;
	ID3D10DepthStencilState* m_depthStencilState;
	ID3D10DepthStencilView* m_depthStencilView;
	ID3D10DepthStencilState* m_depthDisabledStencilState;
	ID3D10RasterizerState* m_rasterState;
	D3DXMATRIX m_projectionMatrix;
	D3DXMATRIX m_worldMatrix;
	D3DXMATRIX m_orthoMatrix;
};

#endif


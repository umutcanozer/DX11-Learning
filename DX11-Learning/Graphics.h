#pragma once
#include <d3d11.h>

class Graphics
{
public:
	Graphics(HWND hwnd);
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	~Graphics();

	void EndFrame();
	void ClearBuffer(float r, float g, float b) {
		float color[4] = {r, g, b, 1.f};
		m_deviceContext->ClearRenderTargetView(m_renderTargetView, color);
	}
private:
	HRESULT hr;

	IDXGISwapChain* m_swapChain = nullptr;
	ID3D11Device* m_device = nullptr;
	ID3D11DeviceContext* m_deviceContext = nullptr;

	ID3D11RenderTargetView* m_renderTargetView;
};


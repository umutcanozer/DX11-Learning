#pragma once
#include <d3d11.h>
#include <wrl.h>

class Graphics
{
public:
	Graphics(HWND hwnd);
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	~Graphics() = default;

	void EndFrame();
	void ClearBuffer(float r, float g, float b);

	void DrawingTriangle(float angle);
private:
	HRESULT hr;

	Microsoft::WRL::ComPtr<IDXGISwapChain> m_swapChain;
	Microsoft::WRL::ComPtr<ID3D11Device> m_device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_deviceContext;

	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_renderTargetView;
};


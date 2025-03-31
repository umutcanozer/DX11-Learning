#include "Graphics.h"
#include <iostream>

Graphics::Graphics(HWND hwnd)
{
	D3D_FEATURE_LEVEL feature_level = D3D_FEATURE_LEVEL_11_0;
	DXGI_SWAP_CHAIN_DESC scd = {};
	scd.BufferDesc.Width = 0;
	scd.BufferDesc.Height = 0;
	scd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	scd.BufferDesc.RefreshRate.Numerator = 0;
	scd.BufferDesc.RefreshRate.Denominator = 0;
	scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	scd.SampleDesc.Count = 1;
	scd.SampleDesc.Quality = 0;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.BufferCount = 1;
	scd.OutputWindow = hwnd;
	scd.Windowed = TRUE;
	scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	scd.Flags = 0;

	hr = D3D11CreateDeviceAndSwapChain(
		nullptr, //default
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0,
		&feature_level,
		1,
		D3D11_SDK_VERSION,
		&scd,
		&m_swapChain,
		&m_device,
		nullptr,
		&m_deviceContext
	);

	if (FAILED(hr)) {
		// Hata kodunu yazdýr veya logla
		std::cerr << "D3D11CreateDeviceAndSwapChain failed with error: " << hr << std::endl;
		return;
	}

	ID3D11Resource* p_backBuffer = nullptr;
	//0 = backbuffer
	m_swapChain->GetBuffer(0, __uuidof(ID3D11Resource), reinterpret_cast<void**>(&p_backBuffer));

	hr = m_device->CreateRenderTargetView(p_backBuffer, nullptr, &m_renderTargetView);
	if (FAILED(hr))
	{
		std::cerr << "CreateRenderTargetView failed with error: " << hr << std::endl;
		return;
	}
	//it was needed temporary to create view
	p_backBuffer->Release();
}

void Graphics::EndFrame()
{
	//Present the back buffer to the screen since rendering is complete.
	//back buffer -> front buffer
	m_swapChain->Present(1u, 0u);
}


Graphics::~Graphics()
{
	if (m_deviceContext)
	{
		m_deviceContext->Release();
		m_deviceContext = nullptr;
	}

	if (m_device)
	{
		m_device->Release();
		m_device = nullptr;
	}

	if (m_swapChain)
	{
		m_swapChain->Release();
		m_swapChain = nullptr;
	}

	if (m_renderTargetView) {
		m_renderTargetView->Release();
		m_renderTargetView = nullptr;
	}
}


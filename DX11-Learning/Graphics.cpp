#include "Graphics.h"
#include <iostream>
#include <d3dcompiler.h>
#include <DirectXMath.h>

namespace dx = DirectX;

Graphics::Graphics(HWND hWnd)
{
	D3D_FEATURE_LEVEL feature_level = D3D_FEATURE_LEVEL_11_1;
	DXGI_SWAP_CHAIN_DESC scd = {};

	ZeroMemory(&scd, sizeof(scd));
	scd.BufferDesc.Width = 800;  
	scd.BufferDesc.Height = 600;
	scd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	scd.BufferDesc.RefreshRate.Numerator = 0;
	scd.BufferDesc.RefreshRate.Denominator = 0;
	scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	scd.SampleDesc.Count = 1;
	scd.SampleDesc.Quality = 0;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.BufferCount = 2;
	scd.OutputWindow = hWnd;
	scd.Windowed = TRUE;
	scd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
	scd.Flags = 0;

	UINT deviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#if defined(DEBUG) || defined(_DEBUG)
	deviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	hr = D3D11CreateDeviceAndSwapChain(
		nullptr, //default
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		deviceFlags,
		&feature_level,
		1,
		D3D11_SDK_VERSION,
		&scd,
		&m_pSwapChain,
		&m_pDevice,
		nullptr,
		&m_pDeviceContext
	);

	if (FAILED(hr)) {
		// Hata kodunu yazdýr veya logla
		std::cerr << "D3D11CreateDeviceAndSwapChain failed with error: " << hr << std::endl;
		return;
	}

	Microsoft::WRL::ComPtr<ID3D11Texture2D> p_backBuffer;
	//0 = backbuffer
	m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), &p_backBuffer);
	hr = m_pDevice->CreateRenderTargetView(p_backBuffer.Get(), nullptr, &m_pRenderTargetView);
	if (FAILED(hr))
	{
		std::cerr << "CreateRenderTargetView failed with error: " << hr << std::endl;
		return;
	}
	D3D11_TEXTURE2D_DESC backBufferDesc = {};
	p_backBuffer->GetDesc(&backBufferDesc);

	//depth stencil
	D3D11_DEPTH_STENCIL_DESC dsDesc = {};
	dsDesc.DepthEnable = TRUE;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;

	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> p_depthSState;
	hr = m_pDevice->CreateDepthStencilState(&dsDesc, &p_depthSState);
	if (FAILED(hr))
	{
		std::cerr << "CreateDepthStencilState failed with error: " << hr << std::endl;
		return;
	}
	m_pDeviceContext->OMSetDepthStencilState(p_depthSState.Get(), 1u);

	Microsoft::WRL::ComPtr<ID3D11Texture2D> p_depthStencil;
	D3D11_TEXTURE2D_DESC dDesc = {};
	//must match with the swap chain values
	dDesc.Width = backBufferDesc.Width;
	dDesc.Height = backBufferDesc.Height;
	dDesc.MipLevels = 1u;
	dDesc.ArraySize = 1u;
	dDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dDesc.SampleDesc.Count = 1u;
	dDesc.SampleDesc.Quality = 0u;
	dDesc.Usage = D3D11_USAGE_DEFAULT;
	dDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	hr = m_pDevice->CreateTexture2D(&dDesc, nullptr, &p_depthStencil);
	if (FAILED(hr))
	{
		std::cerr << "CreateTexture2D failed with error: " << hr << std::endl;
		return;
	}

	D3D11_DEPTH_STENCIL_VIEW_DESC dsViewDesc = {};
	dsViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsViewDesc.Texture2D.MipSlice = 0u;

	hr = m_pDevice->CreateDepthStencilView(p_depthStencil.Get(), &dsViewDesc, &m_pDepthStencilView);
	if (FAILED(hr))
	{
		std::cerr << "CreateDepthStencilView failed with error: " << hr << std::endl;
		return;
	}

	m_pDeviceContext->OMSetRenderTargets(1u, m_pRenderTargetView.GetAddressOf(), m_pDepthStencilView.Get());
	
	D3D11_VIEWPORT vp = {};
	ZeroMemory(&vp, sizeof(D3D11_VIEWPORT));
	vp.Width = 800;
	vp.Height = 600;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;

	// Create the viewport.
	m_pDeviceContext->RSSetViewports(1, &vp);
}

void Graphics::Present()
{
	//Present the back buffer to the screen since rendering is complete.
	//back buffer -> front buffer
	if (FAILED(hr = m_pSwapChain->Present(1u, 0u))) {
		if (hr == DXGI_ERROR_DEVICE_REMOVED)
		{
			throw m_pDevice->GetDeviceRemovedReason();
		}
		else
		{
			throw "Present failed with error: " + hr;
		}
	}
}

void Graphics::ClearBuffer() 
{
	m_pDeviceContext->OMSetRenderTargets(1u, m_pRenderTargetView.GetAddressOf(), m_pDepthStencilView.Get());

	const float teal[] = { 0.098f, 0.439f, 0.439f, 1.000f };
	m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView.Get(), teal);
	m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.f, 0u);
}

void Graphics::DrawIndexed(UINT count)
{
	m_pDeviceContext->DrawIndexed(count, 0u, 0u);
}

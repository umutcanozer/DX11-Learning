#include "Graphics.h"
#include <iostream>
#include <d3dcompiler.h>

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

	Microsoft::WRL::ComPtr<ID3D11Resource> p_backBuffer;
	//0 = backbuffer
	m_swapChain->GetBuffer(0, __uuidof(ID3D11Resource), &p_backBuffer);

	hr = m_device->CreateRenderTargetView(p_backBuffer.Get(), nullptr, &m_renderTargetView);
	if (FAILED(hr))
	{
		std::cerr << "CreateRenderTargetView failed with error: " << hr << std::endl;
		return;
	}
}

void Graphics::EndFrame()
{
	//Present the back buffer to the screen since rendering is complete.
	//back buffer -> front buffer
	m_swapChain->Present(1u, 0u);
}

void Graphics::ClearBuffer(float red, float green, float blue) 
{
	const float color[] = { red,green,blue,1.0f };
	m_deviceContext->ClearRenderTargetView(m_renderTargetView.Get(), color);
}

void Graphics::DrawingTriangle()
{
	struct Vertex {
		float x;
		float y;
	};
	const Vertex vertices[] = {
		{0.f, 0.75f},
		{0.75f, -0.75f},
		{-0.75f, -0.75f}
	};	

	Microsoft::WRL::ComPtr<ID3D11Buffer> p_vertexBuffer;

	D3D11_BUFFER_DESC vbDesc, pbDesc;
	D3D11_SUBRESOURCE_DATA vData, pData;
	Microsoft::WRL::ComPtr<ID3DBlob> p_blob;
	//Vertex
	vbDesc.Usage = D3D11_USAGE_DEFAULT;
	vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbDesc.CPUAccessFlags = 0;
	vbDesc.MiscFlags = 0;
	vbDesc.ByteWidth = sizeof(vertices);
	vbDesc.StructureByteStride = sizeof(Vertex);

	vData.pSysMem = vertices;

	hr = m_device->CreateBuffer(&vbDesc, &vData, &p_vertexBuffer);
	if (FAILED(hr)) {
		std::cerr << "CreateBuffer failed with error: " << hr << std::endl;
		return;
	}

	unsigned int stride;
	unsigned int offset;
	stride = sizeof(Vertex);
	offset = 0u;
	m_deviceContext->IASetVertexBuffers(0u, 1u, p_vertexBuffer.GetAddressOf(), &stride, &offset);

	Microsoft::WRL::ComPtr<ID3D11PixelShader> p_pixelShader;
	hr = D3DReadFileToBlob(L"PixelShader.cso", &p_blob);
	if (FAILED(hr)) {
		std::cerr << "D3DReadFileToBlob failed with error: " << hr << std::endl;
		return;
	}

	hr = m_device->CreatePixelShader(p_blob->GetBufferPointer(), p_blob->GetBufferSize(), nullptr, &p_pixelShader);
	if (FAILED(hr)) {
		std::cerr << "CreatePixelShader failed with error: " << hr << std::endl;
		return;
	}

	Microsoft::WRL::ComPtr<ID3D11VertexShader> p_vertexShader;
	hr = D3DReadFileToBlob(L"VertexShader.cso", &p_blob);
	if (FAILED(hr)) {
		std::cerr << "D3DReadFileToBlob failed with error: " << hr << std::endl;
		return;
	}

	hr = m_device->CreateVertexShader(p_blob->GetBufferPointer(), p_blob->GetBufferSize(), nullptr, &p_vertexShader);
	if (FAILED(hr)) {
		std::cerr << "CreateVertexShader failed with error: " << hr << std::endl;
		return;
	}

	//cant draw without shader
	m_deviceContext->VSSetShader(p_vertexShader.Get(), nullptr, 0);
	m_deviceContext->PSSetShader(p_pixelShader.Get(), nullptr, 0);

	//input vertex layout
	Microsoft::WRL::ComPtr<ID3D11InputLayout> p_inputLayout;
	const D3D11_INPUT_ELEMENT_DESC ied[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	hr = m_device->CreateInputLayout(
		ied, (UINT)std::size(ied),
		p_blob->GetBufferPointer(),
		p_blob->GetBufferSize(),
		&p_inputLayout
	);

	m_deviceContext->IASetInputLayout(p_inputLayout.Get());

	//bind render target
	//we dont use & here because we dont wanna free it
	m_deviceContext->OMSetRenderTargets(1u, m_renderTargetView.GetAddressOf(), nullptr);

	//conf viewport
	D3D11_VIEWPORT vp;
	vp.Width = 800;
	vp.Height = 600;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;

	// Create the viewport.
	m_deviceContext->RSSetViewports(1, &vp);
	m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_deviceContext->Draw((UINT)std::size(vertices), 0u);
}


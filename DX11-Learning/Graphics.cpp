#include "Graphics.h"
#include <iostream>
#include <d3dcompiler.h>
#include <DirectXMath.h>

namespace dx = DirectX;

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

	//depth stencil
	D3D11_DEPTH_STENCIL_DESC dsDesc = {};
	dsDesc.DepthEnable = TRUE;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;

	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> p_depthSState;
	hr = m_device->CreateDepthStencilState(&dsDesc, &p_depthSState);
	if (FAILED(hr))
	{
		std::cerr << "CreateDepthStencilState failed with error: " << hr << std::endl;
		return;
	}

	m_deviceContext->OMSetDepthStencilState(p_depthSState.Get(), 1u);

	Microsoft::WRL::ComPtr<ID3D11Texture2D> p_depthStencil;
	D3D11_TEXTURE2D_DESC dDesc = {};
	//must match with the swap chain values
	dDesc.Width = 800u;
	dDesc.Height = 600u;
	dDesc.MipLevels = 1u;
	dDesc.ArraySize = 1u;
	dDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dDesc.SampleDesc.Count = 1u;
	dDesc.SampleDesc.Quality = 0u;
	dDesc.Usage = D3D11_USAGE_DEFAULT;
	dDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	hr = m_device->CreateTexture2D(&dDesc, nullptr, &p_depthStencil);
	if (FAILED(hr))
	{
		std::cerr << "CreateTexture2D failed with error: " << hr << std::endl;
		return;
	}

	D3D11_DEPTH_STENCIL_VIEW_DESC dsViewDesc = {};
	dsViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsViewDesc.Texture2D.MipSlice = 0u;

	hr = m_device->CreateDepthStencilView(p_depthStencil.Get(), &dsViewDesc, &m_depthStencilView);
	if (FAILED(hr))
	{
		std::cerr << "CreateDepthStencilView failed with error: " << hr << std::endl;
		return;
	}

	m_deviceContext->OMSetRenderTargets(1u, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get());
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
	m_deviceContext->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.f, 0u);
}

void Graphics::DrawingTriangle(float angle, float x, float z)
{
	//when creating a struct in C++ the data is stored in system memory, but we need it to be in video memory
	struct Vertex {
		struct position {
			float x;
			float y;
			float z;
		}pos;
	};
	const Vertex vertices[] = 
	{
		{-1.f,-1.f,-1.f},
		{1.f,-1.f,-1.f},
		{-1.f,1.f,-1.f},
		{1.f,1.f,-1.f},
		{-1.f,-1.f,1.f},
		{1.f,-1.f,1.f},
		{-1.f,1.f,1.f},
		{1.f,1.f,1.f},
	};	

	Microsoft::WRL::ComPtr<ID3D11Buffer> p_vertexBuffer;

	D3D11_BUFFER_DESC vbDesc, pbDesc = {};
	D3D11_SUBRESOURCE_DATA vData, pData = {};
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


	const unsigned short indices[] =
	{
		0,2,1, 2,3,1,
		1,3,5, 3,7,5,
		2,6,3, 3,6,7,
		4,5,7, 4,7,6,
		0,4,2, 2,4,6,
		0,1,4, 1,5,4
	};
	
	Microsoft::WRL::ComPtr<ID3D11Buffer> p_indexBuffer;
	D3D11_BUFFER_DESC ibDesc = {};
	ibDesc.Usage = D3D11_USAGE_DEFAULT;
	ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibDesc.CPUAccessFlags = 0;
	ibDesc.MiscFlags = 0;
	ibDesc.ByteWidth = sizeof(indices);
	ibDesc.StructureByteStride = sizeof(unsigned short);

	D3D11_SUBRESOURCE_DATA iData = {};
	iData.pSysMem = indices;

	hr= m_device->CreateBuffer(&ibDesc, &iData, &p_indexBuffer);
	if (FAILED(hr))
	{
		std::cerr << "CreateBuffer failed with error: " << hr << std::endl;
		return;
	}

	//bind index buffer
	m_deviceContext->IASetIndexBuffer(p_indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);

	//constant buffer
	struct ConstantBuffer {
		dx::XMMATRIX transform;
	};

	const ConstantBuffer cb = {
		{
			dx::XMMatrixTranspose(
				dx::XMMatrixRotationZ(angle) *
				dx::XMMatrixRotationX(angle) *
				dx::XMMatrixTranslation(x,0.f,z + 4.f) *
				dx::XMMatrixPerspectiveLH(1.0f, 3.f / 4.f, 0.5f, 10.f)
			)
		}
	};

	Microsoft::WRL::ComPtr<ID3D11Buffer> p_constantBuffer;
	D3D11_BUFFER_DESC cbDesc = {};
	D3D11_SUBRESOURCE_DATA cData = {};
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.Usage = D3D11_USAGE_DYNAMIC; //for update it every frame
	cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbDesc.MiscFlags = 0u;
	cbDesc.ByteWidth = sizeof(cb);
	cbDesc.StructureByteStride = sizeof(ConstantBuffer);

	cData.pSysMem = &cb;

	hr = m_device->CreateBuffer(&cbDesc, &cData, &p_constantBuffer);
	if (FAILED(hr)) {
		std::cerr << "CreateBuffer failed with error: " << hr << std::endl;
		return;
	}

	m_deviceContext->VSSetConstantBuffers(0u, 1u, p_constantBuffer.GetAddressOf());

	struct ConstantBuffer2 {
		struct {
			float r;
			float g;
			float b;
			float a;
		}face_colors[6];
	};

	const ConstantBuffer2 cb2 = {
		{
			{1.f, 0.f, 1.f},
			{1.f, 0.f, 0.f},
			{0.f, 1.f, 0.f},
			{0.f, 0.f, 1.f},
			{1.f, 1.f, 0.f},
			{0.f, 1.f, 1.f}
		}
	};

	Microsoft::WRL::ComPtr<ID3D11Buffer> p_constantBuffer2;
	D3D11_BUFFER_DESC cbDesc2 = {};
	D3D11_SUBRESOURCE_DATA cData2 = {};
	cbDesc2.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc2.Usage = D3D11_USAGE_DEFAULT; 
	cbDesc2.CPUAccessFlags = 0u;
	cbDesc2.MiscFlags = 0u;
	cbDesc2.ByteWidth = sizeof(cb2);
	cbDesc2.StructureByteStride = sizeof(ConstantBuffer2);

	cData2.pSysMem = &cb2;

	hr = m_device->CreateBuffer(&cbDesc2, &cData2, &p_constantBuffer2);
	if (FAILED(hr)) {
		std::cerr << "CreateBuffer failed with error: " << hr << std::endl;
		return;
	}

	m_deviceContext->PSSetConstantBuffers(0u, 1u, p_constantBuffer2.GetAddressOf());

	//pixel shader
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
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	hr = m_device->CreateInputLayout(
		ied, (UINT)std::size(ied),
		p_blob->GetBufferPointer(),
		p_blob->GetBufferSize(),
		&p_inputLayout
	);

	m_deviceContext->IASetInputLayout(p_inputLayout.Get());

	//conf viewport
	D3D11_VIEWPORT vp = {};
	vp.Width = 800;
	vp.Height = 600;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;

	// Create the viewport.
	m_deviceContext->RSSetViewports(1, &vp);
	m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_deviceContext->DrawIndexed((UINT)std::size(indices), 0u, 0u);
}


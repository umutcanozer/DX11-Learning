#include "ApplicationClass.h"
#include "MeshComponent.h"
#include "ShaderComponent.h"
#include "ConstantBuffer.h"
#include "Topology.h"
#include <memory>
using namespace DirectX;
ApplicationClass::ApplicationClass(int screenWidth, int screenHeight)
{
	m_System = new SystemClass(screenWidth, screenHeight, "Engine");
	m_lastTime = std::chrono::high_resolution_clock::now();

	struct Vertex {
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT4 color;
	};
	const std::vector<Vertex> vertices =
	{
		// Front (Z+)
		{ XMFLOAT3(-1.0f,  1.0f, -1.0f),  XMFLOAT4(1,0,0,1) },
		{ XMFLOAT3(1.0f,  1.0f, -1.0f),  XMFLOAT4(0,1,0,1) }, 
		{ XMFLOAT3(1.0f, -1.0f, -1.0f),  XMFLOAT4(0,0,1,1) }, 
		{ XMFLOAT3(-1.0f, -1.0f, -1.0f),  XMFLOAT4(1,1,1,1) },
		// Back (Z-)
		{ XMFLOAT3(-1.0f,  1.0f, 1.0f),   XMFLOAT4(1,0,0,1) },
		{ XMFLOAT3(1.0f,  1.0f, 1.0f),   XMFLOAT4(0,1,0,1) },
		{ XMFLOAT3(1.0f, -1.0f, 1.0f),   XMFLOAT4(0,0,1,1) },
		{ XMFLOAT3(-1.0f, -1.0f, 1.0f),   XMFLOAT4(1,1,1,1) },
		// Left (X-)
		{ XMFLOAT3(-1.0f,  1.0f,  1.0f),  XMFLOAT4(1,0,0,1) },
		{ XMFLOAT3(-1.0f,  1.0f, -1.0f),  XMFLOAT4(0,1,0,1) },
		{ XMFLOAT3(-1.0f, -1.0f, -1.0f),  XMFLOAT4(0,0,1,1) },
		{ XMFLOAT3(-1.0f, -1.0f,  1.0f),  XMFLOAT4(1,1,1,1) },
		// Right (X+)
		{ XMFLOAT3(1.0f,  1.0f,  1.0f),  XMFLOAT4(1,0,0,1) },
		{ XMFLOAT3(1.0f,  1.0f, -1.0f),  XMFLOAT4(0,1,0,1) },
		{ XMFLOAT3(1.0f, -1.0f, -1.0f),  XMFLOAT4(0,0,1,1) },
		{ XMFLOAT3(1.0f, -1.0f,  1.0f),  XMFLOAT4(1,1,1,1) },
		// Top (Y+)
		{ XMFLOAT3(-1.0f, 1.0f,  1.0f),   XMFLOAT4(1,0,0,1) },
		{ XMFLOAT3(1.0f, 1.0f,  1.0f),   XMFLOAT4(0,1,0,1) },
		{ XMFLOAT3(1.0f, 1.0f, -1.0f),   XMFLOAT4(0,0,1,1) },
		{ XMFLOAT3(-1.0f, 1.0f, -1.0f),   XMFLOAT4(1,1,1,1) },
		// Bottom (Y-)
		{ XMFLOAT3(-1.0f, -1.0f,  1.0f),  XMFLOAT4(1,0,0,1) },
		{ XMFLOAT3(1.0f, -1.0f,  1.0f),  XMFLOAT4(0,1,0,1) },
		{ XMFLOAT3(1.0f, -1.0f, -1.0f),  XMFLOAT4(0,0,1,1) },
		{ XMFLOAT3(-1.0f, -1.0f, -1.0f),  XMFLOAT4(1,1,1,1) }
	};
	const std::vector<unsigned short> indices =
	{
		0, 1, 2,  0, 2, 3,
		4, 6, 5,  4, 7, 6,
		8, 9, 10,  8, 10, 11,
		12, 14, 13,  12, 15, 14,
		16, 17, 18,  16, 18, 19,
		20, 22, 21,  20, 23, 22
	};

	D3D11_BUFFER_DESC cbd = {};
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.Usage = D3D11_USAGE_DYNAMIC;
	cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbd.MiscFlags = 0;
	cbd.ByteWidth = sizeof(MatrixBufferType);
	cbd.StructureByteStride = 0;

	m_System->GFX().GetDevice()->CreateBuffer(&cbd, nullptr, &m_pConstantBuffer);

	std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
	{
		{ "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "COLOR",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 }
	};
	ShaderData shaderData = { m_System->GFX(), L"VertexShader.cso", L"PixelShader.cso", ied };

	viewMat = XMMatrixLookAtLH(
		XMVectorSet(0.0f, 0.0f, -5.0f, 1.0f),  // Kamera konumu (0,0,-5)
		XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f),  // Hedef nokta (küpün merkezi 0,0,0)
		XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)   // Yukarý vektörü (0,1,0)
	);
	projMat = XMMatrixPerspectiveFovLH(XMConvertToRadians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

	std::shared_ptr<Component> cubeSharedMesh = std::make_shared<MeshComponent>(m_System->GFX(), vertices, indices);
	std::shared_ptr<Component> cubeSharedShader = std::make_shared<ShaderComponent>(shaderData);
	std::shared_ptr<Component> cubeSharedTopology = std::make_shared<Topology>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	


	cube.AddComponent(cubeSharedMesh);
	cube.AddComponent(cubeSharedShader);
	cube.AddComponent(cubeSharedTopology);

	
	cube.Bind(m_System->GFX());
}

void ApplicationClass::Frame()
{
	MatrixBufferType* cbDataPtr;
	rotationAngle += GetDeltaTime();
	m_System->GFX().ClearBuffer();

	XMMATRIX worldMat = XMMatrixRotationY(rotationAngle) * XMMatrixRotationZ(rotationAngle);
	D3D11_MAPPED_SUBRESOURCE mapped;
	hr = m_System->GFX().GetDeviceContext()->Map(m_pConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
	if (FAILED(hr))
	{
		std::cerr << "Map failed with error: " << hr << std::endl;
		return;
	}
	cbDataPtr = (MatrixBufferType*)mapped.pData;
	XMStoreFloat4x4(&cbDataPtr->world, XMMatrixTranspose(worldMat));
	XMStoreFloat4x4(&cbDataPtr->view, XMMatrixTranspose(viewMat));
	XMStoreFloat4x4(&cbDataPtr->proj, XMMatrixTranspose(projMat));
	m_System->GFX().GetDeviceContext()->Unmap(m_pConstantBuffer.Get(), 0);
	ID3D11Buffer* constBuffers[] = { m_pConstantBuffer.Get() };
	m_System->GFX().GetDeviceContext()->VSSetConstantBuffers(0, 1, constBuffers);

	cube.GetComponent<MeshComponent>()->Draw(m_System->GFX());

	m_System->GFX().Present();
}

float ApplicationClass::GetDeltaTime()
{
	auto currentTime = std::chrono::high_resolution_clock::now();

	std::chrono::duration<float> delta = currentTime - m_lastTime;

	m_lastTime = currentTime;

	return delta.count();
}


int ApplicationClass::Go()
{
	while (true)
	{
		// process all messages pending, but to not block for new messages
		if (const auto ecode = m_System->ProcessMessages())
		{
			// if return optional has value, means we're quitting so return exit code
			return *ecode;
		}

		while (const auto e = m_System->kbd.ReadKey()) {
			if (!e->IsPress())
			{
				continue;
			}

			if (e->GetCode() == VK_ESCAPE) {
				PostQuitMessage(0);
			}
		}
		Frame();
	}
}

ApplicationClass::~ApplicationClass()
{
	if (m_System) {
		delete m_System;
		m_System = nullptr;
	}
}


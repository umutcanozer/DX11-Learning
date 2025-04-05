#include "ApplicationClass.h"
#include "MeshComponent.h"
#include "ShaderComponent.h"
#include "ConstantBuffer.h"
#include "Topology.h"
#include <memory>

ApplicationClass::ApplicationClass(int screenWidth, int screenHeight)
{
	m_System = new SystemClass(screenWidth, screenHeight, "Engine");
	m_lastTime = std::chrono::high_resolution_clock::now();

	struct Vertex {
		struct position {
			float x;
			float y;
			float z;
		}pos;
	};
	const std::vector<Vertex> vertices =
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
	const std::vector<unsigned short> indices =
	{
		0,2,1, 2,3,1,
		1,3,5, 3,7,5,
		2,6,3, 3,6,7,
		4,5,7, 4,7,6,
		0,4,2, 2,4,6,
		0,1,4, 1,5,4
	};

	struct ConstantBuffer
	{
		struct
		{
			float r;
			float g;
			float b;
			float a;
		} face_colors[6];
	};
	const ConstantBuffer cb2 =
	{
		{
			{ 1.0f,0.0f,1.0f },
			{ 1.0f,0.0f,0.0f },
			{ 0.0f,1.0f,0.0f },
			{ 0.0f,0.0f,1.0f },
			{ 1.0f,1.0f,0.0f },
			{ 0.0f,1.0f,1.0f },
		}
	};

	std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
	{
		{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
	};
	ShaderData shaderData = { m_System->GFX(), L"VertexShader.cso", L"PixelShader.cso", ied };

	std::shared_ptr<Component> cubeSharedMesh = std::make_shared<MeshComponent>(m_System->GFX(), vertices, indices);
	std::shared_ptr<Component> cubeSharedShader = std::make_shared<ShaderComponent>(shaderData);
	std::shared_ptr<Component> cubeSharedPConstantBuffer = std::make_shared<PixelConstantBuffer<ConstantBuffer>>(m_System->GFX(), cb2);
	std::shared_ptr<Component> cubeSharedTopology = std::make_shared<Topology>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	


	cube.AddComponent(cubeSharedTopology);
	cube.AddComponent(cubeSharedMesh);
	cube.AddComponent(cubeSharedShader);
	cube.AddComponent(cubeSharedPConstantBuffer);

	cube.Bind(m_System->GFX());
}

void ApplicationClass::Frame()
{
	auto deltaTime = GetDeltaTime();
	m_System->GFX().ClearBuffer();

	cube.GetComponent<MeshComponent>()->Draw(m_System->GFX());
	cube.Update(deltaTime, m_System->GFX());

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


#include "ApplicationClass.h"

ApplicationClass::ApplicationClass()
{
	m_Direct3D = nullptr;
	m_Camera = nullptr;
	m_Model = nullptr;
	m_ColorShader = nullptr;
	m_System = nullptr;
	last = std::chrono::steady_clock::now();
}


bool ApplicationClass::Initialize(int screenWidth, int screenHeight)
{
	m_System = new SystemClass(screenWidth, screenHeight, "Engine");
	return true;
}

void ApplicationClass::Frame()
{
	m_System->GFX().ClearBuffer(0.f, 0.f, 0.f);

	float angle = std::chrono::duration<float>(std::chrono::steady_clock::now() - last).count();

	m_System->GFX().DrawingTriangle(angle);
	m_System->GFX().EndFrame();
}


bool ApplicationClass::Render()
{
	DirectX::XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	bool result;


	// Clear the buffers to begin the scene.
	m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_Model->Render(m_Direct3D->GetDeviceContext());

	// Render the model using the color shader.
	result = m_ColorShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix);
	if (!result)
	{
		return false;
	}

	// Present the rendered scene to the screen.
	m_Direct3D->EndScene();

	return true;
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
	// Release the color shader object.
	if (m_ColorShader)
	{
		m_ColorShader->Shutdown();
		delete m_ColorShader;
		m_ColorShader = 0;
	}

	// Release the model object.
	if (m_Model)
	{
		m_Model->Shutdown();
		delete m_Model;
		m_Model = 0;
	}

	// Release the camera object.
	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Release the Direct3D object.
	if (m_Direct3D)
	{
		m_Direct3D->Shutdown();
		delete m_Direct3D;
		m_Direct3D = nullptr;
	}

	if (m_System) {
		delete m_System;
		m_System = nullptr;
	}
}


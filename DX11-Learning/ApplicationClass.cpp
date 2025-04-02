#include "ApplicationClass.h"

ApplicationClass::ApplicationClass()
{
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

	m_System->GFX().DrawingTriangle(
		angle,
		m_System->mouse.GetPosX() / 400.0f - 1.0f,
		-m_System->mouse.GetPosY() / 300.0f + 1.0f);

	m_System->GFX().DrawingTriangle(
		angle,
		0.f,
		0.f);

	m_System->GFX().EndFrame();
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


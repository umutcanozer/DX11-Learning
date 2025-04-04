#pragma once
#include <chrono>
#include "SystemClass.h"
#include "GameObject.h"


const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.3f;

class ApplicationClass
{
public:
	ApplicationClass(int screenWidth, int screenHeight);
	ApplicationClass(const ApplicationClass&) = delete;
	ApplicationClass& operator=(const ApplicationClass&) = delete;
	~ApplicationClass();
	
	int Go();
private:
	void Frame();
	float GetDeltaTime();
private:
	SystemClass* m_System;

	std::chrono::steady_clock::time_point m_lastTime;
	GameObject cube;
};


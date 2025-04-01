#pragma once

#include <chrono>
#include "SystemClass.h"



const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.3f;

class ApplicationClass
{
public:
	ApplicationClass();
	ApplicationClass(const ApplicationClass&) = delete;
	ApplicationClass& operator=(const ApplicationClass&) = delete;
	~ApplicationClass();
	
	bool Initialize(int, int);
	int Go();

private:
	void Frame();
private:
	SystemClass* m_System;

	std::chrono::steady_clock::time_point last;
};


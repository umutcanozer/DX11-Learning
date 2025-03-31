#pragma once

#include "D3DClass.h"
#include "SystemClass.h"
#include "cameraclass.h"
#include "modelclass.h"
#include "colorshaderclass.h"


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
	bool Render();
	void Frame();
private:
	D3DClass* m_Direct3D;
	CameraClass* m_Camera;
	ModelClass* m_Model;
	ColorShaderClass* m_ColorShader;
	SystemClass* m_System;
};


#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <optional>
#include <memory>
#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics.h"

class SystemClass
{
private:
	class WindowClass
	{
	public:
		static HINSTANCE GetInstance();
		static const char* GetName();
	private:
		WindowClass();
		~WindowClass();
		WindowClass(const WindowClass&) = delete;
		WindowClass& operator=(const WindowClass&) = delete;
		static constexpr const char* wndClassName = "Engine";
		static WindowClass wndClass;
		HINSTANCE h_inst;

	private:
		LPCWSTR m_applicationName;
		int x = 10;
	};
public:
	SystemClass(int screenWidth, int screenHeight, const char* name);
	SystemClass(const SystemClass&) = delete;
	SystemClass& operator=(const SystemClass&) = delete;
	~SystemClass();

	HWND GetHWND();
	Graphics& GFX();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);
	std::optional<int> ProcessMessages();
	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
public:
	Keyboard kbd;
	Mouse mouse;
private:
	HWND m_hWnd;
	int width;
	int height;
	std::unique_ptr<Graphics> p_Gfx;
};
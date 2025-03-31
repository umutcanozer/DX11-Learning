#include "SystemClass.h"
SystemClass::WindowClass SystemClass::WindowClass::wndClass;

SystemClass::WindowClass::WindowClass()
	:
	h_inst(GetModuleHandle(nullptr))
{

	WNDCLASSEX wc = { 0 };

	// Setup the windows class with default settings.
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetInstance();
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = GetName();
	wc.cbSize = sizeof(wc);

	// Register the window class.
	RegisterClassEx(&wc);
}

HINSTANCE SystemClass::WindowClass::GetInstance()
{
	return wndClass.h_inst;
}

const char* SystemClass::WindowClass::GetName()
{
	return wndClassName;
}


SystemClass::WindowClass::~WindowClass()
{
	if (wndClass.h_inst != nullptr) {
		UnregisterClass(wndClassName, GetInstance());
		wndClass.h_inst = nullptr; 
	}
}

SystemClass::SystemClass(int screenWidth, int screenHeight, const char* name)
	:
	width(width),
	height(height)
{
	int posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
	int posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;

	m_hWnd = CreateWindowEx(WS_EX_APPWINDOW, WindowClass::GetName(), name,
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		posX, posY, screenWidth, screenHeight, NULL, NULL, WindowClass::GetInstance(), NULL);
	SetWindowLongPtr(m_hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
	ShowWindow(m_hWnd, SW_SHOW);
	SetForegroundWindow(m_hWnd);
	SetFocus(m_hWnd);

	// Hide the mouse cursor.
	ShowCursor(true);

	p_Gfx = std::make_unique<Graphics>(m_hWnd);
}

LRESULT CALLBACK SystemClass::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam) 
{
	switch (umsg)
	{
	case WM_KILLFOCUS:
		kbd.ClearState();
		break;
	case WM_KEYDOWN:
		kbd.OnKeyPressed(static_cast<unsigned char>(wparam));
		break;
	case WM_KEYUP:
		kbd.OnKeyReleased(static_cast<unsigned char>(wparam));
		break;
	case WM_CHAR:
		kbd.OnChar(static_cast<unsigned char>(wparam));
		break;
	case WM_MOUSEMOVE:
	{
		const POINTS pt = MAKEPOINTS(lparam);
		if (pt.x >= 0 && pt.x < width && pt.y >= 0 && pt.y < height)
		{
			mouse.OnMouseMove(pt.x, pt.y);
			if (!mouse.IsInWindow())
			{
				SetCapture(hwnd);
				mouse.OnMouseEnter();
			}
		}
		break;
	}
	case WM_LBUTTONDOWN:
	{
		const POINTS pt = MAKEPOINTS(lparam);
		mouse.OnLeftPressed(pt.x, pt.y);
		break;
	}
	case WM_RBUTTONDOWN:
	{
		const POINTS pt = MAKEPOINTS(lparam);
		mouse.OnRightPressed(pt.x, pt.y);
		break;
	}
	case WM_LBUTTONUP:
	{
		const POINTS pt = MAKEPOINTS(lparam);
		mouse.OnLeftReleased(pt.x, pt.y);
		break;
	}
	case WM_RBUTTONUP:
	{
		const POINTS pt = MAKEPOINTS(lparam);
		mouse.OnRightReleased(pt.x, pt.y);
		break;
	}
	case WM_MOUSEWHEEL:
	{
		const POINTS pt = MAKEPOINTS(lparam);
		if (GET_WHEEL_DELTA_WPARAM(wparam) > 0)
		{
			mouse.OnWheelUp(pt.x, pt.y);
		}
		else if (GET_WHEEL_DELTA_WPARAM(wparam) < 0)
		{
			mouse.OnWheelDown(pt.x, pt.y);
		}
		break;
	}
	// Any other messages send to the default message handler as our application won't make use of them.
	default:
		return DefWindowProc(hwnd, umsg, wparam, lparam);
	}
}

std::optional<int> SystemClass::ProcessMessages()
{
	MSG msg;

	// Initialize the message structure.
	ZeroMemory(&msg, sizeof(MSG));

	// Loop until there is a quit message from the window or the user.

	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);

		if (msg.message == WM_QUIT)
		{
			return (int)msg.wParam;
		}
	}

	return {};
}

LRESULT CALLBACK SystemClass::WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	
	SystemClass* pThis = reinterpret_cast<SystemClass*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
	switch (umessage)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	
	// All other messages pass to the message handler in the system class.
	default:
		return pThis->MessageHandler(hwnd, umessage, wparam, lparam);
	}
}

HWND SystemClass::GetHWND()
{
	return m_hWnd;
}

Graphics& SystemClass::GFX()
{
	return *p_Gfx;
}

SystemClass::~SystemClass()
{
	DestroyWindow(m_hWnd);
}



#include "Window.h"

namespace Engine
{
	Window::Window(LPCWSTR gameTitle, Input *input)
	{
		m_gameTitle		= gameTitle;
		m_input			= input;
		m_fullscreen	= false;
		m_hInstance		= NULL;
		m_hWnd			= NULL;
		m_size.cx		= 800;
		m_size.cy		= 600;

		WNDCLASSEX wc;
		DEVMODE dmScreenSettings;
		int screenWidth, screenHeight;
		int posX, posY;

		Engine::WindowHandle = this;

		m_hInstance = GetModuleHandle(NULL);

		wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		wc.lpfnWndProc = WndProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = m_hInstance;
		wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
		wc.hIconSm = wc.hIcon;
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
		wc.lpszMenuName = NULL;
		wc.lpszClassName = m_gameTitle;
		wc.cbSize = sizeof(WNDCLASSEX);

		RegisterClassEx(&wc);

		screenWidth = GetSystemMetrics(SM_CXSCREEN);
		screenHeight = GetSystemMetrics(SM_CYSCREEN);

		if(m_fullscreen)
		{
			memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
			dmScreenSettings.dmSize = sizeof(dmScreenSettings);
			dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
			dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
			dmScreenSettings.dmBitsPerPel = 32;
			dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

			ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);
			m_hWnd = CreateWindowEx(WS_EX_APPWINDOW, m_gameTitle, m_gameTitle, WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP, 0, 0, screenWidth, screenHeight, NULL, NULL, m_hInstance, NULL);
		}
		else
		{
			posX = (screenWidth - m_size.cx) / 2;
			posY = (screenHeight - m_size.cy) / 2;

			m_hWnd = CreateWindowEx(WS_EX_APPWINDOW, m_gameTitle, m_gameTitle, WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_CAPTION | WS_POPUPWINDOW, posX, posY, m_size.cx, m_size.cy, NULL, NULL, m_hInstance, NULL);
		}

		ShowWindow(m_hWnd, SW_SHOW);
		SetForegroundWindow(m_hWnd);
		SetFocus(m_hWnd);

		return;
	}
	
	Window::~Window()
	{
		if(m_fullscreen)
		{
			ChangeDisplaySettings(NULL, 0);
		}

		if(m_hWnd != NULL)
		{
			DestroyWindow(m_hWnd);
			m_hWnd = NULL;
		}
		
		if(m_hInstance != NULL)
		{
			UnregisterClass(m_gameTitle, m_hInstance);
			m_hInstance = NULL;
		}

		Engine::WindowHandle = NULL;

		return;
	}

	bool Window::isFullscreen()
	{
		return m_fullscreen;
	}

	void Window::setFullscreen( bool fullscreen )
	{
		DEVMODE dmScreenSettings;
		int screenWidth, screenHeight;
		int posX, posY;

		if(m_fullscreen != fullscreen)
		{
			m_fullscreen = fullscreen;
			if(m_hWnd != NULL)
			{

				screenWidth = GetSystemMetrics(SM_CXSCREEN);
				screenHeight = GetSystemMetrics(SM_CYSCREEN);

				if(m_fullscreen)
				{
					memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
					dmScreenSettings.dmSize = sizeof(dmScreenSettings);
					dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
					dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
					dmScreenSettings.dmBitsPerPel = 32;
					dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

					ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);
					SetWindowLong(m_hWnd, GWL_STYLE, WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP | WS_VISIBLE);
					SetWindowPos(m_hWnd, NULL, 0, 0, screenWidth, screenHeight, SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED);
				}
				else
				{
					posX = (screenWidth - m_size.cx) / 2;
					posY = (screenHeight - m_size.cy) / 2;

					ChangeDisplaySettings(NULL, 0);
					SetWindowLong(m_hWnd, GWL_STYLE, WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_CAPTION | WS_POPUPWINDOW | WS_VISIBLE);
					SetWindowPos(m_hWnd, NULL, posX, posY, m_size.cx, m_size.cy, SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED);
				}
			}
		}
	}

	SIZE Window::getSize()
	{
		return m_size;
	}

	void Window::setSize( int width, int height )
	{
		if(m_size.cx != width || m_size.cy != height)
		{
			m_size.cx = width;
			m_size.cy = height;

			if(m_hWnd != NULL)
			{
				if(!m_fullscreen)
				{
					SetWindowPos(m_hWnd, NULL, 0, 0, m_size.cx, m_size.cy, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOMOVE);
				}
			}
		}
	}

	LRESULT CALLBACK Window::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
	{
		switch(umsg)
		{
			// Check if a key has been pressed on the keyboard.
			case WM_KEYDOWN:
			{
				// If a key is pressed send it to the input object so it can record that state.
				m_input->KeyDown((unsigned int)wparam);
				return 0;
			}

			// Check if a key has been released on the keyboard.
			case WM_KEYUP:
			{
				// If a key is released then send it to the input object so it can unset the state for that key.
				m_input->KeyUp((unsigned int)wparam);
				return 0;
			}

			// Any other messages send to the default message handler as our application won't make use of them.
			default:
			{
				return DefWindowProc(hwnd, umsg, wparam, lparam);
			}
		}
	}
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	LRESULT result = 0;

	switch(umessage)
	{
		// Check if the window is being destroyed.
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}

		// Check if the window is being closed.
		case WM_CLOSE:
		{
			PostQuitMessage(0);		
			break;
		}

		// All other messages pass to the message handler in the system class.
		default:
		{
			if(Engine::WindowHandle != NULL)
			{
				result = Engine::WindowHandle->MessageHandler(hwnd, umessage, wparam, lparam);
			}
			else
			{
				result = DefWindowProc(hwnd, umessage, wparam, lparam);
			}
			break;
		}
	}

	return result;
}
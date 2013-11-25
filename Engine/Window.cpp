#include "Common.h"
#include "Window.h"
#include "Input.h"
#include "Graphics.h"

namespace Engine
{
	Window::Window(LPCWSTR gameTitle, Input *input)
	{
		m_gameTitle		= gameTitle;
		m_input			= input;
		m_fullscreen	= false;
		m_hInstance		= NULL;
		m_hWnd			= NULL;
	}

	bool Window::Initialize(unsigned int width, unsigned int height)
	{
		WNDCLASSEX wc;
		DEVMODE dmScreenSettings;
		unsigned int screenWidth, screenHeight, posX, posY;

		Engine::WindowHandle = this;

		m_hInstance = GetModuleHandle(NULL);

		ZeroMemory(&wc, sizeof(wc));
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc = WndProc;
		wc.hInstance = m_hInstance;
		wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
		wc.hIconSm = wc.hIcon;
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
		wc.lpszClassName = m_gameTitle;

		RegisterClassEx(&wc);

		screenWidth = GetSystemMetrics(SM_CXSCREEN);
		screenHeight = GetSystemMetrics(SM_CYSCREEN);

		posX = (screenWidth - width) / 2;
		posY = (screenHeight - height) / 2;

		m_hWnd = CreateWindowEx(WS_EX_APPWINDOW, m_gameTitle, m_gameTitle, WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_POPUPWINDOW | WS_CAPTION, posX, posY, width, height, NULL, NULL, m_hInstance, NULL);

		ShowWindow(m_hWnd, SW_SHOW);
		SetForegroundWindow(m_hWnd);
		SetFocus(m_hWnd);

		return true;
	}
	
	void Window::Shutdown()
	{
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

	HWND Window::GetHWnd()
	{
		return m_hWnd;
	}

	SIZE Window::GetSize()
	{
		SIZE size;
		RECT rect;

		GetWindowRect(m_hWnd, &rect);
		size.cx = rect.right - rect.left;
		size.cy = rect.bottom - rect.top;

		return size;
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

			case WM_SYSCHAR:
			{
				if(wparam == VK_RETURN)
				{
					if(m_graphics != NULL)
					{
						m_graphics->SetFullscreen( !m_graphics->IsFullscreen() );
					}
					return 0;
				}
				return DefWindowProc(hwnd, umsg, wparam, lparam);
			}

			case WM_KILLFOCUS:
			{
				if(m_graphics != NULL)
				{
					m_graphics->SetFullscreen( false );
				}
				return 0;
			}

			case WM_SIZE:
			{
				if(m_graphics != NULL)
				{
					m_graphics->ResizeBuffers( LOWORD(lparam), HIWORD(lparam) );
				}
				return 0;
			}

			// Any other messages send to the default message handler as our application won't make use of them.
			default:
			{
				return DefWindowProc(hwnd, umsg, wparam, lparam);
			}
		}
	}

	void Window::SetGraphics( Graphics *graphics )
	{
		m_graphics = graphics;
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
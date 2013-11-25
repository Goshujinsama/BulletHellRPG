#ifndef WINDOW_H
#define WINDOW_H

#include <Windows.h>

namespace Engine
{
	class Input;
	class Graphics;

	class Window
	{
	public:
		Window(LPCWSTR gameTitle, Input *input);

		bool Initialize(unsigned int width, unsigned int height);
		void Shutdown();

		void Run();
		LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

		void SetGraphics( Graphics *graphics );

		HWND GetHWnd();
		SIZE GetSize();

	private:
		LPCWSTR		m_gameTitle;
		bool		m_fullscreen;
		HINSTANCE	m_hInstance;
		HWND		m_hWnd;
		SIZE		m_size;

		Input		*m_input;
		Graphics	*m_graphics;
	};

	static Window* WindowHandle = NULL;
}

static LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);


#endif // GAME_H
#ifndef WINDOW_H
#define WINDOW_H

#include <Windows.h>

#include "Input.h"

namespace Engine
{
	class Window
	{
	public:
		Window(LPCWSTR gameTitle, Input *input);
		~Window();

		void Run();
		LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

	protected:
		bool isFullscreen();
		void setFullscreen( bool fullscreen );
		SIZE getSize();
		void setSize( int width, int height );

	private:
		LPCWSTR		m_gameTitle;
		bool		m_fullscreen;
		HINSTANCE	m_hInstance;
		HWND		m_hWnd;
		SIZE		m_size;

		Input		*m_input;
	};

	static Window* WindowHandle = NULL;
}

static LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);


#endif // GAME_H
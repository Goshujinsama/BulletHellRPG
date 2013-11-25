#include "Window.h"

#include "Common.h"
#include "Game.h"
#include "Input.h"
#include "Window.h"
#include "Graphics.h"

namespace Engine
{
	Game::Game(wchar_t *gameTitle)
	{
		m_input		= new Input();
		m_window	= new Window(gameTitle, m_input);
		m_graphics	= new Graphics(m_window);
	}
	
	Game::~Game()
	{
		SAFE_DELETE(m_graphics);
		SAFE_DELETE(m_window);
		SAFE_DELETE(m_input);
	}

	void Game::Run()
	{
		MSG msg;
		bool bRunning;

		if( Initialize() &&
			m_window->Initialize(800, 600) &&
			m_graphics->Initialize() )
		{
			ZeroMemory(&msg, sizeof(MSG));

			bRunning = true;
			while(bRunning)
			{
				if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}

				if(msg.message == WM_QUIT)
				{
					bRunning = false;
				}
				else
				{
					if(Update(0.0f, 0))
					{
						m_graphics->Begin();
						Render();
						m_graphics->End();
					}
					else
					{
						bRunning = false;
					}
				}
			}
		}

		m_graphics->Shutdown();
		m_window->Shutdown();
		Shutdown();

		return;
	}

	Input *Game::GetInput()
	{
		return m_input;
	}

	Graphics *Game::GetGraphics()
	{
		return m_graphics;
	}
}

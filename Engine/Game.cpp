#include "Window.h"

#include "Game.h"

namespace Engine
{
	Game::Game(LPCWSTR gameTitle)
	{
		m_input = new Input();
		m_window = new Window(gameTitle, m_input);
	}
	
	Game::~Game()
	{
		if(m_window != NULL)
		{
			delete(m_window);
			m_window = NULL;
		}

		if(m_input != NULL)
		{
			delete(m_input);
			m_input = NULL;
		}
	}

	void Game::Run()
	{
		MSG msg;
		bool bRunning;

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
					Render();
				}
				else
				{
					bRunning = false;
				}
			}
		}

		return;
	}

	Input	*Game::getInput()
	{
		return m_input;
	}
}

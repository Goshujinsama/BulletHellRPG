#include "Engine.h"

namespace Engine
{
	Game::Game()
	{
	}

	Game::~Game()
	{
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
}
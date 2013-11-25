#ifndef ENGINE_H
#define ENGINE_H

#include "Common.h"
#include "Game.h"

#define START_GAME(x) int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) \
{								\
	Engine::Game *game;			\
								\
	game = new x();				\
	if(game)					\
	{							\
		game->Run();			\
	}							\
	SAFE_DELETE(game);			\
								\
	return 0;					\
}                                                                                                                \

#endif // ENGINE_H
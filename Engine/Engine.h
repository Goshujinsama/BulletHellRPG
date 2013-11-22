#ifndef ENGINE_H
#define ENGINE_H

#include "Game.h"

#define START_GAME(x) int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) \
{								\
	Engine::Game *game;			\
	bool ret;					\
								\
	game = new x();				\
	if(!game)					\
	{							\
		return 1;				\
	}							\
	ret = game->Initialize();	\
	if(ret)						\
	{							\
		game->Run();			\
	}							\
								\
	game->Shutdown();			\
	delete game;				\
	game = NULL;				\
								\
	return 0;					\
}                                                                                                                      \

#endif // ENGINE_H
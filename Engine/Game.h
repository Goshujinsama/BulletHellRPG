#ifndef GAME_H
#define GAME_H

#include "Window.h"

namespace Engine
{
	class Game
	{
	public:
		Game(LPCWSTR gameTitle);
		~Game();

		void Run();

		virtual bool Initialize() = 0;
		virtual bool Update(float delta, long time) = 0;
		virtual void Render() = 0;
		virtual void Shutdown() = 0;

	protected:
		Input	*getInput();

	private:
		Input	*m_input;
		Window	*m_window;
	};
}

#endif // GAME_H
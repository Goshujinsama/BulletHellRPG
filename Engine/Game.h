#ifndef GAME_H
#define GAME_H

namespace Engine
{
	class Game
	{
	public:
		Game();
		~Game();

		void Run();

		virtual bool Initialize() = 0;
		virtual bool Update(float delta, long time) = 0;
		virtual void Render() = 0;
		virtual void Shutdown() = 0;
	};
}

#endif // GAME_H
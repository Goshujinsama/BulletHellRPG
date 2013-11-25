#ifndef GAME_H
#define GAME_H

namespace Engine
{
	class Input;
	class Window;
	class Graphics;

	class Game
	{
	public:
		Game(wchar_t *gameTitle);
		~Game();

		void Run();

	protected:
		virtual bool Initialize() = 0;
		virtual bool Update(float delta, long time) = 0;
		virtual void Render() = 0;
		virtual void Shutdown() = 0;

		Input		*GetInput();
		Graphics	*GetGraphics();

	private:
		Input		*m_input;
		Window		*m_window;
		Graphics	*m_graphics;
	};
}

#endif // GAME_H
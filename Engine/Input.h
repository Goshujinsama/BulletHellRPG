#ifndef INPUT_H
#define INPUT_H

namespace Engine
{
	class Input
	{
	public:
		Input();

		void KeyDown(unsigned int key);
		void KeyUp(unsigned int key);

		bool IsKeyDown(unsigned int key);
		bool WasKeyPressed(unsigned int key);

	private:
		bool m_keysDown[256];
		bool m_keysPressed[256];
	};
}

#endif // INPUT_H
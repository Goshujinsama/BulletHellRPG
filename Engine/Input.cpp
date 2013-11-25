#include "Input.h"

namespace Engine
{
	Input::Input()
	{
		int i;

		for(i = 0; i < 256; ++i)
		{
			m_keysDown[i] = false;
			m_keysPressed[i] = false;
		}
	}

	void Input::KeyDown(unsigned int key)
	{
		m_keysDown[key] = true;
		m_keysPressed[key] = true;
	}

	void Input::KeyUp(unsigned int key)
	{
		m_keysDown[key] = false;
	}

	bool Input::IsKeyDown(unsigned int key)
	{
		return m_keysDown[key];
	}

	bool Input::WasKeyPressed(unsigned int key)
	{
		bool result;

		result = m_keysPressed[key];
		m_keysPressed[key] = false;

		return result;
	}
}
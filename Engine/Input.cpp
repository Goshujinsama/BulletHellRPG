#include "Input.h"

namespace Engine
{
	Input::Input()
	{
		int i;

		for(i = 0; i < 256; ++i)
		{
			m_keys[i] = false;
		}
	}

	void Input::KeyDown(unsigned int key)
	{
		m_keys[key] = true;
	}

	void Input::KeyUp(unsigned int key)
	{
		m_keys[key] = false;
	}

	bool Input::IsKeyDown(unsigned int key)
	{
		return m_keys[key];
	}
}
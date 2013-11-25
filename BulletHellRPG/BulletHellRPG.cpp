#include <Windows.h>

#include "Engine.h"
#include "Input.h"
#include "Graphics.h"
#include "Sprite.h"
#include "Image.h"

class BulletHellRPG : public Engine::Game
{
public:
	Engine::Sprite *m_sprite;
	Engine::Image *m_image;

	BulletHellRPG() : Engine::Game(L"Bullet Hell RPG")
	{
		m_sprite = NULL;
	}

	virtual bool Initialize()
	{
		return true;
	};

	virtual bool Update(float delta, long time)
	{
		bool result = true;

		if(m_sprite == NULL)
		{
			m_image = new Engine::Image();
			m_image->Initialize(L"test.png", GetGraphics());
			m_sprite = new Engine::Sprite(GetGraphics(), L"", 1, 1);
		}

		if(GetInput()->IsKeyDown(VK_ESCAPE))
		{
			result = false;
		}

		return result;
	};

	virtual void Render()
	{
		m_sprite->Enqueue(0, 0.3, 0.3, 0.1, 0.1);
		m_sprite->Enqueue(0, 0.5, 0.5, 0.1, 0.1);
		m_sprite->Enqueue(0, 0.7, 0.7, 0.1, 0.1);
		m_sprite->Draw();

		return;
	};

	virtual void Shutdown()
	{
		return;
	};
};

START_GAME(BulletHellRPG)
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
			m_sprite = new Engine::Sprite(GetGraphics(), m_image, 1, 1);
			m_sprite->Initialize();
		}

		if(GetInput()->IsKeyDown(VK_ESCAPE))
		{
			result = false;
		}

		return result;
	};

	virtual void Render()
	{
		float ratio = (float)GetGraphics()->GetResolution().cx / GetGraphics()->GetResolution().cy;

		GetGraphics()->SetViewPort(0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f);
		m_sprite->Enqueue(0, -0.5, 0.3, 0.1, 0.1, 0.1 * ratio);
		m_sprite->Enqueue(0, 0.5, 0.0, 0.5, 0.1, 0.1 * ratio);
		m_sprite->Enqueue(0, 0.6, 0.1, 0.3, 0.1, 0.1 * ratio);
		m_sprite->Draw();

		return;
	};

	virtual void Shutdown()
	{
		return;
	};
};

START_GAME(BulletHellRPG)
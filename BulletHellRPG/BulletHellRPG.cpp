#include "Engine.h"

class BulletHellRPG : public Engine::Game
{
public:
	BulletHellRPG() : Engine::Game(L"Bullet Hell RPG") {}

	virtual bool Initialize()
	{
		return true;
	};

	virtual bool Update(float delta, long time)
	{
		bool result = true;

		if(getInput()->IsKeyDown(VK_ESCAPE))
		{
			result = false;
		}

		return result;
	};

	virtual void Render()
	{
		return;
	};

	virtual void Shutdown()
	{
		return;
	};
};

START_GAME(BulletHellRPG)
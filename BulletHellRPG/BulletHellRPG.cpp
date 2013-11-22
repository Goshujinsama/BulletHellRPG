#include "Engine.h"

class BulletHellRPG : public Engine::Game
{
public:
	BulletHellRPG() {}

	virtual bool Initialize() { return true; };
	virtual bool Update(float delta, long time) { return true; };
	virtual void Render() {};
	virtual void Shutdown() {};
};

START_GAME(BulletHellRPG)
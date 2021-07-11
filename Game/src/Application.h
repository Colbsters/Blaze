#pragma once
#include <Blaze/Blaze.h>

class Game
	:public Blaze::GameApp
{
public:
	Game(Blaze::GameAppConfig config)
		:Blaze::GameApp(config)
	{
	}

	virtual void OnCreate() override
	{

	}

	virtual void OnDestroy() override
	{

	}

	virtual bool OnUpdate() override
	{
		GetWindow()->Update();
		return GetWindow()->IsRunning();
	}

private:
};

Blaze::Application* CreateApplication()
{
	return new Game{
		Blaze::GameAppConfig{
			"Blaze Window",
			960, 540
		}
	};
}

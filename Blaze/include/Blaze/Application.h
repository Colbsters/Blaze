#pragma once

#ifndef BLAZE_APPLICATION_H
#define BLAZE_APPLICATION_H

#include <Blaze/Window.h>

#include <string_view>

namespace Blaze
{
	// Application base class
	class Application
	{
	public:
		virtual ~Application() = default;

		virtual void OnCreate() = 0;
		virtual void OnDestroy() = 0;
		virtual bool OnUpdate() = 0;
	};

	struct GameAppConfig
	{
		GameAppConfig() = default;

		WindowCreateInfo windowInfo;
	};

	// GameApp base class, similair to the Application class, but creates things like a window
	class GameApp
		:public Application
	{
	public:
		inline GameApp(const GameAppConfig& config)
		{
			m_window = Window::Create(config.windowInfo);
		}

		virtual ~GameApp() = default;

		inline Ref<Window> GetWindow() { return m_window; }
	private:
		Ref<Window> m_window;
	};

}

extern Blaze::Application* CreateApplication();

#endif // BLAZE_APPLICATION_H
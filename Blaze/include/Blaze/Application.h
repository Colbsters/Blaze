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
		GameAppConfig(std::string_view windowTitle, uint32_t renderWidth, uint32_t renderHeight)
			:windowTitle(windowTitle), renderWidth(renderWidth), renderHeight(renderHeight)
		{}

		std::string_view windowTitle;
		uint32_t renderWidth, renderHeight;
	};

	// GameApp base class, similair to the Application class, but creates things like a window
	class GameApp
		:public Application
	{
	public:
		inline GameApp(const GameAppConfig& config)
		{
			WindowCreateInfo wndInfo;
			wndInfo.wndTitle = config.windowTitle;
			wndInfo.x = 200;
			wndInfo.y = 100;
			wndInfo.width = config.renderWidth;
			wndInfo.height = config.renderHeight;
			m_window = Window::Create(wndInfo);
		}

		virtual ~GameApp() = default;

		inline Ref<Window> GetWindow() { return m_window; }
	private:
		Ref<Window> m_window;
	};

}

extern Blaze::Application* CreateApplication();

#endif // BLAZE_APPLICATION_H
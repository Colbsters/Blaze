#pragma once
#include <Blaze/Blaze.h>
#include <iostream>

void LogWindowEvent(const Blaze::WindowEvent& event, void*)
{
	constexpr const char* keyStrings[150]
	{
		"Null",
		"Backspace",
		"Delete",
		"Tab",
		"Clear",
		"Enter",
		"Pause",
		"Escape",
		"LeftShift",
		"RightShift",
		"LeftControl",
		"RightControl",
		"LeftAlt",
		"RightAlt",
		"LeftWindows",
		"RightWindows",
		"CapsLock",
		"NumberLock",
		"ScollLock",
		"Insert",
		"PageUp",
		"PageDown",
		"End",
		"Home",
		"LeftArrow",
		"RightArrow",
		"UpArrow",
		"DownArrow",
		"Print",
		"PrintScreen",
		"Help",
		"Unknown",
		"SpaceBar",
		"Unknown",
		"Unknown",
		"Unknown",
		"Unknown",
		"Unknown",
		"Unknown",
		"'\"",
		"Unknown",
		"Unknown",
		"Unknown",
		"+=",
		",<",
		"-_",
		".>",
		"/?",
		"0",
		"1",
		"2",
		"3",
		"4",
		"5",
		"6",
		"7",
		"8",
		"9",
		"Unknown",
		";:",
		"Unknown",
		"Unknown",
		"Unknown",
		"Unknown",
		"Unknown",
		"A",
		"B",
		"C",
		"D",
		"E",
		"F",
		"G",
		"H",
		"I",
		"J",
		"K",
		"L",
		"M",
		"N",
		"O",
		"P",
		"Q",
		"R",
		"S",
		"T",
		"U",
		"V",
		"W",
		"X",
		"Y",
		"Z",
		"[{",
		"\\|",
		"]}",
		"Unknown",
		"Unknown",
		"`~",
		"Menu",
		"Numpad0",
		"Numpad1",
		"Numpad2",
		"Numpad3",
		"Numpad4",
		"Numpad5",
		"Numpad6",
		"Numpad7",
		"Numpad8",
		"Numpad9",
		"NumpadPlus",
		"NumpadMinus",
		"NumpadMultiply",
		"NumpadDivide",
		"NumpadDecimalPoint",
		"F1",
		"F2",
		"F3",
		"F4",
		"F5",
		"F6",
		"F7",
		"F8",
		"F9",
		"F10",
		"F11",
		"F12",
		"F13",
		"F14",
		"F15",
		"F16",
		"F17",
		"F18",
		"F19",
		"F20",
		"F21",
		"F22",
		"F23",
		"F24",
		"BrowserBack",
		"BrowserForward",
		"BrowserRefresh",
		"BrowserStop",
		"BrowserSearch",
		"BrowserFavorites",
		"BrowserHome",
		"MediaNextTrack",
		"MediaPreviousTrack",
		"MediaStop",
		"MediaPlayPause",
		"Play",
		"Zoom"
	};
	constexpr const char* mouseButtonStrings[]
	{
		"Null",
		"Left",
		"Middle",
		"Right",
		"XButton1",
		"XButton2"
	};
	switch (event.eventCode)
	{
	case Blaze::WindowEvent::Create: std::cout << "Window created\n"; break;
	case Blaze::WindowEvent::Destroy: std::cout << "Window destroyed\n"; break;
	case Blaze::WindowEvent::Resize:
	{
		auto eventInfo = event.GetWindowEventInfo<Blaze::WindowResizeEventInfo>();
		std::cout << "Window resized; width = " << eventInfo.width << ", height = " << eventInfo.height << '\n';
		break;
	}
	case Blaze::WindowEvent::Move:
	{
		auto eventInfo = event.GetWindowEventInfo<Blaze::WindowMoveEventInfo>();
		std::cout << "Window moved; x = " << eventInfo.x << ", y = " << eventInfo.y << '\n';
		break;
	}
	case Blaze::WindowEvent::KeyDown:
	{
		auto eventInfo = event.GetWindowEventInfo<Blaze::WindowKeyDownEventInfo>();
		std::cout << keyStrings[static_cast<uint32_t>(eventInfo.key)] << " key pressed\n";
		break;
	}
	case Blaze::WindowEvent::KeyUp:
	{
		auto eventInfo = event.GetWindowEventInfo<Blaze::WindowKeyUpEventInfo>();
		std::cout << keyStrings[static_cast<uint32_t>(eventInfo.key)] << " key released\n";
		break;
	}
	case Blaze::WindowEvent::MouseButtonDown:
	{
		auto eventInfo = event.GetWindowEventInfo<Blaze::WindowMouseButtonDownEventInfo>();
		std::cout << mouseButtonStrings[static_cast<uint32_t>(eventInfo.button)] << " mouse button pressed; x = " << eventInfo.x << ", y = " << eventInfo.y << '\n';
		break;
	}
	case Blaze::WindowEvent::MouseButtonUp:
	{
		auto eventInfo = event.GetWindowEventInfo<Blaze::WindowMouseButtonUpEventInfo>();
		std::cout << mouseButtonStrings[static_cast<uint32_t>(eventInfo.button)] << " mouse button released; x = " << eventInfo.x << ", y = " << eventInfo.y << '\n';
		break;
	}
	case Blaze::WindowEvent::MouseMove:
	{
		auto eventInfo = event.GetWindowEventInfo<Blaze::WindowMouseMoveEventInfo>();
		std::cout << "Mouse moved; x = " << eventInfo.x << ", y = " << eventInfo.y << '\n';
		break;
	}
	default:
		std::cout << "Unknown event\n";
		break;
	}
}

class Game
	:public Blaze::GameApp
{
public:
	Game(Blaze::GameAppConfig config)
		:Blaze::GameApp(config), m_keyboard(GetWindow())
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
	Blaze::KeyboardInput m_keyboard;
};

Blaze::Application* CreateApplication()
{
	Blaze::WindowCreateInfo windowInfo;
	windowInfo.wndTitle = "Blaze Window";
	windowInfo.x = 200;
	windowInfo.y = 100;
	windowInfo.width = 960;
	windowInfo.height = 540;
	windowInfo.eventHandlers = {
		{ LogWindowEvent, nullptr }
	};

	return new Game{
		Blaze::GameAppConfig{
			windowInfo
		}
	};
}

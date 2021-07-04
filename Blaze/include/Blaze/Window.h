#pragma once

#ifndef BLAZE_WINDOW_H
#define BLAZE_WINDOW_H

#include <Blaze/Core.h>
#include <Blaze/Object.h>

namespace Blaze
{
	namespace Details
	{
		enum class WindowAPI
		{
			Null = 0,
			Invalid = Null
		};
	}

	struct WindowCreateInfo
		:public ObjectCreateInfo
	{
		std::string wndTitle;
		int32_t x, y;
		uint32_t width, height;
	};

	struct WindowEvent
	{
		enum EventCode
		{
			Null = 0,
			Invalid = Null,
			Create,
			Destroy,
			Resize,
			Move,
			KeyDown,
			KeyUp,
			MouseClick,
			MouseMove
		};

		EventCode eventCode;
	};

	typedef void(*WindowEventHandler)(const WindowEvent& event);

	class BLAZE_API Window
		:public Object
	{
	public:
		inline Window() { classID = GetStaticClassID(); }

		// Creates a window
		static Ref<Window> Create(const WindowCreateInfo& createInfo);

		constexpr static ClassID GetStaticClassID() { return Details::MakeClassID(Details::InterfaceID::Window, Details::ImplementationID::Invalid); }

		// Updates the window
		inline Result Update() { return Update_Impl(); }
		// Returns true while the should be updated
		inline bool IsRunning() { return IsRunning_Impl(); }

		// Pushes an event handler
		inline Result PushEventHandler(WindowEvent::EventCode eventCode, WindowEventHandler eventHandler) { return PushEventHandler_Impl(eventCode, eventHandler); }
		// Removes an event handler
		inline Result RemoveEventHandler(WindowEvent::EventCode eventCode, WindowEventHandler eventHandler) { return RemoveEventHandler_Impl(eventCode, eventHandler); }

		// Resizes the window
		inline Result Resize(uint32_t width, uint32_t height) { return Resize_Impl(width, height); }
		// Moves the window
		inline Result Move(int32_t x, int32_t y) { return Move_Impl(x, y); }
	private:
		virtual Result Update_Impl() = 0;
		virtual bool IsRunning_Impl() = 0;
		virtual Result PushEventHandler_Impl(WindowEvent::EventCode eventCode, WindowEventHandler eventHandler) = 0;
		virtual Result RemoveEventHandler_Impl(WindowEvent::EventCode eventCode, WindowEventHandler eventHandler) = 0;
		virtual Result Resize_Impl(uint32_t width, uint32_t height) = 0;
		virtual Result Move_Impl(int32_t x, int32_t y) = 0;
	};
}

#endif // BLAZE_WINDOW_H
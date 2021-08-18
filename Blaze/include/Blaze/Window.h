#pragma once

#ifndef BLAZE_WINDOW_H
#define BLAZE_WINDOW_H

#include <Blaze/Core.h>
#include <Blaze/Object.h>
#include <Blaze/Input.h>

#include <string>
#include <string_view>
#include <array>

namespace Blaze
{
	class BLAZE_API Window;

	namespace Details
	{
		enum class WindowAPI
		{
			Null = 0,
			Invalid = Null,
			Win32
		};
	}

#pragma region Window___EventInfo structures

	struct WindowResizeEventInfo
	{
		uint32_t width, height;
	};

	struct WindowMoveEventInfo
	{
		int32_t x, y;
	};

	struct WindowKeyUpEventInfo
	{
		KeyCode key;
	};

	struct WindowKeyDownEventInfo
	{
		KeyCode key;
	};

	struct WindowMouseButtonUpEventInfo
	{
		uint32_t x, y;
		MouseButton button;
	};

	struct WindowMouseButtonDownEventInfo
	{
		uint32_t x, y;
		MouseButton button;
	};

	struct WindowMouseMoveEventInfo
	{
		uint32_t x, y;
	};

#pragma endregion

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
			MouseButtonDown,
			MouseButtonUp,
			MouseMove,
			NumEvents = 9
		};

		Ref<Window> window;
		EventCode eventCode = EventCode::Null;

		uint64_t reserved[2];

		template<typename WindowEventInfo>
		WindowEventInfo GetWindowEventInfo() const
		{
			WindowEventInfo info;
			std::memcpy(&info, reserved, std::min(sizeof(info), sizeof(reserved)));
			return info;
		}
	};

	struct WindowEventHandler
	{
		bool operator==(const WindowEventHandler& rhs) { return (eventHandler == rhs.eventHandler) && (data == rhs.data); }

		typedef void(*EventHandler)(const WindowEvent&, void*);
		EventHandler eventHandler;
		void* data;
	};

	struct WindowCreateInfo
		:public ObjectCreateInfo
	{
		std::string wndTitle;
		int32_t x, y;
		uint32_t width, height;
		// These event handlers wil get called for all events;
		std::vector<WindowEventHandler> eventHandlers;
	};

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

		// Pushes an event handler, EventCode::Null means the event handler will be called for all events
		inline Result PushEventHandler(WindowEvent::EventCode eventCode, WindowEventHandler eventHandler) { return PushEventHandler_Impl(eventCode, eventHandler); }
		// Removes an event handler
		inline Result RemoveEventHandler(WindowEvent::EventCode eventCode, WindowEventHandler eventHandler) { return RemoveEventHandler_Impl(eventCode, eventHandler); }

		// Sets te window title
		inline Result SetTitle(std::string_view newTitle) { return SetTitle_Impl(newTitle); }
		// Gets the window title
		inline std::string GetTitle() { return GetTitle_Impl(); }
		// Resizes the window
		inline Result Resize(uint32_t width, uint32_t height) { return Resize_Impl(width, height); }
		// Gets the window size
		inline std::array<uint32_t, 2> GetWindowSize() { return GetWindowSize_Impl(); }
		// Gets the window client (renderable area) size
		inline std::array<uint32_t, 2> GetClientSize() { return GetClientSize_Impl(); }
		// Moves the window
		inline Result Move(int32_t x, int32_t y) { return Move_Impl(x, y); }
		// Gets the position of the window
		inline std::array<int32_t, 2> GetPosition() { return GetPosition_Impl(); }
	private:
		virtual Result Update_Impl() = 0;
		virtual bool IsRunning_Impl() = 0;
		virtual Result PushEventHandler_Impl(WindowEvent::EventCode eventCode, WindowEventHandler eventHandler) = 0;
		virtual Result RemoveEventHandler_Impl(WindowEvent::EventCode eventCode, WindowEventHandler eventHandler) = 0;
		virtual Result SetTitle_Impl(std::string_view newTitle) = 0;
		virtual std::string GetTitle_Impl() = 0;
		virtual Result Resize_Impl(uint32_t width, uint32_t height) = 0;
		virtual std::array<uint32_t, 2> GetWindowSize_Impl() = 0;
		virtual std::array<uint32_t, 2> GetClientSize_Impl() = 0;
		virtual Result Move_Impl(int32_t x, int32_t y) = 0;
		virtual std::array<int32_t, 2> GetPosition_Impl() = 0;
	};
}

#endif // BLAZE_WINDOW_H
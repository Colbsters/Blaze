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
	namespace Details
	{
		enum class WindowAPI
		{
			Null = 0,
			Invalid = Null,
			Win32
		};
	}

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

		inline static EventCode GetStaticEventCode() { return EventCode::Null; }
		inline WindowEvent* CastTo(EventCode event) { return (eventCode == event) || event == EventCode::Null ? this : nullptr; }
		inline const WindowEvent* CastTo(EventCode event) const { return (eventCode == event) || event == EventCode::Null ? this : nullptr; }
		template <typename T>
		inline T& CastTo()
		{
			auto* ptr = static_cast<T*>(CastTo(T::GetStaticEventCode()));
			if (!ptr)
				throw Exception(Result::InvalidCast, "Invalid CastTo event");
			return *ptr;
		}
		template <typename T>
		inline T& CastTo(std::nothrow_t)
		{
			auto* ptr = static_cast<T*>(CastTo(T::GetStaticEventCode()));
			if (!ptr)
				T{ EventCode::Null };
			return *ptr;
		}
		template <typename T>
		inline const T& CastTo() const
		{
			const auto* ptr = static_cast<const T*>(CastTo(T::GetStaticEventCode()));
			if (!ptr)
				throw Exception(Result::InvalidCast, "Invalid CastTo event");
			return *ptr;
		}
		template <typename T>
		inline const T& CastTo(std::nothrow_t) const
		{
			const auto* ptr = static_cast<const T*>(CastTo(T::GetStaticEventCode()));
			if (!ptr)
				return T{ EventCode::Null };
			return *ptr;
		}

		Ref<Window> window;
		EventCode eventCode;
	};

#pragma region Window Events

	struct WindowCreateEvent
		:public WindowEvent
	{
		inline static EventCode GetStaticEventCode() { return EventCode::Create; }
	};

	struct WindowDestroyEvent
		:public WindowEvent
	{
		inline static EventCode GetStaticEventCode() { return EventCode::Destroy; }
	};

	struct WindowResizeEvent
		:public WindowEvent
	{
		inline static EventCode GetStaticEventCode() { return EventCode::Resize; }
		uint32_t width, height;
	};

	struct WindowMoveEvent
		:public WindowEvent
	{
		inline static EventCode GetStaticEventCode() { return EventCode::Move; }
		int32_t x, y;
	};

	struct WindowKeyUpEvent
		:public WindowEvent
	{
		inline static EventCode GetStaticEventCode() { return EventCode::KeyUp; }
		KeyCode key;
	};

	struct WindowKeyDownEvent
		:public WindowEvent
	{
		inline static EventCode GetStaticEventCode() { return EventCode::KeyDown; }
		KeyCode key;
	};

	struct WindowMouseClickEvent
		:public WindowEvent
	{
		inline static EventCode GetStaticEventCode() { return EventCode::MouseClick; }
		MouseButton key;
	};

	struct WindowMouseMoveEvent
		:public WindowEvent
	{
		inline static EventCode GetStaticEventCode() { return EventCode::MouseMove; }
		uint32_t width, height;
	};

#pragma endregion

	struct WindowEventHandler
	{
		typedef void(*EventHandler)(const WindowEvent&, void*);
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

		// Pushes an event handler
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
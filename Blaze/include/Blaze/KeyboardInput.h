#pragma once

#ifndef BLAZE_KEYBOARDINPUT_H
#define BLAZE_KEYBOARDINPUT_H

#include <Blaze/Core.h>
#include <Blaze/Window.h>
#include <Blaze/InputBase.h>

#include <bitset>

namespace Blaze
{
	class KeyboardInput
	{
	public:
		inline KeyboardInput() = default;
		inline KeyboardInput(const KeyboardInput& other)
			:m_window(other.m_window)
		{
			m_window->PushEventHandler(WindowEvent::KeyDown, { KeyDownHandler, this });
			m_window->PushEventHandler(WindowEvent::KeyUp, { KeyUpHandler, this });
		}
		inline KeyboardInput(Ref<Window> window)
			:m_window(window)
		{
			m_window->PushEventHandler(WindowEvent::KeyDown, { KeyDownHandler, this });
			m_window->PushEventHandler(WindowEvent::KeyUp, { KeyUpHandler, this });
		}
		inline ~KeyboardInput()
		{
			m_window->RemoveEventHandler(WindowEvent::KeyDown, { KeyDownHandler, this });
			m_window->RemoveEventHandler(WindowEvent::KeyUp, { KeyUpHandler, this });
		}

		const KeyboardInput& operator=(const KeyboardInput& other)
		{
			SetWindow(other.m_window);
			return *this;
		}

		// Returns the old window
		inline Ref<Window> SetWindow(Ref<Window> window)
		{
			// Reset the keys
			m_keys.reset();
			// Remove the event handlers from the old window
			m_window->RemoveEventHandler(WindowEvent::KeyDown, { KeyDownHandler, this });
			m_window->RemoveEventHandler(WindowEvent::KeyUp, { KeyUpHandler, this });
			// Save the window so it can be returned
			auto oldWindow = m_window;
			// Setup the new window
			m_window = window;
			m_window->PushEventHandler(WindowEvent::KeyDown, { KeyDownHandler, this });
			m_window->PushEventHandler(WindowEvent::KeyUp, { KeyUpHandler, this });
			// Return the old window
			return oldWindow;
		}

		inline bool IsKeyPressed(KeyCode key)
		{
			if (key == KeyCode::Invalid)
				return false;
			return m_keys[static_cast<size_t>(key) - 1];
		}

		inline Ref<Window> GetWindow() { return m_window; }
	private:
		inline static void KeyDownHandler(const WindowEvent& event, void* data)
		{
			auto& keyboardInput = *reinterpret_cast<KeyboardInput*>(data);
			auto eventInfo = event.GetWindowEventInfo<WindowKeyDownEventInfo>();
			keyboardInput.m_keys[static_cast<size_t>(eventInfo.key) - 1] = true;
		}
		inline static void KeyUpHandler(const WindowEvent& event, void* data)
		{
			auto& keyboardInput = *reinterpret_cast<KeyboardInput*>(data);
			auto eventInfo = event.GetWindowEventInfo<WindowKeyUpEventInfo>();
			keyboardInput.m_keys[static_cast<size_t>(eventInfo.key) - 1] = false;
		}

		Ref<Window> m_window;
		std::bitset<149> m_keys;
	};
}

#endif // BLAZE_KEYBOARDINPUT_H
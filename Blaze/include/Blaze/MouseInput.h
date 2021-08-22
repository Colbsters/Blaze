#pragma once

#ifndef BLAZE_MOUSEINPUT_H
#define BLAZE_MOUSEINPUT_H

#include <Blaze/Core.h>
#include <Blaze/Window.h>
#include <Blaze/InputBase.h>

#include <bitset>

namespace Blaze
{
	class MouseInput
	{
	public:
		inline MouseInput() = default;
		inline MouseInput(const MouseInput& other)
			:m_window(other.m_window), m_buttons(other.m_buttons), m_posX(other.m_posX), m_posY(other.m_posY)
		{
		}
		inline MouseInput(Ref<Window> window)
			:m_window(window), m_buttons(), m_posX(0), m_posY(0)
		{
			m_window->PushEventHandler(WindowEvent::MouseButtonDown, { ButtonDownHandler, this });
			m_window->PushEventHandler(WindowEvent::MouseButtonUp, { ButtonUpHandler, this });
			m_window->PushEventHandler(WindowEvent::MouseMove, { MouseMoveHandler, this });
		}
		inline ~MouseInput()
		{
			if (m_window)
			{
				m_window->RemoveEventHandler(WindowEvent::MouseButtonDown, { ButtonDownHandler, this });
				m_window->RemoveEventHandler(WindowEvent::MouseButtonUp, { ButtonUpHandler, this });
				m_window->RemoveEventHandler(WindowEvent::MouseMove, { MouseMoveHandler, this });
			}
		}

		const MouseInput& operator=(const MouseInput& other)
		{
			SetWindow(other.m_window);
			return *this;
		}

		// Returns the old window
		inline Ref<Window> SetWindow(Ref<Window> window)
		{
			// Reset the keys
			m_buttons.reset();
			// Remove the event handlers from the old window
			m_window->RemoveEventHandler(WindowEvent::MouseButtonDown, { ButtonDownHandler, this });
			m_window->RemoveEventHandler(WindowEvent::MouseButtonUp, { ButtonUpHandler, this });
			m_window->PushEventHandler(WindowEvent::MouseMove, { MouseMoveHandler, this });
			// Save the window so it can be returned
			auto oldWindow = m_window;
			// Setup the new window
			m_window = window;
			m_window->PushEventHandler(WindowEvent::MouseButtonDown, { ButtonDownHandler, this });
			m_window->PushEventHandler(WindowEvent::MouseButtonUp, { ButtonUpHandler, this });
			m_window->RemoveEventHandler(WindowEvent::MouseMove, { MouseMoveHandler, this });
			// Return the old window
			return oldWindow;
		}

		inline bool IsButtonPressed(MouseButton button)
		{
			if (button == MouseButton::Invalid)
				return false;
			return m_buttons[static_cast<size_t>(button) - 1];
		}
		inline int32_t GetX() { return m_posX; }
		inline int32_t GetY() { return m_posY; }

		inline Ref<Window> GetWindow() { return m_window; }
	private:
		inline static void ButtonDownHandler(const WindowEvent& event, void* data)
		{
			auto& mouseInput = *reinterpret_cast<MouseInput*>(data);
			auto eventInfo = event.GetWindowEventInfo<WindowMouseButtonDownEventInfo>();
			mouseInput.m_buttons[static_cast<size_t>(eventInfo.button) - 1] = true;
			mouseInput.m_posX = eventInfo.x;
			mouseInput.m_posY = eventInfo.y;
		}
		inline static void ButtonUpHandler(const WindowEvent& event, void* data)
		{
			auto& mouseInput = *reinterpret_cast<MouseInput*>(data);
			auto eventInfo = event.GetWindowEventInfo<WindowMouseButtonUpEventInfo>();
			mouseInput.m_buttons[static_cast<size_t>(eventInfo.button) - 1] = false;
			mouseInput.m_posX = eventInfo.x;
			mouseInput.m_posY = eventInfo.y;
		}
		inline static void MouseMoveHandler(const WindowEvent& event, void* data)
		{
			auto& mouseInput = *reinterpret_cast<MouseInput*>(data);
			auto eventInfo = event.GetWindowEventInfo<WindowMouseMoveEventInfo>();
			mouseInput.m_posX = eventInfo.x;
			mouseInput.m_posY = eventInfo.y;
		}

		Ref<Window> m_window;
		std::bitset<5> m_buttons;
		int32_t m_posX, m_posY;
	};
}

#endif // BLAZE_MOUSEINPUT_H
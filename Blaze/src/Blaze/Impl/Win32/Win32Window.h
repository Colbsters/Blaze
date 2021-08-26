#pragma once

#ifndef BLAZE_WIN32_WIN32WINDOW_H
#define BLAZE_WIN32_WIN32WINDOW_H

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

#include <Blaze/Core.h>
#include <Blaze/Error.h>
#include <Blaze/Window.h>
#include <Blaze/InputBase.h>

#include <vector>
#include <array>
#include <mutex>

namespace Blaze
{
	namespace Win32
	{
		constexpr size_t numWindowEventCodes = 8;

		class Win32Window
			:public Window
		{
		public:
			inline Win32Window() { classID = GetStaticClassID(); }

			virtual Result Create_Impl(const ObjectCreateInfo& createInfo) override;
			virtual Result Destroy_Impl() override;

			constexpr static ClassID GetStaticClassID() { return Details::MakeClassID(Details::InterfaceID::Window, Details::ImplementationID::Win32); }

			virtual Result Update_Impl() override;
			virtual bool IsRunning_Impl() override;

			virtual Ref<Object> CastTo_Impl(ClassID objectID) override;

			virtual Result PushEventHandler_Impl(WindowEvent::EventCode eventCode, WindowEventHandler eventHandler) override;
			virtual Result RemoveEventHandler_Impl(WindowEvent::EventCode eventCode, WindowEventHandler eventHandler) override;

			virtual Result SetTitle_Impl(std::string_view newTitle) override;
			virtual std::string GetTitle_Impl() override;

			virtual Result Resize_Impl(uint32_t width, uint32_t height) override;
			virtual std::array<uint32_t, 2> GetWindowSize_Impl() override;
			virtual std::array<uint32_t, 2> GetClientSize_Impl() override;

			virtual Result Move_Impl(int32_t x, int32_t y) override;
			virtual std::array<int32_t, 2> GetPosition_Impl() override;

			virtual Result SetShowState_Impl(WindowShowState showState) override;
			virtual WindowShowState GetShowState_Impl() override;

			inline virtual WindowAPI GetWindowAPI_Impl() override { return WindowAPI::Win32; }

			inline HWND GetHwnd() { return m_hWnd; }
		private:
			static LRESULT __stdcall WndProc(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam);
			static WindowEvent TranslateWindowEvent(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam);
			static KeyCode TranslateKeycode(WPARAM wparam, LPARAM lparam);

			static ATOM s_windowClassAtom;
			static HINSTANCE s_hInstance;
			static std::mutex s_windowClassInfoMutex;
			
			// One vector for each event, extra one for all events
			std::array<std::vector<WindowEventHandler>, WindowEvent::NumEvents + 1> m_eventHandlers;

			HWND m_hWnd;
			bool m_isRun;
		};
	}
}

extern "C"
{
	// Allocates a win32 window, does not call create
	// This function is meant for dynamic loading, if implementations ever get split off into separate DLLs
	// This function is meant for internal use
	BLAZE_API Blaze::Win32::Win32Window* AllocateWin32Window();
}

#endif // BLAZE_WIN32_WIN32WINDOW_H
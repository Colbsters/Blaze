#include <pch.h>
#include "Win32Window.h"

namespace Blaze
{
	namespace Win32
	{
		ATOM Win32Window::s_windowClassAtom = 0;
		HINSTANCE Win32Window::s_hInstance = nullptr;
		std::mutex Win32Window::s_windowClassInfoMutex{};

		Result Win32Window::Create_Impl(const ObjectCreateInfo& createInfo)
		{
			{
				std::lock_guard<std::mutex> guard(s_windowClassInfoMutex);

				if (!s_hInstance)
					s_hInstance = GetModuleHandleW(nullptr);

				if (!s_windowClassAtom)
				{
					WNDCLASSEXW wc;
					wc.cbSize = sizeof(WNDCLASSEXW);
					wc.cbWndExtra = sizeof(Win32Window*);
					wc.cbClsExtra = 0;
					wc.hbrBackground = nullptr;
					wc.hCursor = LoadCursorW(nullptr, IDC_ARROW);
					wc.hIcon = LoadIconW(0, IDI_APPLICATION);
					wc.hIconSm = LoadIconW(0, IDI_APPLICATION);
					wc.hInstance = s_hInstance;
					wc.lpfnWndProc = WndProc;
					wc.lpszClassName = L"blaze_wndcls_1";
					wc.lpszMenuName = nullptr;
					wc.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;

					s_windowClassAtom = RegisterClassExW(&wc);

					if (!s_windowClassAtom)
						return Result::SystemError;
				}
			}

			auto info = static_cast<const WindowCreateInfo&>(createInfo);

			std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
			std::wstring wndTitleWide = converter.from_bytes(info.wndTitle);

			// NOTE: m_hWnd gets set in WndProc during WM_NCCREATE
			if (!CreateWindowExW(0,
				MAKEINTATOM(s_windowClassAtom),
				wndTitleWide.c_str(),
				WS_OVERLAPPEDWINDOW,
				info.x, info.y, info.width, info.height,
				nullptr, nullptr, s_hInstance, this))
				return Result::SystemError;

			ShowWindow(m_hWnd, SW_SHOW);

			m_isRun = true;

			return Result::Success;
		}

		Result Win32Window::Destroy_Impl()
		{
			if (!DestroyWindow(m_hWnd))
				Result::SystemError;
			return Result::Success;
		}

		Result Win32Window::Update_Impl()
		{
			MSG msg;
			while (PeekMessageW(&msg, m_hWnd, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessageW(&msg);
			}
			return Result::Success;
		}

		bool Win32Window::IsRunning_Impl()
		{
			return m_isRun;
		}

		Ref<Object> Win32Window::CastTo_Impl(ClassID objectID)
		{
			// Array of ids that can be casted to
			constexpr ClassID castableIDs[]
			{
				Object::GetStaticClassID(),
				Window::GetStaticClassID(),
				GetStaticClassID() // Include a class ID for this type as well
			};

			// If the id can be casted to, return a pointer his object
			for (auto id : castableIDs)
			{
				if (id == objectID)
					return shared_from_this();
			}
			return Ref<Object>{ nullptr };
		}

		Result Win32Window::PushEventHandler_Impl(WindowEvent::EventCode eventCode, WindowEventHandler eventHandler)
		{
			m_eventHandlers[static_cast<size_t>(eventCode) - 1].push_back(eventHandler);
			return Result::Success;
		}

		Result Win32Window::RemoveEventHandler_Impl(WindowEvent::EventCode eventCode, WindowEventHandler eventHandler)
		{
			auto& eventHandlers = m_eventHandlers[static_cast<size_t>(eventCode)];

			// Find the handler
			auto handlerLocation = std::find(eventHandlers.begin(), eventHandlers.end(), eventHandler);
			if (handlerLocation == eventHandlers.end())
				return Result::InvalidParam;

			// Erace the handler
			m_eventHandlers[static_cast<size_t>(eventCode) - 1].erase(handlerLocation);
			return Result::Success;
		}

		Result Win32Window::Resize_Impl(uint32_t width, uint32_t height)
		{
			return Result();
		}

		Result Win32Window::Move_Impl(int32_t x, int32_t y)
		{
			return Result();
		}

		LRESULT __stdcall Win32Window::WndProc(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam)
		{
			Win32Window* window;

			if (msg == WM_NCCREATE)
			{
				auto cs = reinterpret_cast<LPCREATESTRUCT>(lparam);
				// Get the
				window = static_cast<Win32Window*>(cs->lpCreateParams);
				window->m_hWnd = hWnd;
				SetWindowLongPtrW(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));
			}
			else
				window = reinterpret_cast<Win32Window*>(GetWindowLongPtrW(hWnd, GWLP_USERDATA));


			if (window)
			{
				WindowEvent event = TranslateWindowEvent(hWnd, msg, wparam, lparam);

				if (event.eventCode != WindowEvent::Invalid)
				{
					for (auto& eventHandler : window->m_eventHandlers[static_cast<size_t>(event.eventCode) - 1])
						eventHandler(event);
				}
			}

			if (msg == WM_DESTROY)
			{
				PostQuitMessage(0);
				window->m_isRun = false;
			}

			return DefWindowProcW(hWnd, msg, wparam, lparam);
		}

		WindowEvent Win32Window::TranslateWindowEvent(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam)
		{
			WindowEvent event;
			switch (msg)
			{
			case WM_CREATE:
				event.eventCode = WindowEvent::Create;
				break;
			case WM_DESTROY:
				event.eventCode = WindowEvent::Destroy;
				break;
			default:
				event.eventCode = WindowEvent::Invalid;
				break;
			}

			return event;
		}
	}
}

extern "C"
{
	Blaze::Win32::Win32Window* AllocateWin32Window()
	{
		return new Blaze::Win32::Win32Window();
	}
}

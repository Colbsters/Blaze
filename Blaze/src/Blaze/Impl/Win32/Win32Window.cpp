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
			auto info = static_cast<const WindowCreateInfo&>(createInfo);
			for (auto eventHandlers : m_eventHandlers) eventHandlers.clear();
			m_eventHandlers[0].insert(m_eventHandlers[0].begin(), info.eventHandlers.begin(), info.eventHandlers.end());

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

			std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
			std::wstring wndTitleWide = converter.from_bytes(info.wndTitle);

			RECT rect{
				static_cast<LONG>(info.x),
				static_cast<LONG>(info.y),
				static_cast<LONG>(info.x + info.width),
				static_cast<LONG>(info.y + info.height)
			};
			AdjustWindowRectEx(&rect, WS_OVERLAPPEDWINDOW, FALSE, 0);

			// NOTE: m_hWnd gets set in WndProc during WM_NCCREATE
			if (!CreateWindowExW(0,
				MAKEINTATOM(s_windowClassAtom),
				wndTitleWide.c_str(),
				WS_OVERLAPPEDWINDOW,
				rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top,
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
			for (auto eventHandlers : m_eventHandlers)
				eventHandlers.clear();
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
			m_eventHandlers[static_cast<size_t>(eventCode)].push_back(eventHandler);
			return Result::Success;
		}

		Result Win32Window::RemoveEventHandler_Impl(WindowEvent::EventCode eventCode, WindowEventHandler eventHandler)
		{
			auto eventHandlers = m_eventHandlers[static_cast<size_t>(eventCode)];

			// Find the handler
			auto handlerLocation = std::find(eventHandlers.begin(), eventHandlers.end(), eventHandler);
			if (handlerLocation == eventHandlers.end())
				return Result::InvalidParam;

			// Erase the handler
			eventHandlers.erase(handlerLocation);
			return Result::Success;
		}

		Result Win32Window::SetTitle_Impl(std::string_view newTitle)
		{
			std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
			std::wstring titleWide = converter.from_bytes(&(*newTitle.begin()), &(*newTitle.end()));
			SetWindowTextW(m_hWnd, titleWide.data());
			return Result();
		}

		std::string Win32Window::GetTitle_Impl()
		{
			// Get the length of he title
			SetLastError(0);
			size_t titleLength = GetWindowTextLengthW(m_hWnd);
			if ((!titleLength) && GetLastError())
				return { };
			// Get the title
			std::vector<wchar_t> titleWide(titleLength + 1);
			GetWindowTextW(m_hWnd, titleWide.data(), static_cast<int>(titleLength + 1));
			// Convert to UTF-8
			std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
			return converter.to_bytes(titleWide.data());
		}

		Result Win32Window::Resize_Impl(uint32_t width, uint32_t height)
		{
			if (!SetWindowPos(m_hWnd, nullptr, 0, 0, width, height, SWP_NOMOVE))
				return Result::SystemError;
			return Result::Success;
		}

		std::array<uint32_t, 2> Win32Window::GetWindowSize_Impl()
		{
			RECT rect;
			GetWindowRect(m_hWnd, &rect);
			return { static_cast<uint32_t>(rect.right - rect.left), static_cast<uint32_t>(rect.bottom - rect.top) };
		}

		std::array<uint32_t, 2> Win32Window::GetClientSize_Impl()
		{
			RECT rect;
			GetClientRect(m_hWnd, &rect);
			return { static_cast<uint32_t>(rect.bottom), static_cast<uint32_t>(rect.right) };
		}

		Result Win32Window::Move_Impl(int32_t x, int32_t y)
		{
			if (!SetWindowPos(m_hWnd, nullptr, x, y, 0, 0, SWP_NOSIZE))
				return Result::SystemError;
			return Result::Success;
		}

		std::array<int32_t, 2> Win32Window::GetPosition_Impl()
		{
			RECT rect;
			GetWindowRect(m_hWnd, &rect);
			return { static_cast<int32_t>(rect.left), static_cast<int32_t>(rect.top) };
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
					// General event handlers
					for (auto& eventHandler : window->m_eventHandlers[0])
						eventHandler.eventHandler(event, eventHandler.data);
					// Specific event handlers
					for (auto& eventHandler : window->m_eventHandlers[static_cast<size_t>(event.eventCode)])
						eventHandler.eventHandler(event, eventHandler.data);

					switch (msg)
					{
					case WM_XBUTTONDOWN:
					case WM_XBUTTONUP:
					case WM_XBUTTONDBLCLK:
						return TRUE;
					default:
						break;
					}
				}

				if (msg == WM_DESTROY)
				{
					PostQuitMessage(0);
					window->m_isRun = false;
				}
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
			case WM_SIZE:
			{
				event.eventCode = WindowEvent::Resize;
				WindowResizeEventInfo info;
				info.width = LOWORD(lparam);
				info.height = HIWORD(lparam);
				std::memcpy(event.reserved, &info, std::min(sizeof(info), sizeof(event.reserved)));
				break;
			}
			case WM_MOVE:
			{
				event.eventCode = WindowEvent::Move;
				WindowMoveEventInfo info;
				info.x = LOWORD(lparam);
				info.y = HIWORD(lparam);
				std::memcpy(event.reserved, &info, std::min(sizeof(info), sizeof(event.reserved)));
				break;
			}
			case WM_KEYDOWN:
			{
				event.eventCode = WindowEvent::KeyDown;
				WindowKeyDownEventInfo info;
				info.key = TranslateKeycode(wparam, lparam);
				std::memcpy(event.reserved, &info, std::min(sizeof(info), sizeof(event.reserved)));
				break;
			}
			case WM_KEYUP:
			{
				event.eventCode = WindowEvent::KeyUp;
				WindowKeyUpEventInfo info;
				info.key = TranslateKeycode(wparam, lparam);
				std::memcpy(event.reserved, &info, std::min(sizeof(info), sizeof(event.reserved)));
				break;
			}
			case WM_LBUTTONDOWN:
			case WM_RBUTTONDOWN:
			case WM_MBUTTONDOWN:
			case WM_XBUTTONDOWN:
			{
				event.eventCode = WindowEvent::MouseButtonDown;
				WindowMouseButtonDownEventInfo info;
				// Get the button
				switch (msg)
				{
				case WM_LBUTTONDOWN: info.button = MouseButton::Left; break;
				case WM_RBUTTONDOWN: info.button = MouseButton::Right; break;
				case WM_MBUTTONDOWN: info.button = MouseButton::Middle; break;
				case WM_XBUTTONDOWN: info.button = (HIWORD(wparam) == XBUTTON1) ? MouseButton::XButton1 : MouseButton::XButton2;
				}
				info.x = GET_X_LPARAM(lparam);
				info.y = GET_Y_LPARAM(lparam);
				std::memcpy(event.reserved, &info, std::min(sizeof(info), sizeof(event.reserved)));
				break;
			}
			case WM_LBUTTONUP:
			case WM_RBUTTONUP:
			case WM_MBUTTONUP:
			case WM_XBUTTONUP:
			{
				event.eventCode = WindowEvent::MouseButtonUp;
				WindowMouseButtonUpEventInfo info;
				// Get the button
				switch (msg)
				{
				case WM_LBUTTONUP: info.button = MouseButton::Left; break;
				case WM_RBUTTONUP: info.button = MouseButton::Right; break;
				case WM_MBUTTONUP: info.button = MouseButton::Middle; break;
				case WM_XBUTTONUP: info.button = (HIWORD(wparam) == XBUTTON1) ? MouseButton::XButton1 : MouseButton::XButton2;
				}
				info.x = GET_X_LPARAM(lparam);
				info.y = GET_Y_LPARAM(lparam);
				std::memcpy(event.reserved, &info, std::min(sizeof(info), sizeof(event.reserved)));
				break;
			}
			case WM_MOUSEMOVE:
			{
				event.eventCode = WindowEvent::MouseMove;
				WindowMouseMoveEventInfo info;
				info.x = GET_X_LPARAM(lparam);
				info.y = GET_Y_LPARAM(lparam);
				std::memcpy(event.reserved, &info, std::min(sizeof(info), sizeof(event.reserved)));
				break;
			}
			default:
				event.eventCode = WindowEvent::Invalid;
				break;
			}

			return event;
		}

		KeyCode Win32Window::TranslateKeycode(WPARAM wparam, LPARAM lparam)
		{
			constexpr std::array<KeyCode, 187> wparamToKeyCodeTable = 
			{
				KeyCode::Invalid,		// VK_LBUTTON
				KeyCode::Invalid,		// VK_RBUTTON
				KeyCode::Invalid,		// VK_CANCEL
				KeyCode::Invalid,		// VK_MBUTTON
				KeyCode::Invalid,		// VK_XBUTTON1
				KeyCode::Invalid,		// VK_XBUTTON2
				KeyCode::Invalid,		// 0x07 Undefined
				KeyCode::Backspace,		// VK_BACK
				KeyCode::Tab,			// VK_TAB
				KeyCode::Invalid,		// 0x0A-0B Reserved
				KeyCode::Invalid,
				KeyCode::Clear,			// VK_CLEAR
				KeyCode::Enter,			// VK_RETURN
				KeyCode::Invalid,		// 0x0E-0F Undefined
				KeyCode::Invalid,
				static_cast<KeyCode>(0xff),	// VK_SHIFT, set to 0xff as further processing is need for left/right
				static_cast<KeyCode>(0xff),	// VK_CONTROL, set to 0xff as further processing is need for left/right
				static_cast<KeyCode>(0xff),	// VK_MENU, set to 0xff as further processing is need for left/right
				KeyCode::Pause,			// VK_PAUSE
				KeyCode::CapsLock,		// VK_CAPITAL
				KeyCode::Invalid,		// VK_KANA, VK_HANGUEL, VK_HANGUL
				KeyCode::Invalid,		// VK_IME_ON
				KeyCode::Invalid,		// VK_JUNJA
				KeyCode::Invalid,		// VK_FINAL
				KeyCode::Invalid,		// VK_HANJA, VK_KANJI
				KeyCode::Invalid,		// VK_IME_OFF
				KeyCode::Escape,		// VK_ESCAPE
				KeyCode::Invalid,		// VK_CONVERT
				KeyCode::Invalid,		// VK_NONCONVERT
				KeyCode::Invalid,		// VK_ACCEPT
				KeyCode::Invalid,		// VK_MODECHANGE
				KeyCode::SpaceBar,		// VK_SPACE
				KeyCode::PageUp,		// VK_PRIOR
				KeyCode::PageDown,		// VK_NEXT
				KeyCode::End,			// VK_END
				KeyCode::Home,			// VK_HOME
				KeyCode::LeftArrow,		// VK_LEFT
				KeyCode::UpArrow,		// VK_UP
				KeyCode::RightArrow,	// VK_RIGHT
				KeyCode::DownArrow,		// VK_DOWN
				KeyCode::Invalid,		// VK_SELECT
				KeyCode::Print,			// VK_PRINT
				KeyCode::Invalid,		// VK_EXECUTE
				KeyCode::PrintScreen,	// VK_SNAPSHOT
				KeyCode::Insert,		// VK_INSERT
				KeyCode::Delete,		// VK_DELETE
				KeyCode::Help,			// VK_HELP
				KeyCode::Keyboard0,		// 0-9
				KeyCode::Keyboard1,
				KeyCode::Keyboard2,
				KeyCode::Keyboard3,
				KeyCode::Keyboard4,
				KeyCode::Keyboard5,
				KeyCode::Keyboard6,
				KeyCode::Keyboard7,
				KeyCode::Keyboard8,
				KeyCode::Keyboard9,
				KeyCode::Invalid,		// 0x3A-40 Undefined
				KeyCode::Invalid,
				KeyCode::Invalid,
				KeyCode::Invalid,
				KeyCode::Invalid,
				KeyCode::Invalid,
				KeyCode::Invalid,
				KeyCode::A,				// A-Z
				KeyCode::B,
				KeyCode::C,
				KeyCode::D,
				KeyCode::E,
				KeyCode::F,
				KeyCode::G,
				KeyCode::H,
				KeyCode::I,
				KeyCode::J,
				KeyCode::K,
				KeyCode::L,
				KeyCode::M,
				KeyCode::N,
				KeyCode::O,
				KeyCode::P,
				KeyCode::Q,
				KeyCode::R,
				KeyCode::S,
				KeyCode::T,
				KeyCode::U,
				KeyCode::V,
				KeyCode::W,
				KeyCode::X,
				KeyCode::Y,
				KeyCode::Z,
				KeyCode::LeftWindows,	// VK_LWIN
				KeyCode::RightWindows,	// VK_RWIN
				KeyCode::Invalid,		// VK_APPS
				KeyCode::Invalid,		// 0x5E Reserved
				KeyCode::Invalid,		// VK_SLEEP
				KeyCode::Numpad0,		// VK_NUMPAD0-9
				KeyCode::Numpad1,
				KeyCode::Numpad2,
				KeyCode::Numpad3,
				KeyCode::Numpad4,
				KeyCode::Numpad5,
				KeyCode::Numpad6,
				KeyCode::Numpad7,
				KeyCode::Numpad8,
				KeyCode::Numpad9,
				KeyCode::NumpadMultiply,	// VK_MULTIPLY
				KeyCode::NumpadPlus,		// VK_ADD
				KeyCode::Invalid,			// VK_SEPARATOR
				KeyCode::NumpadMinus,		// VK_SUBTRACT
				KeyCode::NumpadDecimalPoint,// VK_DECIMAL
				KeyCode::NumpadDivide,		// VK_DIVIDE
				KeyCode::F1,		// VK_F1
				KeyCode::F2,		// VK_F2
				KeyCode::F3,		// VK_F3
				KeyCode::F4,		// VK_F4
				KeyCode::F5,		// VK_F5
				KeyCode::F6,		// VK_F6
				KeyCode::F7,		// VK_F7
				KeyCode::F8,		// VK_F8
				KeyCode::F9,		// VK_F9
				KeyCode::F10,		// VK_F10
				KeyCode::F11,		// VK_F11
				KeyCode::F12,		// VK_F12
				KeyCode::F13,		// VK_F13
				KeyCode::F14,		// VK_F14
				KeyCode::F15,		// VK_F15
				KeyCode::F16,		// VK_F16
				KeyCode::F17,		// VK_F17
				KeyCode::F18,		// VK_F18
				KeyCode::F19,		// VK_F19
				KeyCode::F20,		// VK_F20
				KeyCode::F21,		// VK_F21
				KeyCode::F22,		// VK_F22
				KeyCode::F23,		// VK_F23
				KeyCode::F24,		// VK_F24
				KeyCode::Invalid,		// 0x88-8F Unassigned
				KeyCode::Invalid,
				KeyCode::Invalid,
				KeyCode::Invalid,
				KeyCode::Invalid,
				KeyCode::Invalid,
				KeyCode::Invalid,
				KeyCode::Invalid,
				KeyCode::NumberLock,	// VK_NUMLOCK
				KeyCode::ScollLock,		// VK_SCROLL
				// [145-157] (0x92-9D)
				//KeyCode::Invalid,		// 0x92-96 OEM specific
				//KeyCode::Invalid,
				//KeyCode::Invalid,
				//KeyCode::Invalid,
				//KeyCode::Invalid,
				//KeyCode::Invalid,		// 0x97-9F Unassigned
				//KeyCode::Invalid,
				//KeyCode::Invalid,
				//KeyCode::Invalid,
				//KeyCode::Invalid,
				//KeyCode::Invalid,
				//KeyCode::Invalid,
				//KeyCode::Invalid,
				KeyCode::LeftShift,			// VK_LSHIFT
				KeyCode::RightShift,		// VK_RSHIFT
				KeyCode::LeftControl,		// VK_LCONTROL
				KeyCode::RightControl,		// VK_RCONTROL
				KeyCode::LeftAlt,			// VK_LMENU
				KeyCode::RightAlt,			// VK_RMENU
				KeyCode::BrowserBack,		// VK_BROWSER_BACK
				KeyCode::BrowserForward,	// VK_BROWSER_FORWARD
				KeyCode::BrowserRefresh,	// VK_BROWSER_REFRESH
				KeyCode::BrowserStop,		// VK_BROWSER_STOP
				KeyCode::BrowserSearch,		// VK_BROWSER_SEARCH
				KeyCode::BrowserFavorites,	// VK_BROWSER_FAVORITES
				KeyCode::BrowserHome,		// VK_BROWSER_HOME
				KeyCode::Invalid,		// VK_VOLUME_MUTE
				KeyCode::Invalid,		// VK_VOLUME_DOWN
				KeyCode::Invalid,		// VK_VOLUME_UP
				KeyCode::MediaNextTrack,		// VK_MEDIA_NEXT_TRACK
				KeyCode::MediaPreviousTrack,	// VK_MEDIA_PREV_TRACK
				KeyCode::MediaStop,				// VK_MEDIA_STOP
				KeyCode::MediaPlayPause,		// VK_MEDIA_PLAY_PAUSE
				KeyCode::Invalid,		// VK_LAUNCH_MAIL
				KeyCode::Invalid,		// VK_LAUNCH_MEDIA_SELECT
				KeyCode::Invalid,		// VK_LAUNCH_APP1
				KeyCode::Invalid,		// VK_LAUNCH_APP2
				KeyCode::Invalid,		// 0xB8-B9 Reserved
				KeyCode::Invalid,
				KeyCode::Semicolon,		// VK_OEM_1
				KeyCode::Plus,			// VK_OEM_PLUS
				KeyCode::Comma,			// VK_OEM_COMMA
				KeyCode::Minus,			// VK_OEM_MINUS
				KeyCode::Period,		// VK_OEM_PERIOD
				KeyCode::Slash,			// VK_OEM_2
				KeyCode::Backtick,		// VK_OEM_3
				// [191-216] (0xC0-D9)
				//KeyCode::Invalid,		// 0xC1-D7 Reserved
				//KeyCode::Invalid,
				//KeyCode::Invalid,
				//KeyCode::Invalid,
				//KeyCode::Invalid,
				//KeyCode::Invalid,
				//KeyCode::Invalid,
				//KeyCode::Invalid,
				//KeyCode::Invalid,
				//KeyCode::Invalid,
				//KeyCode::Invalid,
				//KeyCode::Invalid,
				//KeyCode::Invalid,
				//KeyCode::Invalid,
				//KeyCode::Invalid,
				//KeyCode::Invalid,
				//KeyCode::Invalid,
				//KeyCode::Invalid,
				//KeyCode::Invalid,
				//KeyCode::Invalid,
				//KeyCode::Invalid,
				//KeyCode::Invalid,
				//KeyCode::Invalid,
				//KeyCode::Invalid,		// 0xD8-DA Unassigned
				//KeyCode::Invalid,
				//KeyCode::Invalid,
				KeyCode::OpenBracket,	// VK_OEM_4
				KeyCode::BackSlash,		// VK_OEM_5
				KeyCode::CloseBracket,	// VK_OEM_6
				KeyCode::Quote,			// VK_OEM_7
				// [221-247] (0xDE-F8)
				//KeyCode::Invalid,		// VK_OEM_8
				//KeyCode::Invalid,		// 0xE0 Reserved
				//KeyCode::Invalid,		// 0xE1 OEM specific
				//KeyCode::Invalid,		// VK_OEM_102
				//KeyCode::Invalid,		// 0xE3-E4 OEM specific
				//KeyCode::Invalid,
				//KeyCode::Invalid,		// VK_PROCESSKEY
				//KeyCode::Invalid,		// 0xE6 OEM specific
				//KeyCode::Invalid,		// VK_PACKET
				//KeyCode::Invalid,		// 0xE8 Unassigned
				//KeyCode::Invalid,		// 0xE9-F5 OEM specific
				//KeyCode::Invalid,
				//KeyCode::Invalid,
				//KeyCode::Invalid,
				//KeyCode::Invalid,
				//KeyCode::Invalid,
				//KeyCode::Invalid,
				//KeyCode::Invalid,
				//KeyCode::Invalid,
				//KeyCode::Invalid,
				//KeyCode::Invalid,
				//KeyCode::Invalid,
				//KeyCode::Invalid,
				//KeyCode::Invalid,		// VK_ATTN
				//KeyCode::Invalid,		// VK_CRSEL
				//KeyCode::Invalid,		// VK_EXSEL
				//KeyCode::Invalid,		// VK_EREOF
				KeyCode::Play,			// VK_PLAY
				KeyCode::Zoom,			// VK_ZOOM
				KeyCode::Invalid,		// VK_NONAME
				KeyCode::Invalid,		// VK_PA1
				KeyCode::Clear			// VK_OEM_CLEAR
			};

			// Get the index
			size_t index = wparam;
			// Transform the index to span the large KeyCode::Invalid gaps
			if (index >= 0xDE)
			{
				if (index <= 0xF8)
					return KeyCode::Invalid;
				index -= (27 + 26 + 13);
			}
			else if (index >= 0xC0)
			{
				if (index <= 0xD9)
					return KeyCode::Invalid;
				index -= (26 + 13);
			}
			else if (index >= 0x92)
			{
				if (wparam <= 0x9D)
					return KeyCode::Invalid;
				index -= (13);
			}

			// Get left/right keycodes
			if (wparamToKeyCodeTable[index - 1] == static_cast<KeyCode>(0xff))
			{
				UINT scancode = (lparam & 0x00ff0000) >> 16;
				int extended = (lparam & 0x01000000) != 0;

				switch (wparam)
				{
				case VK_SHIFT:
					wparam = MapVirtualKeyW(scancode, MAPVK_VSC_TO_VK_EX);
					break;
				case VK_CONTROL:
					wparam = extended ? VK_LCONTROL : VK_RCONTROL;
					break;
				case VK_MENU:
					wparam = extended ? VK_LMENU : VK_RMENU;
					break;
				}

				// Retransform the index to span the large KeyCode::Invalid gaps
				index = wparam;
				if (index >= 0xDE)
				{
					if (index <= 0xF8)
						return KeyCode::Invalid;
					index -= (27 + 26 + 13);
				}
				else if (index >= 0xC0)
				{
					if (index <= 0xD9)
						return KeyCode::Invalid;
					index -= (26 + 13);
				}
				else if (index >= 0x92)
				{
					if (wparam <= 0x9D)
						return KeyCode::Invalid;
					index -= (13);
				}
			}

			return wparamToKeyCodeTable[index - 1];
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

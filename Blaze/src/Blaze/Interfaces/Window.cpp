#include <pch.h>
#include <Blaze/Window.h>
#include <Blaze/Impl/Win32/Win32Window.h>

namespace Blaze
{
	namespace Details
	{
#if defined(BLAZE_PLATFORM_WIN32) || defined(BLAZE_PLATFORM_WIN64)
		constexpr WindowAPI windowAPI = WindowAPI::Win32;
#else // ^^^ Windows (Win32) / Invalid platform vvv
		constexpr WindowAPI windowAPI = WindowAPI::Invalid;
#endif // ^^^ Invalid platform
	}

	Ref<Window> Window::Create(const WindowCreateInfo& createInfo)
	{
		Ref<Window> ptr;

		switch (Details::windowAPI)
		{
		case WindowAPI::Win32:
			ptr = Ref<Window>{ AllocateWin32Window() };
			break;
			// TODO: Add cases for other platforms
		default:
			return Ref<Window>{ nullptr };
		}

		ptr->Object::Create(static_cast<const ObjectCreateInfo&>(createInfo));

		return ptr;
	}
}

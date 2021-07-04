#include <pch.h>
#include <Blaze/Window.h>

namespace Blaze
{
	namespace Details
	{
		constexpr WindowAPI windowAPI = WindowAPI::Invalid;
	}

	Ref<Window> Window::Create(const WindowCreateInfo& createInfo)
	{
		Ref<Window> ptr;

		switch (Details::windowAPI)
		{
			// TODO: Add cases for other platforms
		default:
			return Ref<Window>{ nullptr };
		}

		ptr->Object::Create(static_cast<const ObjectCreateInfo&>(createInfo));

		return ptr;
	}
}

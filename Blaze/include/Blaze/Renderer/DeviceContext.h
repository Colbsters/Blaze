#pragma once

#ifndef BLAZE_DEVICECONTEXT_H
#define BLAZE_DEVICECONTEXT_H

#include <Blaze/Core.h>
#include <Blaze/Object.h>
#include <Blaze/Window.h>

namespace Blaze
{
	enum class RenderAPI
	{
		Null = 0,
		Invalid = Null,
		OpenGL
	};

	struct DeviceContextCreateInfo
		:public ObjectCreateInfo
	{
		Ref<Window> window;
		// Set to RenderAPI::Null for default api
		RenderAPI renderingApi = RenderAPI::Null;
	};

	class BLAZE_API DeviceContext
		:public Object
	{
	public:
		inline DeviceContext() { classID = GetStaticClassID(); }
		virtual ~DeviceContext() = default;

		static Ref<DeviceContext> Create(const DeviceContextCreateInfo& createInfo);

		static constexpr ClassID GetStaticClassID() { return Details::MakeClassID(Details::InterfaceID::DeviceContext, Details::ImplementationID::Invalid); }

		inline Result SwapBuffers() { return SwapBuffers_Impl(); }

		inline RenderAPI GetRenderAPI() { return GetRenderAPI_Impl(); }
	private:
		virtual Result SwapBuffers_Impl() = 0;
		virtual RenderAPI GetRenderAPI_Impl() = 0;
	};
}

#endif // BLAZE_DEVICECONTEXT_H
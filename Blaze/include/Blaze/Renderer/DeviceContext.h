#pragma once

#ifndef BLAZE_DeviceContext_H
#define BLAZE_DeviceContext_H

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

		static Ref<DeviceContext> Create(const DeviceContextCreateInfo& createInfo);

		static constexpr ClassID GetStaticClassID() { return Details::MakeClassID(Details::InterfaceID::DeviceContext, Details::ImplementationID::Invalid); }

		// Makes the this context current
		inline Result MakeCurrent() { return MakeCurrent_Impl(); }
		// Makes this context obselete if it is current
		inline Result MakeObsolete() { return MakeObsolete_Impl(); }
		// Checks if this context is current
		inline bool IsCurrent() { return IsCurrent_Impl(); }

		inline RenderAPI GetRenderAPI() { return GetRenderAPI_Impl(); }

	private:
		virtual Result MakeCurrent_Impl() = 0;
		virtual Result MakeObsolete_Impl() = 0;
		virtual bool IsCurrent_Impl() = 0;
		virtual RenderAPI GetRenderAPI_Impl() = 0;
	};
}

#endif // BLAZE_DeviceContext_H
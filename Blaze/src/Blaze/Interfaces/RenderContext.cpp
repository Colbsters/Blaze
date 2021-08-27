#include <pch.h>
#include <Blaze/Renderer/RenderContext.h>
#include <Blaze/Impl/OpenGL/GLRenderContext.h>

namespace Blaze
{
	namespace Details
	{
#ifdef BLAZE_PLATFORM_WIN64
		constexpr std::array<RenderAPI, 1> renderAPIs{ RenderAPI::OpenGL };
#else
		constexpr std::array<RenderAPI, 0> renderAPIs {};
#endif
	}

	Ref<RenderContext> RenderContext::Create(const RenderContextCreateInfo& createInfo)
	{
		Ref<RenderContext> ptr;
		RenderContextCreateInfo info = createInfo;

		// Use default API if one isn't specified
		if (createInfo.renderingApi == RenderAPI::Null)
			info.renderingApi = Details::renderAPIs[0];

		// Make sure the API is valid
		if (std::find(Details::renderAPIs.begin(), Details::renderAPIs.end(), info.renderingApi) == Details::renderAPIs.end())
			return Ref<RenderContext>{ nullptr };

		switch (info.renderingApi)
		{
		case RenderAPI::OpenGL:
			ptr = Ref<RenderContext>{ AllocateOpenGLRenderContext(info.window->GetWindowAPI()) };
			break;
		default:
			return Ref<RenderContext>{ nullptr };
		}

		ptr->Object::Create(static_cast<const ObjectCreateInfo&>(info));

		return ptr;
	}
}

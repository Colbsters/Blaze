#include <pch.h>
#include <Blaze/Renderer/RenderContext.h>

namespace Blaze
{
	namespace Details
	{
		constexpr std::array<RenderAPI, 1> renderAPIs = { RenderAPI::Invalid };
	}

	Ref<RenderContext> RenderContext::Create(const RenderContextCreateInfo& createInfo)
	{
		Ref<RenderContext> ptr;

		if (std::find(Details::renderAPIs.begin(), Details::renderAPIs.end(), createInfo.renderingApi) == Details::renderAPIs.end())
			return Ref<RenderContext>{ nullptr };

		switch (createInfo.renderingApi)
		{
		default:
			return Ref<RenderContext>{ nullptr };
		}

		ptr->Object::Create(static_cast<const ObjectCreateInfo&>(createInfo));
	}
}

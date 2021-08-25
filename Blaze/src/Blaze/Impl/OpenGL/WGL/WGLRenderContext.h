#pragma once

#ifndef BLAZE_OPENGL_WGLRENDERCONTEXT_H
#define BLAZE_OPENGL_WGLRENDERCONTEXT_H

#include <Blaze/Core.h>
#include <Blaze/Error.h>
#include <Blaze/Renderer/RenderContext.h>
#include <Blaze/Impl/OpenGL/GLRenderContext.h>
#include <Blaze/Window.h>

#include <utility>
#include <unordered_map>
#include <thread>
#include <mutex>

namespace Blaze
{
	namespace OpenGL
	{
		Result InitializeWGL();

		class WGLRenderContext
			:public GLRenderContext
		{
		public:
			WGLRenderContext() { classID = GetStaticClassID(); }

			static constexpr ClassID GetStaticClassID() { return Details::MakeClassID(Details::InterfaceID::RenderContext, Details::ImplementationID::WGL); }

			virtual Result Create_Impl(const ObjectCreateInfo& createInfo) override;

			virtual Result Destroy_Impl() override;

			virtual Ref<Object> CastTo_Impl(ClassID objectID) override;

			virtual Result MakeCurrent_Impl() override;

			virtual Result MakeObsolete_Impl() override;

			virtual bool IsCurrent_Impl() override;
		private:
			static std::unordered_map<std::thread::id, WGLRenderContext*> m_currentContexts;

			HDC m_hdc;
			HGLRC m_hglrc;
		};
	}
}

#endif // BLAZE_OPENGL_WGLRENDERCONTEXT_H
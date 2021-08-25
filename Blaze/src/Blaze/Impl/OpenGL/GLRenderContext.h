#pragma once

#ifndef BLAZE_OPENGL_GLRENDERCONTEXT_H
#define BLAZE_OPENGL_GLRENDERCONTEXT_H

#include <Blaze/Core.h>
#include <Blaze/Error.h>
#include <Blaze/Renderer/RenderContext.h>
#include <Blaze/Window.h>

namespace Blaze
{
	namespace OpenGL
	{
		// Not a real implementation, just a base class for OpenGL render contexts (WGL, etc.)
		class GLRenderContext
			:public RenderContext
		{
		public:
			inline GLRenderContext() { classID = GetStaticClassID(); }

			static constexpr ClassID GetStaticClassID() { return Details::MakeClassID(Details::InterfaceID::RenderContext, Details::ImplementationID::OpenGL); }

			inline virtual RenderAPI GetRenderAPI_Impl() override { return RenderAPI::OpenGL; }
		};
	}
}

extern "C"
{
	// Allocates an OpenGL render context, does not call create
	// This function is meant for allocating sub-implementations (WGL, etc.) and dynamic loading, if implementations ever get split off into separate DLLs
	// This function is meant for internal use
	BLAZE_API Blaze::OpenGL::GLRenderContext* AllocateOpenGLRenderContext(Blaze::WindowAPI windowAPI);
}

#endif // BLAZE_OPENGL_GLRENDERCONTEXT_H
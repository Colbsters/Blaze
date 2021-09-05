#pragma once

#ifndef BLAZE_OPENGL_GLDEVICECONTEXT_H
#define BLAZE_OPENGL_GLDEVICECONTEXT_H

#include <Blaze/Core.h>
#include <Blaze/Error.h>
#include <Blaze/Renderer/DeviceContext.h>
#include <Blaze/Window.h>

namespace Blaze
{
	namespace OpenGL
	{
		// Not a real implementation, just a base class for OpenGL render contexts (WGL, etc.)
		class GLDeviceContext
			:public DeviceContext
		{
		public:
			inline GLDeviceContext() { classID = GetStaticClassID(); }

			static constexpr ClassID GetStaticClassID() { return Details::MakeClassID(Details::InterfaceID::DeviceContext, Details::ImplementationID::OpenGL); }

			inline virtual RenderAPI GetRenderAPI_Impl() override { return RenderAPI::OpenGL; }

			inline GladGLContext GetGL() { return m_gl; }
		protected:
			GladGLContext m_gl;
		};
	}
}

extern "C"
{
	// Allocates an OpenGL render context, does not call create
	// This function is meant for allocating sub-implementations (WGL, etc.) and dynamic loading, if implementations ever get split off into separate DLLs
	// This function is meant for internal use
	BLAZE_API Blaze::OpenGL::GLDeviceContext* AllocateOpenGLDeviceContext(Blaze::WindowAPI windowAPI);
}

#endif // BLAZE_OPENGL_GLDEVICECONTEXT_H
#pragma once

#ifndef BLAZE_OPENGL_WGLDEVICECONTEXT_H
#define BLAZE_OPENGL_WGLDEVICECONTEXT_H

#include <Blaze/Core.h>
#include <Blaze/Error.h>
#include <Blaze/Renderer/DeviceContext.h>
#include <Blaze/Impl/OpenGL/GLDeviceContext.h>
#include <Blaze/Window.h>
#include <Blaze/Impl/Win32/Win32Window.h>

#include <utility>
#include <unordered_map>
#include <thread>
#include <mutex>

namespace Blaze
{
	namespace OpenGL
	{
		Result InitializeWGL();

		class WGLDeviceContext
			:public GLDeviceContext
		{
		public:
			WGLDeviceContext() { classID = GetStaticClassID(); }
			~WGLDeviceContext();

			static constexpr ClassID GetStaticClassID() { return Details::MakeClassID(Details::InterfaceID::DeviceContext, Details::ImplementationID::WGL); }

			virtual Result Create_Impl(const ObjectCreateInfo& createInfo) override;

			virtual Result Destroy_Impl() override;

			virtual Result SwapBuffers_Impl() override;

			virtual Ref<Object> CastTo_Impl(ClassID objectID) override;

			virtual Result MakeCurrent_Impl() override;

			virtual Result MakeObsolete_Impl() override;

			virtual bool IsCurrent_Impl() override;
		private:
			static std::unordered_map<std::thread::id, WGLDeviceContext*> m_currentContexts;

			Ref<Win32::Win32Window> m_window;
			HDC m_hdc;
			HGLRC m_hglrc;
		};
	}
}

#endif // BLAZE_OPENGL_WGLDEVICECONTEXT_H
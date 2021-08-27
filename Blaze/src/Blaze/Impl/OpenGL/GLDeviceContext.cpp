#include <pch.h>
#include "GLDeviceContext.h"
#include <Blaze/Impl/OpenGL/WGL/WGLDeviceContext.h>

Blaze::OpenGL::GLDeviceContext* AllocateOpenGLDeviceContext(Blaze::WindowAPI windowAPI)
{
	switch (windowAPI)
	{
	case Blaze::WindowAPI::Win32:
		return new Blaze::OpenGL::WGLDeviceContext();
	}

    return nullptr;
}

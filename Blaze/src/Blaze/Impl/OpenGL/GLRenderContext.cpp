#include <pch.h>
#include "GLRenderContext.h"
#include <Blaze/Impl/OpenGL/WGL/WGLRenderContext.h>

Blaze::OpenGL::GLRenderContext* AllocateOpenGLRenderContext(Blaze::WindowAPI windowAPI)
{
	switch (windowAPI)
	{
	case Blaze::WindowAPI::Win32:
		return new Blaze::OpenGL::WGLRenderContext();
	}

    return nullptr;
}

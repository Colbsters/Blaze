#pragma once

#ifndef BLAZE_OPENGL_GLBUFFER_H
#define BLAZE_OPENGL_GLBUFFER_H

#include <Blaze/Core.h>
#include <Blaze/Error.h>
#include <Blaze/Renderer/Buffer.h>
#include <Blaze/Impl/OpenGL/GLDeviceContext.h>

namespace Blaze
{
	namespace OpenGL
	{
		class GLBuffer
			:public Buffer
		{
		public:
			GLBuffer() { classID = GetStaticClassID(); }
			~GLBuffer();

			constexpr static ClassID GetStaticClassID() { return Details::MakeClassID(Details::InterfaceID::Buffer, Details::ImplementationID::OpenGL); }

			virtual Result Create_Impl(const ObjectCreateInfo& createInfo);
			virtual Result Destroy_Impl();

			virtual Ref<Object> CastTo_Impl(ClassID objectID);
			
			virtual Result Write_Impl(const void* data, size_t sizeInBytes) override;

			virtual Result MapMemory_Impl(size_t sizeInBytes, BufferAccess access, void*& ptr) override;

			virtual Result UnmapMemory_Impl() override;
		private:

			Ref<GLDeviceContext> m_deviceContext;
			GladGLContext m_gl;
			BufferType m_type;
			unsigned int m_bufferID;
		};
	}
}

extern "C"
{
	// Allocates an OpenGL buffer, does not call create
	// This function is meant for dynamic loading, if implementations ever get split off into separate DLLs
	// This function is meant for internal use
	BLAZE_API Blaze::OpenGL::GLBuffer* AllocateOpenGLBuffer();
}

#endif // BLAZE_OPENGL_GLBUFFER_H
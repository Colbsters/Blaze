#include <pch.h>
#include "GLBuffer.h"
#include <glad/gl.h>

namespace Blaze
{
	namespace OpenGL
	{
		namespace Details
		{
			static GLenum BufferTypeToGLTarget(BufferType type)
			{
				constexpr std::array<GLenum, 2> translationTable =
				{
					GL_ARRAY_BUFFER,
					GL_ELEMENT_ARRAY_BUFFER
				};

				if ((static_cast<size_t>(type) < 1) || (static_cast<size_t>(type) >= translationTable.size()))
					return 0;

				return translationTable[static_cast<size_t>(type) - 1];
			}

			static GLenum BufferAccessToGLAccess(BufferAccess access)
			{
				constexpr std::array<GLenum, 3> translationTable =
				{
					GL_READ_ONLY,
					GL_WRITE_ONLY,
					GL_READ_WRITE
				};

				if ((static_cast<size_t>(access) < 1) || (static_cast<size_t>(access) >= translationTable.size()))
					return 0;

				return translationTable[static_cast<size_t>(access) - 1];
			}
		}

		GLBuffer::~GLBuffer()
		{
			Destroy_Impl();
		}

		Result GLBuffer::Create_Impl(const ObjectCreateInfo& createInfo)
		{
			auto info = static_cast<const BufferCreateInfo&>(createInfo);
			m_deviceContext = info.deviceContext->CastTo<GLDeviceContext>();
			m_gl = m_deviceContext->GetGL();
			m_type = info.type;

			m_deviceContext->MakeCurrent();

			m_gl.GenBuffers(1, &m_bufferID);

			if (info.data && (info.size > 0))
			{
				GLenum target = Details::BufferTypeToGLTarget(m_type);
				m_gl.BindBuffer(target, m_bufferID);
				m_gl.BufferData(target, info.size, info.data, GL_DYNAMIC_DRAW);
			}

			return Result::Success;
		}

		Result GLBuffer::Destroy_Impl()
		{
			if (m_bufferID)
			{
				m_deviceContext->MakeCurrent();

				GLenum target = Details::BufferTypeToGLTarget(m_type);
				m_gl.BindBuffer(target, m_bufferID);
				m_gl.DeleteBuffers(1, &m_bufferID);
				m_bufferID = 0;
			}
			else
				Result::Uninitialized;

			return Result::Success;
		}

		Ref<Object> GLBuffer::CastTo_Impl(ClassID objectID)
		{
			constexpr std::array<ClassID, 3> castableIDs = {
				Object::GetStaticClassID(),
				Buffer::GetStaticClassID(),
				GetStaticClassID()
			};

			// Check to make sure the class ID is valid
			if (std::find(castableIDs.begin(), castableIDs.end(), objectID) == castableIDs.end())
				return Ref<Object>{ nullptr };

			return shared_from_this();
		}

		Result GLBuffer::Write_Impl(const void* data, size_t sizeInBytes)
		{
			m_deviceContext->MakeCurrent();

			GLenum target = Details::BufferTypeToGLTarget(m_type);
			m_gl.BindBuffer(target, m_bufferID);
			m_gl.BufferData(target, sizeInBytes, data, GL_DYNAMIC_DRAW);

			return Result::Success;
		}

		Result GLBuffer::MapMemory_Impl(size_t sizeInBytes, BufferAccess access, void*& ptr)
		{
			m_deviceContext->MakeCurrent();

			GLenum target = Details::BufferTypeToGLTarget(m_type);
			GLenum glAccess = Details::BufferAccessToGLAccess(access);
			m_gl.BindBuffer(target, m_bufferID);
			ptr = m_gl.MapBuffer(target, glAccess);

			return Result::Success;
		}

		Result GLBuffer::UnmapMemory_Impl()
		{
			m_deviceContext->MakeCurrent();

			GLenum target = Details::BufferTypeToGLTarget(m_type);
			m_gl.BindBuffer(target, m_bufferID);
			m_gl.UnmapBuffer(target);

			return Result::Success;
		}
	}
}

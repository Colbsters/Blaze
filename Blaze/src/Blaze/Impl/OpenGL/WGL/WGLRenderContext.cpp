#include "pch.h"
#include "WGLRenderContext.h"

namespace Blaze
{
	namespace OpenGL
	{
		std::unordered_map<std::thread::id, WGLRenderContext*> WGLRenderContext::m_currentContexts = {};

		Result InitializeWGL()
		{
			return Result();
		}

		Result WGLRenderContext::Create_Impl(const ObjectCreateInfo& createInfo)
		{
			return Result();
		}

		Result WGLRenderContext::Destroy_Impl()
		{
			return Result();
		}

		Ref<Object> WGLRenderContext::CastTo_Impl(ClassID objectID)
		{
			constexpr std::array<ClassID, 4> castableIDs = {
				Object::GetStaticClassID(),
				RenderContext::GetStaticClassID(),
				GLRenderContext::GetStaticClassID(),
				GetStaticClassID()
			};

			// Check to make sure the class ID is valid
			if (std::find(castableIDs.begin(), castableIDs.end(), objectID) == castableIDs.end())
				return Ref<Object>{ nullptr };

			return shared_from_this();
		}

		Result WGLRenderContext::MakeCurrent_Impl()
		{
			auto threadId = std::this_thread::get_id();

			// Context cannot be made current if uninitialized
			if (!(m_hdc && m_hglrc))
				return Result::Uninitialized;
			// No need to make the context current if it already is
			if (this == m_currentContexts[threadId])
				return Result::Success;
			// Make the context current and check for error
			if (!wglMakeCurrent(m_hdc, m_hglrc))
				return Result::SystemError;

			m_currentContexts[threadId] = this;
			return Result::Success;
		}

		Result WGLRenderContext::MakeObsolete_Impl()
		{
			auto threadId = std::this_thread::get_id();

			// No need to make the context obsolete if it already is
			if (this != m_currentContexts[threadId])
				return Result::Success;
			// Make the context current and check for error
			if (!wglMakeCurrent(nullptr, nullptr))
				return Result::SystemError;

			m_currentContexts[threadId] = nullptr;
			return Result::Success;
		}

		bool WGLRenderContext::IsCurrent_Impl()
		{
			return this == m_currentContexts[std::this_thread::get_id()];
		}
	}
}

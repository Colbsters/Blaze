#include "pch.h"
#include "WGLRenderContext.h"

namespace Blaze
{
	namespace OpenGL
	{
		std::unordered_map<std::thread::id, WGLRenderContext*> WGLRenderContext::m_currentContexts = {};

		Result InitializeWGL()
		{
			static bool isWGLInitialized = false;

			// Check if wg is already loaded
			if (isWGLInitialized)
				return Result::Success;

			Result res;

			// Create dummy window
			WindowCreateInfo wndInfo;
			wndInfo.wndTitle = "Blaze WGL Dummy Window";
			wndInfo.showState = WindowShowState::Hide;

			Ref<Win32::Win32Window> dummyWindow = std::make_shared<Win32::Win32Window>();
			res = dummyWindow->Object::Create(static_cast<const ObjectCreateInfo&>(wndInfo));
			if (res != Result::Success)
				return res;

			HWND hWnd = dummyWindow->GetHwnd();
			HDC hdc = GetDC(hWnd);

			// Set a dummy pixel format
			PIXELFORMATDESCRIPTOR pfd = {};
			pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
			pfd.nVersion = 1;
			pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
			pfd.iPixelType = PFD_TYPE_RGBA;
			pfd.cColorBits = 32;
			pfd.cDepthBits = 24;
			pfd.cStencilBits = 8;
			pfd.iLayerType = PFD_MAIN_PLANE;

			int pixelFormat = ChoosePixelFormat(hdc, &pfd);
			if (!pixelFormat)
				return Result::SystemError;

			if (!SetPixelFormat(hdc, pixelFormat, &pfd))
				return Result::SystemError;

			// Create a dummy context
			HGLRC dummyContext = wglCreateContext(hdc);
			if (!dummyContext)
				return Result::SystemError;

			// Make the context current
			if (!wglMakeCurrent(hdc, dummyContext))
				return Result::SystemError;

			// Load WGL
			if (!gladLoaderLoadWGL(hdc))
				return Result::UnknownError;

			// Make the context obsolete
			if (!wglMakeCurrent(nullptr, nullptr))
				return Result::SystemError;

			// Delete the dummy context
			if (!wglDeleteContext(dummyContext))
				return Result::SystemError;

			isWGLInitialized = true;
			return Result::Success;
		}

		Result WGLRenderContext::Create_Impl(const ObjectCreateInfo& createInfo)
		{
			// Reqested OpenGL version: [0] = major, [1] = minor
			constexpr std::array<int, 2> requestedGLVersion = { 3, 3 };

			Result res;

			// Obtain informatio about the window
			const auto& info = static_cast<const RenderContextCreateInfo&>(createInfo);
			m_window = info.window->CastTo<Win32::Win32Window>();
			m_hdc = GetDC(m_window->GetHwnd());

			// Initialize WGL
			res = InitializeWGL();
			if (res != Result::Success)
				return res;

			// Attributes for pixel format creation
			constexpr std::array<int, 19> pixelFormatAttribs =
			{
				WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
				WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
				WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
				WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
				WGL_COLOR_BITS_ARB, 32,
				WGL_DEPTH_BITS_ARB, 24,
				WGL_STENCIL_BITS_ARB, 8,
				WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
				WGL_SWAP_METHOD_ARB, WGL_SWAP_EXCHANGE_ARB,
				0, // End
			};

			// Choose the pixel format
			int pixelFormat;
			UINT numFormats;
			if (!wglChoosePixelFormatARB(m_hdc, pixelFormatAttribs.data(), nullptr, 1, &pixelFormat, &numFormats))
				return Result::SystemError;

			// Describe the pixel format so it can be set
			PIXELFORMATDESCRIPTOR pfd = { sizeof(PIXELFORMATDESCRIPTOR) };
			if (!DescribePixelFormat(m_hdc, pixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pfd))
				return Result::SystemError;

			// Set the pixel format
			if (!SetPixelFormat(m_hdc, pixelFormat, &pfd))
				return Result::SystemError;

			// Attributes for context creation
			constexpr std::array<int, 7> contextAttribs =
			{
				WGL_CONTEXT_MAJOR_VERSION_ARB, requestedGLVersion[0],
				WGL_CONTEXT_MINOR_VERSION_ARB, requestedGLVersion[1],
				WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
				0
			};
			
			// Create the modern OpenGL context
			m_hglrc = wglCreateContextAttribsARB(m_hdc, nullptr, contextAttribs.data());
			if (!m_hglrc)
				return Result::SystemError;

			// Make the context current
			res = MakeCurrent_Impl();
			if (res != Result::Success)
				return res;

			// Load OpenGL using glad2
			if (!gladLoaderLoadGLContext(&m_gl))
				return Result::UnknownError;

			// Print out OpenGL information (OpenGL version + GLSL version)
			std::cout << "[Blaze:Info]: OpenGL info: \n\t OpenGL Version: " << m_gl.GetString(GL_VERSION) << "\n\t GLSL Version : " << m_gl.GetString(GL_SHADING_LANGUAGE_VERSION) << '\n';

			return Result::Success;
		}

		Result WGLRenderContext::Destroy_Impl()
		{
			Result res;

			// Make the context obsolete
			res = MakeObsolete_Impl();
			if (res != Result::Success)
				return res;

			if (!wglDeleteContext(m_hglrc))
				return Result::SystemError;

			return Result::Success;
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

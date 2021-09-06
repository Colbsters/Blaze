#include <pch.h>
#include <Blaze/Renderer/Buffer.h>
#include <Blaze/Impl/OpenGL/GLBuffer.h>

namespace Blaze
{
    Ref<Buffer> Buffer::Create(const BufferCreateInfo& createInfo)
    {
        Ref<Buffer> ptr;

        switch (createInfo.deviceContext->GetRenderAPI())
        {
        case RenderAPI::OpenGL:
            ptr = Ref<Buffer>{ AllocateOpenGLBuffer() };
            break;
        default:
            return Ref<Buffer>{ nullptr };
        }

        ptr->Object::Create(static_cast<const ObjectCreateInfo&>(createInfo));

        return ptr;
    }
}

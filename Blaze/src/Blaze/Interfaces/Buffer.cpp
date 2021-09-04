#include <pch.h>
#include <Blaze/Renderer/Buffer.h>

namespace Blaze
{
    Ref<Buffer> Buffer::Create(const BufferCreateInfo& createInfo)
    {
        Ref<Buffer> ptr;

        switch (createInfo.deviceContext->GetRenderAPI())
        {
        default:
            return Ref<Buffer>{ nullptr };
        }

        ptr->Object::Create(static_cast<const ObjectCreateInfo&>(createInfo));

        return ptr;
    }
}

#pragma once

#ifndef BLAZE_BUFFER_H
#define BLAZE_BUFFER_H

#include <Blaze/Core.h>
#include <Blaze/Object.h>
#include <Blaze/Renderer/DeviceContext.h>

namespace Blaze
{
	enum class BufferType
	{
		Null = 0,
		Invalid = Null,
		Vertex,
		Index
	};

	enum class BufferAccess
	{
		Null = 0,
		Invalid = Null,
		Read		= 0x01,
		Write		= 0x02
	};

	struct BufferCreateInfo
		:public ObjectCreateInfo
	{
		Ref<DeviceContext> deviceContext;
		BufferType type;
		void* data = nullptr;
		size_t size = 0;
	};

	class BLAZE_API Buffer
		:public Object
	{
	public:
		inline Buffer() { classID = GetStaticClassID(); }
		virtual ~Buffer() = default;

		static Ref<Buffer> Create(const BufferCreateInfo& createInfo);

		constexpr static ClassID GetStaticClassID() { return Details::MakeClassID(Details::InterfaceID::Buffer, Details::ImplementationID::Invalid); }
		
		// Writes bytes to the gpu buffer
		inline Result Write(const void* data, size_t sizeInBytes) { return Write_Impl(data, sizeInBytes); }

		// Maps the gpu memory to cpu memory, the mapped memory location is stored in ptr
		inline Result MapMemory(size_t sizeInBytes, BufferAccess access, void*& ptr) { return MapMemory_Impl(sizeInBytes, access, ptr); }
		// Unmaps and deallocates the memory allocated by MapMemory, the data also gets sent to the gpu
		inline Result UnmapMemory() { return UnmapMemory_Impl(); }

	private:
		virtual Result Write_Impl(const void* data, size_t sizeInBytes) = 0;
		virtual Result MapMemory_Impl(size_t sizeInBytes, BufferAccess access, void*& ptr) = 0;
		virtual Result UnmapMemory_Impl() = 0;
	};
}

#endif // BLAZE_BUFFER_H
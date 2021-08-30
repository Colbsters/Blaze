#pragma once

#ifndef BLAZE_FORMAT_H
#define BLAZE_FORMAT_H

#include <Blaze/Core.h>

#include <cmath>

namespace Blaze
{
	enum class FormatType : uint8_t
	{
		Null = 0,
		Invalid = Null,
		Float,
		Int,
		UInt,
		Signed = Int,
		Unsigned = UInt
	};

	enum class Format
	{
		Null = 0,
		Invalid = Null,

#pragma region Signed integer formats

		R8_Int,
		R8G8_Int,
		R8G8B8_Int,
		R8G8B8A8_Int,
		R16_Int,
		R16G16_Int,
		R16G16B16_Int,
		R16G16B16A16_Int,
		R32_Int,
		R32G32_Int,
		R32G32B32_Int,
		R32G32B32A32_Int,

#pragma endregion

#pragma region Unsigned integer formats

		R8_UInt,
		R8G8_UInt,
		R8G8B8_UInt,
		R8G8B8A8_UInt,
		R16_UInt,
		R16G16_UInt,
		R16G16B16_UInt,
		R16G16B16A16_UInt,
		R32_UInt,
		R32G32_UInt,
		R32G32B32_UInt,
		R32G32B32A32_UInt,

#pragma endregion

#pragma region Floating-point formats

		R32_Float,
		R32G32_Float,
		R32G32B32_Float,
		R32G32B32A32_Float,
		R64_Float,
		R64G64_Float,
		R64G64B64_Float,
		R64G64B64A64_Float,

#pragma endregion

	};

	namespace Details
	{
		struct FormatInfo
		{
			uint8_t redBits, greenBits, blueBits, alphaBits;
			FormatType formatType;
			uint8_t sizeInBytes;
		};

		BLAZE_API FormatInfo GetFormatInfo(Format format);
	}

	struct VertexAttribute
	{
		Format format;
		size_t offset = 0; // Leave 0 for automatic calculation
	};

	class VertexFormat
	{
	public:
		inline Result PushAttribute(const VertexAttribute& attrib)
		{
			m_offset = attrib.offset == 0 ? m_offset : attrib.offset;

			m_attibutes.push_back(VertexAttribute{ attrib.format, m_offset });

			auto formatInfo = Details::GetFormatInfo(attrib.format);
			m_offset += formatInfo.sizeInBytes;
			m_stride += formatInfo.sizeInBytes;

			return Result::Success;
		}
		inline Result Reset()
		{
			m_attibutes.clear();
			m_stride = 0;
			m_offset = 0;
		}
	private:
		std::vector<VertexAttribute> m_attibutes;
		size_t m_stride;
		size_t m_offset;
	};
};

#endif // BLAZE_FORMAT_H
#include <pch.h>
#include <Blaze/Renderer/Format.h>

namespace Blaze
{
	namespace Details
	{
		static constexpr std::array<FormatInfo, 32> formatInfoTable = 
		{
			// Signed integer formats
			FormatInfo{ 8, 0, 0, 0, FormatType::Int, 1 },
			FormatInfo{ 8, 8, 0, 0, FormatType::Int, 2 },
			FormatInfo{ 8, 8, 8, 0, FormatType::Int, 3 },
			FormatInfo{ 8, 8, 8, 8, FormatType::Int, 4 },

			FormatInfo{ 16,  0,  0,  0, FormatType::Int, 2 },
			FormatInfo{ 16, 16,  0,  0, FormatType::Int, 4 },
			FormatInfo{ 16, 16, 16,  0, FormatType::Int, 6 },
			FormatInfo{ 16, 16, 16, 16, FormatType::Int, 8 },

			FormatInfo{ 32,  0,  0,  0, FormatType::Int, 4 },
			FormatInfo{ 32, 32,  0,  0, FormatType::Int, 8 },
			FormatInfo{ 32, 32, 32,  0, FormatType::Int, 12 },
			FormatInfo{ 32, 32, 32, 32, FormatType::Int, 16 },

			// Unsigned integer formats
			FormatInfo{ 8, 0, 0, 0, FormatType::UInt, 1 },
			FormatInfo{ 8, 8, 0, 0, FormatType::UInt, 2 },
			FormatInfo{ 8, 8, 8, 0, FormatType::UInt, 3 },
			FormatInfo{ 8, 8, 8, 8, FormatType::UInt, 4 },
			
			FormatInfo{ 16,  0,  0,  0, FormatType::UInt, 2 },
			FormatInfo{ 16, 16,  0,  0, FormatType::UInt, 4 },
			FormatInfo{ 16, 16, 16,  0, FormatType::UInt, 6 },
			FormatInfo{ 16, 16, 16, 16, FormatType::UInt, 8 },
			
			FormatInfo{ 32,  0,  0,  0, FormatType::UInt, 4 },
			FormatInfo{ 32, 32,  0,  0, FormatType::UInt, 8 },
			FormatInfo{ 32, 32, 32,  0, FormatType::UInt, 12 },
			FormatInfo{ 32, 32, 32, 32, FormatType::UInt, 16 },

			// Signed integer formats
			FormatInfo{ 32,  0,  0,  0, FormatType::Float, 4 },
			FormatInfo{ 32, 32,  0,  0, FormatType::Float, 8 },
			FormatInfo{ 32, 32, 32,  0, FormatType::Float, 12 },
			FormatInfo{ 32, 32, 32, 32, FormatType::Float, 16 },
			
			FormatInfo{ 64,  0,  0,  0, FormatType::Float, 8 },
			FormatInfo{ 64, 32,  0,  0, FormatType::Float, 16 },
			FormatInfo{ 64, 64, 64,  0, FormatType::Float, 24 },
			FormatInfo{ 64, 64, 64, 64, FormatType::Float, 32 }
		};

		FormatInfo GetFormatInfo(Format format)
		{
			if ((static_cast<size_t>(format) - 1) < formatInfoTable.size())
				return FormatInfo{ 0, 0, 0, 0, FormatType::Invalid, 0 };
			return formatInfoTable[static_cast<size_t>(format) - 1];
		}
	}
}

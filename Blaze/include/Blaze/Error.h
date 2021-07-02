#pragma once

#ifndef BLAZE_ERROR_H
#define BLAZE_ERROR_H

#include <exception>
#include <string>
#include <cstdint>


namespace Blaze
{
	enum class Result : int32_t
	{
		Success = 0,
		UnknownError,
		InvalidParam,
		InvalidCast,
		SystemError,
		AllocationError
	};

	class Exception
		:public std::exception
	{
	public:
		inline Exception(Result code, const char* message = nullptr)
			:std::exception(("Blaze Exception (" + std::to_string(static_cast<int32_t>(m_code)) + "): " + (message ? message : "No message") + '\n').c_str()), m_code(code)
		{
		}

		inline Exception(const Exception& e)
			:std::exception(e), m_code(e.m_code)
		{
		}

		constexpr Result GetCode() { return m_code; }
	private:
		Result m_code;
	};
}

#endif // BLAZE_ERROR_H
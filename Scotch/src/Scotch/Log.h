#pragma once

#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog//fmt/ostr.h"

namespace Scotch {
	class SCOTCH_API Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};

}

// Core Log Macros
#define SH_CORE_TRACE(...)     ::Scotch::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define SH_CORE_INFO(...)      ::Scotch::Log::GetCoreLogger()->info(__VA_ARGS__)
#define SH_CORE_WARN(...)      ::Scotch::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define SH_CORE_ERROR(...)     ::Scotch::Log::GetCoreLogger()->error(__VA_ARGS__)
#define SH_CORE_FATAL(...)     ::Scotch::Log::GetCoreLogger()->fatal(__VA_ARGS__)
								 
// Client Log Macros			 
#define SH_TRACE(...)          ::Scotch::Log::GetClientLogger()->trace(__VA_ARGS__)
#define SH_INFO(...)           ::Scotch::Log::GetClientLogger()->info(__VA_ARGS__)
#define SH_WARN(...)           ::Scotch::Log::GetClientLogger()->warn(__VA_ARGS__)
#define SH_ERROR(...)          ::Scotch::Log::GetClientLogger()->error(__VA_ARGS__)
#define SH_FATAL(...)          ::Scotch::Log::GetClientLogger()->fatal(__VA_ARGS__)
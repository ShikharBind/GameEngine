#pragma once

#ifdef SH_PLATFORM_WINDOWS
	#ifdef SH_BUILD_DLL
		#define SCOTCH_API __declspec(dllexport)
	#else
		#define SCOTCH_API __declspec(dllimport)
	#endif
#else
	#error Scotch only support Windows!
#endif

#ifdef SH_ENABLE_ASSERTS
	#define SH_ASSERT(x, ...) { if(!(x)) {HZ_ERROR(Assertion Failed: {0}, __VA_ARGS__); __debugbreak(); } }
	#define SH_CORE_ASSERT(x, ...) { if(!(x)) {HZ_CORE_ERROR(Assertion Failed: {0}, __VA_ARGS__); __debugbreak(); } }
#else
	#define SH_ASSERT(x, ...)
	#define SH_CORE_ASSERT(x, ...)
#endif


#define BIT(x) (1 << x)

#define SH_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)
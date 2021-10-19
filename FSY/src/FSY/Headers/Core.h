#pragma once

#ifdef FSY_PLATFORM_WINDOWS
	#ifdef FSY_BUILD_DLL
		#define FSY_API __declspec(dllexport)
	#else
		#define FSY_API __declspec(dllimport)
	#endif // FSY_BUILD_DLL
#endif // FSY_PLATFORM_WINDOWS

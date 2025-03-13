#pragma once

// Since only Windows and Linux OS is supported...
#if defined _WIN32 || defined __CYGWIN__
	#define WINDOWS
#else
	#define LINUX
#endif

#if defined(__clang__)
#define CLANG
#elif defined(__GNUC__) || defined(__GNUG__)
#define GCC
#elif defined(_MSC_VER)
#define MSVC
#endif

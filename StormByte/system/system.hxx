#pragma once

#include <StormByte/visibility.h>

#include <filesystem>
#include <string>

namespace StormByte::System {
	#ifdef WINDOWS
	STORMBYTE_PUBLIC std::string			UTF8Encode(const std::wstring&);
	STORMBYTE_PUBLIC std::wstring			UTF8Decode(const std::string&);
	#endif
	STORMBYTE_PUBLIC std::filesystem::path	TempFileName();
	#define CurrentFileDirectory std::filesystem::path(__FILE__).parent_path()
	#define ASSERT_EQUAL(expected, actual) if ((expected) != (actual)) { \
		std::cerr << "Assertion failed at " << __FILE__ << ":" << __LINE__ << ": expected \"" << (expected) << "\", got \"" << (actual) << "\"" << std::endl; \
		return 1; \
	}
	#define ASSERT_FALSE(condition) if ((condition)) { \
		std::cerr << "Assertion failed at " << __FILE__ << ":" << __LINE__ << ": condition is true, expected false" << std::endl; \
		return 1; \
	}
}
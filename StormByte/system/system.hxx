#pragma once

#include <StormByte/visibility.h>

#include <filesystem>
#include <string>

/**
 * @namespace StormByte::System
 * @brief All the classes for handling system exceptions
 */
namespace StormByte::System {
	#ifdef WINDOWS
	STORMBYTE_PUBLIC std::string			UTF8Encode(const std::wstring&);
	STORMBYTE_PUBLIC std::wstring			UTF8Decode(const std::string&);
	#endif
	/**
	 * Safely gets a temporary file name
	 */
	STORMBYTE_PUBLIC std::filesystem::path	TempFileName();
}
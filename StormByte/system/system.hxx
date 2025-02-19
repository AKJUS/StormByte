#pragma once

#include <StormByte/visibility.h>

#include <filesystem>
#include <string>

/**
 * @namespace System
 * @brief All the classes for handling system exceptions
 */
namespace StormByte::System {
	#ifdef WINDOWS
	/**
	 * @brief Converts a wide string to a string
	 * @param ws wide string
	 * @return string
	 */
	STORMBYTE_PUBLIC std::string			UTF8Encode(const std::wstring&);

	/**
	 * @brief Converts a string to a wide string
	 * @param s string
	 * @return wide string
	 */
	STORMBYTE_PUBLIC std::wstring			UTF8Decode(const std::string&);
	#endif
	/**
	 * Safely gets a temporary file name
	 * @return temporary file full path
	 */
	STORMBYTE_PUBLIC std::filesystem::path	TempFileName();
}
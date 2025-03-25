#pragma once

#include <StormByte/visibility.h>

#include <chrono>
#include <filesystem>
#include <string>

/**
 * @namespace System
 * @brief Namespace for system functions
 */
namespace StormByte::System {
	/**
	 * Safely gets a temporary file name
	 * @return temporary file full path
	 */
	STORMBYTE_PUBLIC std::filesystem::path				TempFileName(const std::string& prefix = "TMP");

	/**
	 * Gets current path
	 * @return current path
	 */
	STORMBYTE_PUBLIC std::filesystem::path				CurrentPath();

	/**
	 * Sleeps for a specific duration
	 * @param duration Time to sleep, specified as a std::chrono::duration
	 */
	template <typename Rep, typename Period>
	STORMBYTE_PUBLIC void Sleep(const std::chrono::duration<Rep, Period>& duration);
}
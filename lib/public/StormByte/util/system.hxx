#pragma once

#include <StormByte/visibility.h>

#include <filesystem>
#include <string>

/**
 * @namespace Util
 * @brief Contains utility classes and functions.
 */
namespace StormByte::Util {
	/**
	 * @class System
	 * @brief Utility functions for system
	 */
	struct STORMBYTE_PUBLIC System {
		/**
		 * Safely gets a temporary file name
		 * @return temporary file full path
		 */
		static std::filesystem::path	TempFileName(const std::string& prefix = "TMP");

		/**
		 * Gets current path
		 * @return current path
		 */
		static std::filesystem::path	CurrentPath();

		/**
		 * Sleeps for a specific time
		 * @param seconds time to sleep in microseconds
		 */
		static void 					Sleep(const std::size_t& useconds);
	};
}
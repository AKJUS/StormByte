#pragma once

#include <visibility.h>

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
		static std::filesystem::path	TempFileName();
	};
}
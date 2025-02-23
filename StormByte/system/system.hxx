#pragma once

#include <StormByte/visibility.h>

#include <filesystem>
#include <string>

/**
 * @namespace System
 * @brief All the classes for handling system exceptions
 */
namespace StormByte::System {
	/**
	 * Safely gets a temporary file name
	 * @return temporary file full path
	 */
	STORMBYTE_PUBLIC std::filesystem::path	TempFileName();
}
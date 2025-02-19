#pragma once

#include <StormByte/visibility.h>

/**
 * @namespace Log
 * @brief All the classes for handling logging
 */
namespace StormByte::Log {
	/**
	 * @enum Level
	 * @brief Log level
	 */
	enum class STORMBYTE_PUBLIC Level {
		Debug = 0,	///< Debug level
		Warning,	///< Warning level
		Notice,		///< Notice level
		Info,		///< Info level
		Error,		///< Error level
		Fatal		///< Fatal level
	};

	/**
	 * Gets Level string
	 * @return string
	 */
	constexpr STORMBYTE_PUBLIC const char* GetLevelAsString(const Level& l) noexcept {
		switch(l) {
			case Level::Debug:		return "Debug";
			case Level::Warning:	return "Warning";
			case Level::Notice:		return "Notice";
			case Level::Info:		return "Info";
			case Level::Error:		return "Error";
			case Level::Fatal:		return "Fatal";
			default:				return "Unknown";
		}
	}
}
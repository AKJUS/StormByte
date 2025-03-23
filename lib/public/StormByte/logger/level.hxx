#pragma once

#include <StormByte/visibility.h>

#include <string>

/**
 * @namespace Logger
 * @brief All the classes for handling logging
 */
namespace StormByte::Logger {
	/**
	 * @enum Level
	 * @brief Log level
	 */
	enum class STORMBYTE_PUBLIC Level: unsigned short {
		LowLevel = 0,				///< Low level debug level
		Debug,						///< Debug level
		Warning,					///< Warning level
		Notice,						///< Notice level
		Info,						///< Info level
		Error,						///< Error level
		Fatal						///< Fatal level
	};

	/**
	 * Gets Level string
	 * @return string
	 */
	constexpr STORMBYTE_PUBLIC std::string LevelToString(const Level& l) noexcept {
		switch(l) {
			case Level::LowLevel:	return "LowLevel";
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
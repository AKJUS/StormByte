#pragma once

#include <StormByte/visibility.h>

#include <algorithm>
#include <queue>
#include <string>
#include <sstream>

/**
 * @namespace StormByte::Util::String
 * @brief All the classes for handling strings
 */
namespace StormByte::Util::String {
	/**
	 * Creates a std::string with a given level of indentation
	 * @param level level of indentation
	 * @return string with indentation
	 */
	constexpr std::string STORMBYTE_PUBLIC 			Indent(const int& level) noexcept {
		return level == 0 ? std::string() : std::string(level, '\t');
	}

	/**
	 * Checks if a string is numeric
	 * @param str string to check
	 * @return true if numeric, false otherwise
	 */
	constexpr bool STORMBYTE_PUBLIC					IsNumeric(const std::string& str) noexcept {
		return !str.empty() && std::all_of(str.begin(), str.end(), ::isdigit);
	}

	/**
	 * Explodes a string into a queue of strings
	 * @param str string to explode
	 * @param delimiter delimiter character
	 * @return queue of strings
	 */
	std::queue<std::string> STORMBYTE_PUBLIC 		Explode(const std::string& str, const char& delimiter);
}
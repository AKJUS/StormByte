#pragma once

#include <StormByte/expected.hxx>
#include <StormByte/exception.hxx>

#include <algorithm>
#include <cstdint>
#include <queue>
#include <string>
#include <utility>
#include <vector>

/**
 * @namespace Util
 * @brief Contains utility classes and functions.
 */
namespace StormByte::Util::String {
	/**
	 * Creates a std::string with a given level of indentation
	 * @param level level of indentation
	 * @return string with indentation
	 */
	STORMBYTE_PUBLIC constexpr std::string  								Indent(const int& level) noexcept {
		return level == 0 ? std::string() : std::string(level, '\t');
	}

	/**
	 * Checks if a string is numeric
	 * @param str string to check
	 * @return true if numeric, false otherwise
	 */
	STORMBYTE_PUBLIC constexpr bool 										IsNumeric(const std::string& str) noexcept {
		return !str.empty() && std::all_of(str.begin(), str.end(), ::isdigit);
	}

	/**
	 * Converts a string to lower case
	 * @param str string to convert
	 * @return lower case string
	 */
	STORMBYTE_PUBLIC std::string											ToLower(const std::string& str) noexcept;

	/**
	 * Converts a string to upper case
	 * @param str string to convert
	 * @return upper case string
	 */
	STORMBYTE_PUBLIC std::string											ToUpper(const std::string& str) noexcept;

	/**
	 * Explodes a string into a queue of strings
	 * @param str string to explode
	 * @param delimiter delimiter character
	 * @return queue of strings
	 */
	STORMBYTE_PUBLIC std::queue<std::string>  								Explode(const std::string& str, const char delimiter);

	/**
	 * Split a string into a vector of strings delimited by spaces
	 * @param str string to explode
	 * @return vector of strings
	 */
	STORMBYTE_PUBLIC std::vector<std::string>								Split(const std::string& str);

	/**
	 * Splits a fraction string into a pair of integers
	 * @param fraction fraction string to split
	 * @return pair of integers
	 */
	STORMBYTE_PUBLIC StormByte::Expected<std::pair<int, int>, Exception>	SplitFraction(const std::string& fraction);

	/**
	 * Splits a fraction string into a pair of integers and multiply by needed factor to get desired denominator
	 * @param str string to split
	 * @param denominator desired denominator
	 * @return pair of integers
	 */
	STORMBYTE_PUBLIC StormByte::Expected<std::pair<int, int>, Exception>	SplitFraction(const std::string& str, const int& denominator);

	/**
	 * @brief Gets a string from a number of bytes (example: 10.52 GiB)
	 * @param bytes number of bytes
	 * @return string
	 */
	STORMBYTE_PUBLIC std::string											HumanReadableByteSize(const uint64_t& bytes) noexcept;

	#ifdef WINDOWS
	/**
	 * @brief Converts a wide string to a string
	 * @param ws wide string
	 * @return string
	 */
	STORMBYTE_PUBLIC std::string											UTF8Encode(const std::wstring& ws);

	/**
	 * @brief Converts a string to a wide string
	 * @param s string
	 * @return wide string
	 */
	STORMBYTE_PUBLIC std::wstring											UTF8Decode(const std::string& s);
	#endif

	/**
	 * @brief Sanitizes newlines in a string
	 * @param str string to sanitize
	 * @return sanitized string
	 */
	STORMBYTE_PUBLIC std::string 											SanitizeNewlines(const std::string& str) noexcept;
}
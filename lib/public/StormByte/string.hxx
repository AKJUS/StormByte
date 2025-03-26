#pragma once

#include <StormByte/expected.hxx>
#include <StormByte/exception.hxx>

#include <algorithm>
#include <cstdint>
#include <cmath>
#include <iomanip>
#include <locale>
#include <queue>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

/**
 * @namespace StormByte
 * @brief Main namespace for the StormByte library.
 *
 * The `StormByte` namespace serves as the root for all components and utilities in the StormByte library.
 * It provides foundational classes and tools for building robust, thread-safe, and efficient applications.
 */
namespace StormByte::String {
	/**
	 * @enum Format
	 * @brief String formatting options.
	 *
	 * Options for formatting strings, such as raw output or human-readable formats.
	 */
	enum class STORMBYTE_PUBLIC Format : unsigned short {
		Raw,                        ///< Raw output.
		HumanReadableNumber,        ///< Human-readable number format.
		HumanReadableBytes          ///< Human-readable bytes format.
	};

	/**
	 * @brief Creates a string with a given level of indentation.
	 * @param level The level of indentation.
	 * @return A string with the specified indentation.
	 */
	constexpr STORMBYTE_PUBLIC std::string Indent(const int& level) noexcept {
		return level == 0 ? std::string() : std::string(level, '\t');
	}

	/**
	 * @brief Checks if a string is numeric.
	 * @param str The string to check.
	 * @return `true` if the string is numeric, `false` otherwise.
	 */
	constexpr STORMBYTE_PUBLIC bool IsNumeric(const std::string& str) noexcept {
		return !str.empty() && std::all_of(str.begin(), str.end(), ::isdigit);
	}

	/**
	 * @brief Converts a string to lowercase.
	 * @param str The string to convert.
	 * @return The lowercase version of the string.
	 */
	STORMBYTE_PUBLIC std::string ToLower(const std::string& str) noexcept;

	/**
	 * @brief Converts a string to uppercase.
	 * @param str The string to convert.
	 * @return The uppercase version of the string.
	 */
	STORMBYTE_PUBLIC std::string ToUpper(const std::string& str) noexcept;

	/**
	 * @brief Splits a string into a queue of substrings based on a delimiter.
	 * @param str The string to split.
	 * @param delimiter The delimiter character.
	 * @return A queue of substrings.
	 */
	STORMBYTE_PUBLIC std::queue<std::string> Explode(const std::string& str, const char delimiter);

	/**
	 * @brief Splits a string into a vector of substrings delimited by spaces.
	 * @param str The string to split.
	 * @return A vector of substrings.
	 */
	STORMBYTE_PUBLIC std::vector<std::string> Split(const std::string& str);

	/**
	 * @brief Splits a fraction string into a pair of integers.
	 * @param fraction The fraction string to split.
	 * @return A pair of integers representing the numerator and denominator.
	 */
	STORMBYTE_PUBLIC StormByte::Expected<std::pair<int, int>, Exception> SplitFraction(const std::string& fraction);

	/**
	 * @brief Splits a fraction string into a pair of integers and scales it to a desired denominator.
	 * @param str The fraction string to split.
	 * @param denominator The desired denominator.
	 * @return A pair of integers representing the scaled numerator and denominator.
	 */
	STORMBYTE_PUBLIC StormByte::Expected<std::pair<int, int>, Exception> SplitFraction(const std::string& str, const int& denominator);

	// Main HumanReadable template function
	template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> && !std::is_same_v<T, wchar_t>>>
	STORMBYTE_PUBLIC std::string HumanReadable(const T& number, const Format& format, const std::string& locale = "en_US.UTF-8") noexcept;

	/**
	 * @brief Converts a wide string to a UTF-8 encoded string.
	 * @param ws The wide string to convert.
	 * @throws std::runtime_error if the conversion fails.
	 * @return The UTF-8 encoded string.
	 */
	STORMBYTE_PUBLIC std::string UTF8Encode(const std::wstring& ws);

	/**
	 * @brief Converts a UTF-8 encoded string to a wide string.
	 * @param s The UTF-8 encoded string to convert.
	 * @throws std::runtime_error if the conversion fails.
	 * @return The wide string.
	 */
	STORMBYTE_PUBLIC std::wstring UTF8Decode(const std::string& s);

	/**
	 * @brief Sanitizes newlines in a string.
	 * @param str The string to sanitize.
	 * @return The sanitized string.
	 */
	STORMBYTE_PUBLIC std::string SanitizeNewlines(const std::string& str) noexcept;
}

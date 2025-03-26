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
 * @namespace String
 * @brief Namespace for string manipulation functions.
 */
namespace StormByte::String {
	/**
	 * @enum Format
	 * @brief String Format
	 */
	enum class STORMBYTE_PUBLIC Format: unsigned short {
		Raw,                                                				///< Raw output
		HumanReadableNumber,                                				///< Human-readable output
		HumanReadableBytes                                  				///< Human-readable bytes output
	};

	/**
	 * Creates a std::string with a given level of indentation.
	 * @param level Level of indentation.
	 * @return String with indentation.
	 */
	constexpr STORMBYTE_PUBLIC std::string 									Indent(const int& level) noexcept {
		return level == 0 ? std::string() : std::string(level, '\t');
	}

	/**
	 * Checks if a string is numeric.
	 * @param str String to check.
	 * @return true if numeric, false otherwise.
	 */
	constexpr STORMBYTE_PUBLIC bool 										IsNumeric(const std::string& str) noexcept {
		return !str.empty() && std::all_of(str.begin(), str.end(), ::isdigit);
	}

	/**
	 * Converts a string to lowercase.
	 * @param str String to convert.
	 * @return Lowercase string.
	 */
	STORMBYTE_PUBLIC std::string 											ToLower(const std::string& str) noexcept;

	/**
	 * Converts a string to uppercase.
	 * @param str String to convert.
	 * @return Uppercase string.
	 */
	STORMBYTE_PUBLIC std::string 											ToUpper(const std::string& str) noexcept;

	/**
	 * Explodes a string into a queue of strings.
	 * @param str String to explode.
	 * @param delimiter Delimiter character.
	 * @return Queue of strings.
	 */
	STORMBYTE_PUBLIC std::queue<std::string> 								Explode(const std::string& str, const char delimiter);

	/**
	 * Splits a string into a vector of strings delimited by spaces.
	 * @param str String to split.
	 * @return Vector of strings.
	 */
	STORMBYTE_PUBLIC std::vector<std::string> 								Split(const std::string& str);

	/**
	 * Splits a fraction string into a pair of integers.
	 * @param fraction Fraction string to split.
	 * @return Pair of integers.
	 */
	STORMBYTE_PUBLIC StormByte::Expected<std::pair<int, int>, Exception> 	SplitFraction(const std::string& fraction);

	/**
	 * Splits a fraction string into a pair of integers and multiplies by a factor to get the desired denominator.
	 * @param str String to split.
	 * @param denominator Desired denominator.
	 * @return Pair of integers.
	 */
	STORMBYTE_PUBLIC StormByte::Expected<std::pair<int, int>, Exception> 	SplitFraction(const std::string& str, const int& denominator);

	// Main HumanReadable template function
	template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> && !std::is_same_v<T, wchar_t>>>
	STORMBYTE_PUBLIC std::string 											HumanReadable(const T& number, const Format& format, const std::string& locale = "en_US.UTF-8") noexcept;

	/**
	 * @brief Converts a wide string to a string.
	 * @param ws Wide string to convert.
	 * @throws std::runtime_error if conversion fails.
	 * @return Converted string.
	 */
	STORMBYTE_PUBLIC std::string 											UTF8Encode(const std::wstring& ws);

	/**
	 * @brief Converts a string to a wide string.
	 * @param s String to convert.
	 * @throws std::runtime_error if conversion fails.
	 * @return Converted wide string.
	 */
	STORMBYTE_PUBLIC std::wstring 											UTF8Decode(const std::string& s);

	/**
	 * @brief Sanitizes newlines in a string.
	 * @param str String to sanitize.
	 * @return Sanitized string.
	 */
	STORMBYTE_PUBLIC std::string 											SanitizeNewlines(const std::string& str) noexcept;
}

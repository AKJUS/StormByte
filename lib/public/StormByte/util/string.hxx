#pragma once

#include <StormByte/expected.hxx>
#include <StormByte/exception.hxx>

#include <algorithm>
#include <cstdint>
#include <cmath>
#include <iomanip>
#include <locale>
#include <format>
#include <queue>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

/**
 * @namespace Util
 * @brief Contains utility classes and functions.
 */
namespace StormByte::Util {
	struct STORMBYTE_PUBLIC String {
		/**
		 * @enum Format
		 * @brief String Format
		 */
		enum class Format: unsigned short {
			Raw, 													///< Raw output
			HumanReadableNumber, 									///< Human readable output
			HumanReadableBytes 										///< Human readable bytes output
		};

		/**
		 * Creates a std::string with a given level of indentation
		 * @param level level of indentation
		 * @return string with indentation
		 */
		static constexpr std::string  								Indent(const int& level) noexcept {
			return level == 0 ? std::string() : std::string(level, '\t');
		}

		/**
		 * Checks if a string is numeric
		 * @param str string to check
		 * @return true if numeric, false otherwise
		 */
		static constexpr bool 										IsNumeric(const std::string& str) noexcept {
			return !str.empty() && std::all_of(str.begin(), str.end(), ::isdigit);
		}

		/**
		 * Converts a string to lower case
		 * @param str string to convert
		 * @return lower case string
		 */
		static std::string											ToLower(const std::string& str) noexcept;

		/**
		 * Converts a string to upper case
		 * @param str string to convert
		 * @return upper case string
		 */
		static std::string											ToUpper(const std::string& str) noexcept;

		/**
		 * Explodes a string into a queue of strings
		 * @param str string to explode
		 * @param delimiter delimiter character
		 * @return queue of strings
		 */
		static std::queue<std::string>  							Explode(const std::string& str, const char delimiter);

		/**
		 * Split a string into a vector of strings delimited by spaces
		 * @param str string to explode
		 * @return vector of strings
		 */
		static std::vector<std::string>								Split(const std::string& str);

		/**
		 * Splits a fraction string into a pair of integers
		 * @param fraction fraction string to split
		 * @return pair of integers
		 */
		static StormByte::Expected<std::pair<int, int>, Exception>	SplitFraction(const std::string& fraction);

		/**
		 * Splits a fraction string into a pair of integers and multiply by needed factor to get desired denominator
		 * @param str string to split
		 * @param denominator desired denominator
		 * @return pair of integers
		 */
		static StormByte::Expected<std::pair<int, int>, Exception>	SplitFraction(const std::string& str, const int& denominator);

		/**
		 * @brief Gets a string from a number of bytes (example: 10.52 GiB)
		 * @param number number to handle
		 * @param format format
		 * @tparam T number type
		 * @return string
		 */
		template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> && !std::is_same_v<T, wchar_t>>>
		static std::string HumanReadable(const T& number, const Format& format, const std::string& locale = "en_US.UTF-8") noexcept {
			switch(format) {
				case Format::Raw:
					return std::to_string(number);
				case Format::HumanReadableNumber:
					return HumanReadableNumber(number, locale);
				case Format::HumanReadableBytes:
					return HumanReadableByteSize(number, locale);
				default:
					return std::to_string(number);
			}
		}

		/**
		 * @brief Converts a wide string to a string
		 * @param ws wide string
		 * @throws std::runtime_error if conversion fails
		 * @return string
		 */
		static std::string											UTF8Encode(const std::wstring& ws);

		/**
		 * @brief Converts a string to a wide string
		 * @param s string
		 * @throws std::runtime_error if conversion fails
		 * @return wide string
		 */
		static std::wstring											UTF8Decode(const std::string& s);

		/**
		 * @brief Sanitizes newlines in a string
		 * @param str string to sanitize
		 * @return sanitized string
		 */
		static std::string 											SanitizeNewlines(const std::string& str) noexcept;

		private:
			template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
			static std::string 										HumanReadableByteSize(const T& bytes, const std::string& locale) noexcept {
				try {
					constexpr uint64_t KB = 1024;
					constexpr uint64_t MB = KB * 1024;
					constexpr uint64_t GB = MB * 1024;
					constexpr uint64_t TB = GB * 1024;
					constexpr uint64_t PB = TB * 1024;
			
					double value = static_cast<double>(bytes);
					std::string suffix = "Bytes";
			
					if (bytes >= PB) {
						value /= PB;
						suffix = "PiB";
					} else if (bytes >= TB) {
						value /= TB;
						suffix = "TiB";
					} else if (bytes >= GB) {
						value /= GB;
						suffix = "GiB";
					} else if (bytes >= MB) {
						value /= MB;
						suffix = "MiB";
					} else if (bytes >= KB) {
						value /= KB;
						suffix = "KiB";
					}
			
					// Handle rounding and precision
					std::ostringstream oss;
					try {
						oss.imbue(std::locale(locale));
					} catch (...) {
						oss.imbue(std::locale("C"));
					}
			
					// Round to nearest integer if the fractional part is negligible
					if (std::fabs(value - std::round(value)) < 0.01) {
						oss << static_cast<int64_t>(std::round(value)); // Show as integer if close enough
					} else if (value < 0.01) {
						// If the value is too small to show meaningful decimals, show as "0"
						oss << "0";
					} else {
						// Otherwise, show two decimal places
						oss << std::fixed << std::setprecision(2) << value;
					}
			
					return oss.str() + " " + suffix;
			
				} catch (...) {
					return std::to_string(bytes) + " Bytes"; // Fallback for errors
				}
			}		
		
			template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> && !std::is_same_v<T, wchar_t>>>
			static std::string HumanReadableNumber(const T& number, const std::string& locale) noexcept {
				try {
					std::ostringstream oss;

					// Set the locale for thousands separators
					try {
						oss.imbue(std::locale(locale));
					} catch (...) {
						oss.imbue(std::locale("C")); // Fallback to "C" locale
					}

					if constexpr (std::is_integral_v<T>) {
						// Always format integral numbers without decimals
						oss << number;
					} else if constexpr (std::is_floating_point_v<T>) {
						// Floating-point: check if it is effectively an integer
						if (std::fmod(number, 1.0) == 0.0) {
							// Exact integer: no decimals
							oss << static_cast<int64_t>(number);
						} else {
							// Otherwise, format with two decimal places
							oss << std::fixed << std::setprecision(2) << number;
						}
					}

					return oss.str();
				} catch (...) {
					// Fallback in case of errors
					return std::to_string(number);
				}
			}
	};
}
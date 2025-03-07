#pragma once

#include <visibility.h>

#include <algorithm>
#include <cstdint>
#include <queue>
#include <string>

/**
 * @namespace Util
 * @brief Contains utility classes and functions.
 */
namespace StormByte::Util {
	/**
	 * @class String
	 * @brief Utility functions for strings
	 */
	struct STORMBYTE_PUBLIC String {
		/**
		 * Creates a std::string with a given level of indentation
		 * @param level level of indentation
		 * @return string with indentation
		 */
		static constexpr std::string  				Indent(const int& level) noexcept {
			return level == 0 ? std::string() : std::string(level, '\t');
		}

		/**
		 * Checks if a string is numeric
		 * @param str string to check
		 * @return true if numeric, false otherwise
		 */
		static constexpr bool 						IsNumeric(const std::string& str) noexcept {
			return !str.empty() && std::all_of(str.begin(), str.end(), ::isdigit);
		}

		/**
		 * Converts a string to lower case
		 * @param str string to convert
		 * @return lower case string
		 */
		static std::string							ToLower(const std::string& str) noexcept;

		/**
		 * Converts a string to upper case
		 * @param str string to convert
		 * @return upper case string
		 */
		static std::string							ToUpper(const std::string& str) noexcept;

		/**
		 * Explodes a string into a queue of strings
		 * @param str string to explode
		 * @param delimiter delimiter character
		 * @return queue of strings
		 */
		static std::queue<std::string>  			Explode(const std::string& str, const char& delimiter);

		/**
		 * @brief Gets a string from a number of bytes (example: 10.52 GiB)
		 * @param bytes number of bytes
		 * @return string
		 */
		static std::string							HumanReadableByteSize(const uint64_t& bytes) noexcept;

		#ifdef WINDOWS
		/**
		 * @brief Converts a wide string to a string
		 * @param ws wide string
		 * @return string
		 */
		static std::string							UTF8Encode(const std::wstring& ws);

		/**
		 * @brief Converts a string to a wide string
		 * @param s string
		 * @return wide string
		 */
		static std::wstring							UTF8Decode(const std::string& s);
		#endif
	};
}
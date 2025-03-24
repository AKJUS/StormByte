#pragma once

#include <StormByte/logger/level.hxx>

#include <memory>
#include <ostream>

/**
 * @namespace Logger
 * @brief All the classes for handling logging
 */
namespace StormByte::Logger {
	/**
	 * @class Log
	 * @brief Log class
	 */
	class STORMBYTE_PUBLIC Log {
		public:
			/**
			 * Constructor
			 * @param out output stream
			 * @param level log level
			 * @param format custom format
			 */
			Log(std::ostream& out, const Level& level = Level::Info, const std::string& format = "[%L] %T") noexcept;

			/**
			 * Copy constructor
			 */
			Log(const Log&)														= delete;

			/**
			 * Move constructor
			 */
			Log(Log&&) noexcept													= default;

			/**
			 * Assignment operator
			 */
			Log& operator=(const Log&)											= delete;

			/**
			 * Move operator
			 */
			Log& operator=(Log&&) noexcept										= default;

			/**
			 * Destructor
			 */
			virtual ~Log() noexcept												= default;

			/**
			 * Sets the current log level
			 * @param level log level
			 * @return Log instance reference
			 */
			Log& 																operator<<(const Level& level) noexcept;

			/**
			 * Overload for operator<<
			 * @param value value
			 * @tparam T type of value
			 * @return Log instance
			 */
			template <typename T> Log& 											operator<<(const T& value) noexcept {
				if constexpr (std::is_same_v<T, bool>) {
					// Handle booleans
					print_message(value ? "true" : "false");
				} else if constexpr (std::is_integral_v<T> || std::is_floating_point_v<T>) {
					// Handle numbers
					print_message(std::to_string(value));
				} else if constexpr (std::is_same_v<T, std::string> || std::is_same_v<T, const char*> || std::is_array_v<T>) {
					// Handle std::string, C-style strings, and string literals
					print_message(std::string(value)); // Convert char[] to std::string
				} else {
					// Unsupported types trigger the static_assert
					static_assert(!std::is_same_v<T, T>, "Unsupported type for Log::operator<<");
				}
				return *this;
			}

			// Overload for manipulators like std::endl
			Log& 																operator<<(std::ostream& (*manip)(std::ostream&)) noexcept;

		protected:
			/**
			 * Prints the time
			 */
			void 																print_time() const noexcept;

			/**
			 * Prints the log level
			 */
			void 																print_level() const noexcept;

			/**
			 * Prints the header
			 */
			void 																print_header() const noexcept;

			/** Prints the message
			 * @param message message
			 */
			void 																print_message(const std::string& message) noexcept;

			/**
			 * Output stream
			 */
			std::ostream& m_out;

			/**
			 * Print level and current level
			 */
			Level m_print_level;												///< Print level
			Level m_current_level;												///< Current level

			/**
			 * Line started
			 */
			bool m_line_started;												///< Line started
			
			/**
			 * Custom user Format
			 */
			const std::string m_format;											///< Custom user format %L for Level and %T for Time
	};

	/**
	 * Overload for operator<<
	 * @param logger logger
	 * @param value value
	 * @tparam Ptr type of logger
	 * @tparam T type of value
	 * @return logger
	 */
	template <typename Ptr, typename T> Ptr& 									operator<<(Ptr& logger, const T& value) noexcept {
		if (logger) {
			*logger << value; // Forward the call to the underlying Log instance
		}
		return logger;
	}

	/**
	 * Overload for operator<<
	 * @param logger logger
	 * @param level log level
	 * @tparam Ptr type of logger
	 * @return logger
	 */
	template <typename Ptr> Ptr& 												operator<<(Ptr& logger, std::ostream& (*manip)(std::ostream&)) noexcept {
		if (logger) {
			*logger << manip; // Forward the manipulator (e.g., std::endl) to the Log instance
		}
		return logger;
	}
}
#pragma once

#include <StormByte/log/level.hxx>

#include <ostream>

/**
 * @namespace Log
 * @brief All the classes for handling logging
 */
namespace StormByte::Log {
	/**
	 * @class Logger
	 * @brief Logger class
	 */
	class STORMBYTE_PUBLIC Logger {
		public:
			/**
			 * Constructor
			 * @param out output stream
			 * @param level log level
			 * @param format custom format
			 */
			Logger(std::ostream& out, const Level& level = Level::Info, const std::string& format = "[%L] %T") noexcept;

			/**
			 * Copy constructor
			 */
			Logger(const Logger&)					= delete;

			/**
			 * Move constructor
			 */
			Logger(Logger&&) noexcept				= default;

			/**
			 * Assignment operator
			 */
			Logger& operator=(const Logger&)		= delete;

			/**
			 * Move operator
			 */
			Logger& operator=(Logger&&) noexcept	= default;

			/**
			 * Destructor
			 */
			virtual ~Logger() noexcept				= default;

			/**
			 * Sets the log level
			 * @param level log level
			 */
			Logger& operator<<(const Level& level) noexcept;

			/**
			 * Logs a string
			 * @param str string
			 */
			Logger& operator<<(const std::string& str) noexcept;

			/**
			 * Logs a const char*
			 * @param str string
			 */
			Logger& operator<<(const char* str) noexcept;

			/**
			 * Logs an integer
			 * @param value integer
			 */
			Logger& operator<<(const int& value) noexcept;

			/**
			 * Logs a double
			 * @param value double
			 */
			Logger& operator<<(const double& value) noexcept;

			/**
			 * Logs a boolean
			 * @param value boolean
			 */
			Logger& operator<<(const bool& value) noexcept;

		protected:
			/**
			 * Prints the time
			 */
			void print_time() const noexcept;

			/**
			 * Prints the log level
			 */
			void print_level() const noexcept;

			/**
			 * Prints the header
			 */
			void print_header() const noexcept;

			/** Prints the message
			 * @param message message
			 */
			void print_message(const std::string& message) noexcept;

			/**
			 * Output stream
			 */
			std::ostream& m_out;

			/**
			 * Print level and current level
			 */
			Level m_print_level;		///< Print level
			Level m_current_level;		///< Current level

			/**
			 * Line started
			 */
			bool m_line_started;		///< Line started
			
			/**
			 * Custom user Format
			 */
			const std::string m_format;	///< Custom user format %L for Level and %T for Time
	};
}
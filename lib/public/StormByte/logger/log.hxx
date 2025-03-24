#pragma once

#include <StormByte/logger/level.hxx>
#include <StormByte/util/string.hxx>

#include <memory>
#include <optional>
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
	class STORMBYTE_PUBLIC Log final {
		// Friend declarations for manipulators
		friend Log& humanreadable_number(Log& logger) noexcept;
		friend Log& humanreadable_bytes(Log& logger) noexcept;
		friend Log& nohumanreadable(Log& logger) noexcept;

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
		Log(const Log&) = delete;

		/**
		 * Move constructor
		 */
		Log(Log&&) noexcept = default;

		/**
		 * Assignment operator
		 */
		Log& operator=(const Log&) = delete;

		/**
		 * Move operator
		 */
		Log& operator=(Log&&) noexcept = default;

		/**
		 * Destructor
		 */
		~Log() noexcept = default;

		/**
		 * Sets the current log level
		 * @param level log level
		 * @return Log instance reference
		 */
		Log& operator<<(const Level& level) noexcept;

		// Overload for manipulators like std::endl
		Log& 									operator<<(std::ostream& (*manip)(std::ostream&)) noexcept;

		/**
		 * @brief Overload for manipulators
		 * @param manip manipulator
		 * @return Log instance
		 */
		inline Log& operator<<(Log& (*manip)(Log&) noexcept) {
			return manip(*this);
		}

		/**
		 * Overload for operator<<
		 * @param value value
		 * @tparam T type of value
		 * @return Log instance
		 */
		template <typename T>
		Log& operator<<(const T& value) noexcept
		requires (!std::is_same_v<std::decay_t<T>, Log& (*)(Log&) noexcept>) {
			using DecayedT = std::decay_t<T>;

			if constexpr (std::is_same_v<DecayedT, bool>) {
				print_message(value ? "true" : "false");
			} else if constexpr (std::is_integral_v<DecayedT> || std::is_floating_point_v<DecayedT>) {
				print_message(value);
			} else if constexpr (std::is_same_v<DecayedT, std::string> || std::is_same_v<DecayedT, const char*>) {
				print_message(std::string(value));
			} else if constexpr (std::is_same_v<DecayedT, std::wstring> || std::is_same_v<DecayedT, const wchar_t*>) {
				print_message(Util::String::UTF8Encode(value));
			} else if constexpr (std::is_array_v<T> && std::is_same_v<std::remove_extent_t<T>, char>) {
				print_message(std::string(value));
			} else {
				static_assert(!std::is_same_v<T, T>, "Unsupported type for Log::operator<<");
			}
			return *this;
		}

	private:
		std::ostream& m_out;
		Level m_print_level;                     ///< Print level
		std::optional<Level> m_current_level;    ///< Current level
		bool m_header_displayed;                 ///< Line started
		const std::string m_format;              ///< Custom user format %L for Level and %T for Time
		Util::String::Format m_human_readable_format; ///< Human readable size

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

		// Overload for numeric types
		template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> && !std::is_same_v<T, wchar_t>>>
		void print_message(const T& value) noexcept {
			std::string message;
			if (m_human_readable_format == Util::String::Format::Raw) {
				message = std::to_string(value);
			} else {
				// Pass raw numeric type (without std::decay_t) to HumanReadable
				message = Util::String::HumanReadable(value, m_human_readable_format, "en_US.UTF-8");
			}
			print_message(message); // Forward to std::string overload
		}

		// Overload for std::string
		void print_message(const std::string& message) noexcept;

		// Overload for wchar_t
		void print_message(const wchar_t& value) noexcept;
	};

	// Manipulator to enable human-readable number formatting
	inline Log& humanreadable_number(Log& logger) noexcept {
		logger.m_human_readable_format = Util::String::Format::HumanReadableNumber;
		return logger;
	}	

	// Manipulator to enable human-readable bytes formatting
	inline Log& humanreadable_bytes(Log& logger) noexcept {
		logger.m_human_readable_format = Util::String::Format::HumanReadableBytes;
		return logger;
	}

	// Manipulator to disable human-readable formatting (Raw output)
	inline Log& nohumanreadable(Log& logger) noexcept {
		logger.m_human_readable_format = Util::String::Format::Raw;
		return logger;
	}

	template <typename Ptr, typename T>
	Ptr& operator<<(Ptr& logger, const T& value) noexcept
	requires std::is_same_v<Ptr, std::shared_ptr<Log>> || std::is_same_v<Ptr, std::unique_ptr<Log>> {
		if (logger) {
			*logger << value; // Forward the call to the underlying Log instance
		}
		return logger;
	}

	template <typename Ptr>
	Ptr& operator<<(Ptr& logger, const Level& level) noexcept
	requires std::is_same_v<Ptr, std::shared_ptr<Log>> || std::is_same_v<Ptr, std::unique_ptr<Log>> {
		if (logger) {
			*logger << level; // Forward the Level to the underlying Log instance
		}
		return logger;
	}

	template <typename Ptr>
	Ptr& operator<<(Ptr& logger, std::ostream& (*manip)(std::ostream&)) noexcept
	requires std::is_same_v<Ptr, std::shared_ptr<Log>> || std::is_same_v<Ptr, std::unique_ptr<Log>> {
		if (logger) {
			*logger << manip; // Forward manipulators like std::endl
		}
		return logger;
	}
}
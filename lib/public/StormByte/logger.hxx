#pragma once

#include <StormByte/string.hxx>

#include <memory>
#include <optional>
#include <ostream>

/**
 * @namespace StormByte
 * @brief Main namespace for the StormByte library and components
 */
namespace StormByte {
	/**
	 * @class Logger
	 * @brief Logger class
	 */
	class STORMBYTE_PUBLIC Logger final {
		// Friend declarations for manipulators
		friend STORMBYTE_PUBLIC Logger& humanreadable_number(Logger& logger) noexcept;
		friend STORMBYTE_PUBLIC Logger& humanreadable_bytes(Logger& logger) noexcept;
		friend STORMBYTE_PUBLIC Logger& nohumanreadable(Logger& logger) noexcept;

	public:
		/**
		 * @enum Level
		 * @brief Logger level
		 */
		enum class STORMBYTE_PUBLIC Level : unsigned short {
			LowLevel = 0,										///< Low level debug level
			Debug,												///< Debug level
			Warning,											///< Warning level
			Notice,												///< Notice level
			Info,												///< Info level
			Error,												///< Error level
			Fatal												///< Fatal level
		};

		/**
		 * Gets Level string
		 * @return string
		 */
		constexpr static std::string LevelToString(const Level& l) noexcept {
			switch (l) {
			case Level::LowLevel:							return "LowLevel";
			case Level::Debug:								return "Debug";
			case Level::Warning:							return "Warning";
			case Level::Notice:								return "Notice";
			case Level::Info:								return "Info";
			case Level::Error:								return "Error";
			case Level::Fatal:								return "Fatal";
			default:										return "Unknown";
			}
		}

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
		Logger(const Logger&) = delete;

		/**
		 * Move constructor
		 */
		Logger(Logger&&) noexcept = default;

		/**
		 * Assignment operator
		 */
		Logger& operator=(const Logger&) = delete;

		/**
		 * Move operator
		 */
		Logger& operator=(Logger&&) noexcept = default;

		/**
		 * Destructor
		 */
		~Logger() noexcept = default;

		/**
		 * Sets the current log level
		 * @param level log level
		 * @return Logger instance reference
		 */
		Logger& operator<<(const Level& level) noexcept;

		// Overload for manipulators like std::endl
		Logger& operator<<(std::ostream& (*manip)(std::ostream&)) noexcept;

		/**
		 * @brief Overload for manipulators
		 * @param manip manipulator
		 * @return Logger instance
		 */
		inline Logger& operator<<(Logger& (*manip)(Logger&) noexcept) {
			return manip(*this);
		}

		/**
		 * Overload for operator<<
		 * @param value value
		 * @tparam T type of value
		 * @return Logger instance
		 */
		template <typename T>
		Logger& operator<<(const T& value) noexcept
			requires (!std::is_same_v<std::decay_t<T>, Logger& (*)(Logger&) noexcept>) {
			using DecayedT = std::decay_t<T>;

			if constexpr (std::is_same_v<DecayedT, bool>) {
				print_message(value ? "true" : "false");
			}
			else if constexpr (std::is_integral_v<DecayedT> || std::is_floating_point_v<DecayedT>) {
				print_message(value);
			}
			else if constexpr (std::is_same_v<DecayedT, std::string> || std::is_same_v<DecayedT, const char*>) {
				print_message(std::string(value));
			}
			else if constexpr (std::is_same_v<DecayedT, std::wstring> || std::is_same_v<DecayedT, const wchar_t*>) {
				print_message(String::UTF8Encode(value));
			}
			else if constexpr (std::is_array_v<T> && std::is_same_v<std::remove_extent_t<T>, char>) {
				print_message(std::string(value));
			}
			else {
				static_assert(!std::is_same_v<T, T>, "Unsupported type for Logger::operator<<");
			}
			return *this;
		}

	private:
		std::ostream& m_out;
		Level m_print_level;                     				///< Print level
		std::optional<Level> m_current_level;    				///< Current level
		bool m_header_displayed;                 				///< Line started
		const std::string m_format;              				///< Custom user format %L for Level and %T for Time
		String::Format m_human_readable_format; 				///< Human readable size

		/**
		 * Prints the time
		 */
		void 													print_time() const noexcept;

		/**
		 * Prints the log level
		 */
		void 													print_level() const noexcept;

		/**
		 * Prints the header
		 */
		void 													print_header() const noexcept;

		// Overload for numeric types
		template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> && !std::is_same_v<T, wchar_t>>>
		void 													print_message(const T& value) noexcept {
			std::string message;
			if (m_human_readable_format == String::Format::Raw) {
				message = std::to_string(value);
			}
			else {
				// Pass raw numeric type (without std::decay_t) to HumanReadable
				message = String::HumanReadable(value, m_human_readable_format, "en_US.UTF-8");
			}
			print_message(message); // Forward to std::string overload
		}

		// Overload for std::string
		void 													print_message(const std::string& message) noexcept;

		// Overload for wchar_t
		void 													print_message(const wchar_t& value) noexcept;
	};

	// Manipulator to enable human-readable number formatting
	inline STORMBYTE_PUBLIC Logger& humanreadable_number(Logger& logger) noexcept {
		logger.m_human_readable_format = String::Format::HumanReadableNumber;
		return logger;
	}

	// Manipulator to enable human-readable bytes formatting
	inline STORMBYTE_PUBLIC Logger& humanreadable_bytes(Logger& logger) noexcept {
		logger.m_human_readable_format = String::Format::HumanReadableBytes;
		return logger;
	}

	// Manipulator to disable human-readable formatting (Raw output)
	inline STORMBYTE_PUBLIC Logger& nohumanreadable(Logger& logger) noexcept {
		logger.m_human_readable_format = String::Format::Raw;
		return logger;
	}

	template <typename Ptr, typename T>
	Ptr& operator<<(Ptr& logger, const T& value) noexcept
		requires std::is_same_v<Ptr, std::shared_ptr<Logger>> || std::is_same_v<Ptr, std::unique_ptr<Logger>> {
		if (logger) {
			*logger << value; // Forward the call to the underlying Logger instance
		}
		return logger;
	}

	template <typename Ptr>
	Ptr& operator<<(Ptr& logger, const Logger::Level& level) noexcept
		requires std::is_same_v<Ptr, std::shared_ptr<Logger>> || std::is_same_v<Ptr, std::unique_ptr<Logger>> {
		if (logger) {
			*logger << level; // Forward the Level to the underlying Logger instance
		}
		return logger;
	}

	template <typename Ptr>
	Ptr& operator<<(Ptr& logger, std::ostream& (*manip)(std::ostream&)) noexcept
		requires std::is_same_v<Ptr, std::shared_ptr<Logger>> || std::is_same_v<Ptr, std::unique_ptr<Logger>> {
		if (logger) {
			*logger << manip; // Forward manipulators like std::endl
		}
		return logger;
	}
}
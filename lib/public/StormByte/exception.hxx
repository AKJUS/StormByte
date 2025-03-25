#pragma once

#include <StormByte/visibility.h>
#include <string>
#include <format>

/**
 * @namespace StormByte
 * @brief Main namespace for the StormByte library and components
 */
namespace StormByte {
	/**
	 * @class Exception
	 * @brief Exception base class for all the StormByte library
	 * 
	 * Handles generic exceptions across library boundaries (Windows/Linux),
	 * using `const char*` for memory management to avoid `std::string` issues
	 * across DLL boundaries.
	 */
	class STORMBYTE_PUBLIC Exception {
	public:
		/**
		 * @brief Constructor
		 * @param message Exception message as a `std::string`
		 */
		explicit Exception(const std::string& message);

		/**
		 * @brief Constructor
		 * @param message Exception message as a moved `std::string`
		 */
		explicit Exception(std::string&& message);

		/**
		 * @brief Constructor forwards the message to the `std::format` function
		 * @tparam Args Format argument types
		 * @param fmt Format string
		 * @param args Arguments for formatting
		 * 
		 * If no arguments are provided, the format string is used as the exception message directly.
		 */
		template <typename... Args>
		Exception(std::format_string<Args...> fmt, Args&&... args) {
			if constexpr (sizeof...(Args) == 0) {
				// No arguments provided, use the format string directly
				m_what = copy_str(fmt);
			} else {
				// Format the message with the provided arguments
				std::string formatted_message = std::format(fmt, std::forward<Args>(args)...);
				m_what = copy_str(formatted_message.c_str());
			}
		}

		/**
		 * @brief Copy constructor
		 * @param e Exception to copy
		 */
		Exception(const Exception& e);

		/**
		 * @brief Move constructor
		 * @param e Exception to move
		 */
		Exception(Exception&& e) noexcept;

		/**
		 * @brief Destructor
		 */
		virtual ~Exception() noexcept;

		/**
		 * @brief Copy assignment operator
		 * @param e Exception to copy
		 * @return Reference to this exception
		 */
		Exception& operator=(const Exception& e);

		/**
		 * @brief Move assignment operator
		 * @param e Exception to move
		 * @return Reference to this exception
		 */
		Exception& operator=(Exception&& e) noexcept;

		/**
		 * @brief Gets the exception message
		 * @return Exception message as `const char*`
		 */
		virtual const char* 									what() const noexcept;

	private:
		const char* m_what; 									///< Exception message

		/**
		 * @brief Copies a string to internal storage
		 * @param str String to copy
		 * @return Copied string as `const char*`
		 */
		const char* 											copy_str(const char* str) noexcept;

		/**
		 * @brief Frees the allocated string from internal storage
		 */
		void 													free_str() noexcept;
	};

	/**
	 * @class BufferException
	 * @brief Exception class for buffer-related errors
	 * 
	 * Represents specific exceptions related to buffer issues in the StormByte library.
	 * Inherits all functionality from `Exception`.
	 */
	class STORMBYTE_PUBLIC BufferException: public Exception {
		public:
			/**
			 * Constructor
			 * @param reason Exception reason as `std::string`
			 */
			using Exception::Exception;
	};

	/**
	 * @class BufferOverflow
	 * @brief Exception class for buffer overflow errors
	 * 
	 * Represents exceptions specifically caused by buffer overflow scenarios.
	 * Inherits all functionality from `BufferException`.
	 */
	class STORMBYTE_PUBLIC BufferOverflow: public BufferException {
		public:
			/**
			 * Constructor
			 * @param reason Exception reason as `std::string`
			 */
			using BufferException::BufferException;
	};

	/**
	 * @class BufferNotReady
	 * @brief Exception class for buffer not ready errors
	 * 
	 * Represents exceptions specifically caused by buffer not being ready for operations.
	 * Inherits all functionality from `BufferException`.
	 */
	class STORMBYTE_PUBLIC BufferNotReady: public BufferException {
		public:
			/**
			 * Constructor
			 * @param reason Exception reason as `std::string`
			 */
			using BufferException::BufferException;
	};
}

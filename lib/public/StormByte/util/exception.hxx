#pragma once

#include <StormByte/exception.hxx>

/**
 * @namespace Util
 * @brief Contains utility classes and functions.
 */
namespace StormByte::Util {
	/**
	 * @class Exception
	 * @brief Exception base class for Util namespace classes
	 */
	class STORMBYTE_PUBLIC Exception:public StormByte::Exception {
		public:
			/**
			 * Constructor
			 * @param reason
			 */
			Exception(const std::string& reason);

			/**
			 * Constructor
			 * @param reason
			 */
			Exception(std::string&& reason);

			/**
			 * Copy constructor
			 */
			Exception(const Exception&)					= default;

			/**
			 * Move constructor
			 */
			Exception(Exception&&) noexcept				= default;

			/**
			 * Assignment operator
			 */
			Exception& operator=(const Exception&)		= default;

			/**
			 * Move assignment operator
			 */
			Exception& operator=(Exception&&) noexcept	= default;

			/**
			 * Destructor
			 */
			virtual ~Exception() noexcept override		= default;
	};

	/**
	 * @class SerializeError
	 * @brief Exception class for serialization errors
	 */
	class STORMBYTE_PUBLIC BufferException: public Exception {
		public:
			/**
			 * Constructor
			 * @param reason
			 */
			BufferException(const std::string& reason);

			/**
			 * Constructor
			 * @param reason
			 */
			BufferException(std::string&& reason);

			/**
			 * Copy constructor
			 * @param other
			 */
			BufferException(const BufferException& other)					= default;

			/**
			 * Move constructor
			 * @param other
			 */
			BufferException(BufferException&& other) noexcept				= default;

			/**
			 * Assignment operator
			 * @param other
			 * @return reference to this
			 */
			BufferException& operator=(const BufferException& other)		= default;

			/**
			 * Move assignment operator
			 * @param other
			 * @return reference to this
			 */
			BufferException& operator=(BufferException&& other) noexcept	= default;

			/**
			 * Destructor
			 */
			virtual ~BufferException() noexcept override					= default;
	};

	class STORMBYTE_PUBLIC BufferOverflow: public BufferException {
		public:
			/**
			 * Constructor
			 * @param reason
			 */
			BufferOverflow(const std::string& reason);

			/**
			 * Constructor
			 * @param reason
			 */
			BufferOverflow(std::string&& reason);

			/**
			 * Copy constructor
			 * @param other
			 */
			BufferOverflow(const BufferOverflow& other)					= default;

			/**
			 * Move constructor
			 * @param other
			 */
			BufferOverflow(BufferOverflow&& other) noexcept				= default;

			/**
			 * Assignment operator
			 * @param other
			 * @return reference to this
			 */
			BufferOverflow& operator=(const BufferOverflow& other)		= default;

			/**
			 * Move assignment operator
			 * @param other
			 * @return reference to this
			 */
			BufferOverflow& operator=(BufferOverflow&& other) noexcept	= default;

			/**
			 * Destructor
			 */
			~BufferOverflow() noexcept override							= default;
	};
}
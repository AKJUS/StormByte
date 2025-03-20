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
			virtual ~Exception() noexcept				= default;
	};

	/**
	 * @class SerializeError
	 * @brief Exception class for serialization errors
	 */
	class STORMBYTE_PUBLIC DeserializeError: public Exception {
		public:
			/**
			 * Constructor
			 * @param reason
			 */
			DeserializeError(const std::string& reason);

			/**
			 * Constructor
			 * @param reason
			 */
			DeserializeError(std::string&& reason);

			/**
			 * Copy constructor
			 * @param other
			 */
			DeserializeError(const DeserializeError& other)					= default;

			/**
			 * Move constructor
			 * @param other
			 */
			DeserializeError(DeserializeError&& other) noexcept				= default;

			/**
			 * Assignment operator
			 * @param other
			 * @return reference to this
			 */
			DeserializeError& operator=(const DeserializeError& other)		= default;

			/**
			 * Move assignment operator
			 * @param other
			 * @return reference to this
			 */
			DeserializeError& operator=(DeserializeError&& other) noexcept	= default;

			/**
			 * Destructor
			 */
			virtual ~DeserializeError() noexcept							= default;
	};
}
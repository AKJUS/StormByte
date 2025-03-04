#pragma once

#include <exception.hxx>

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
	 * @class ValueTypeMissmatch
	 * @brief Exception thrown when value type requested is not correct
	 */
	class STORMBYTE_PUBLIC ValueTypeMissmatch: public Exception {
		public:
			/**
			 * Constructor
			 * @param targettype
			 */
			ValueTypeMissmatch(const std::string& targettype);

			/**
			 * Copy constructor
			 */
			ValueTypeMissmatch(const ValueTypeMissmatch&)					= default;

			/**
			 * Move constructor
			 */
			ValueTypeMissmatch(ValueTypeMissmatch&&) noexcept				= default;

			/**
			 * Assignment operator
			 */
			ValueTypeMissmatch& operator=(const ValueTypeMissmatch&)		= default;

			/**
			 * Move operator
			 */
			ValueTypeMissmatch& operator=(ValueTypeMissmatch&&) noexcept 	= default;

			/**
			 * Destructor
			 */
			virtual ~ValueTypeMissmatch() noexcept override					= default;
	};
}
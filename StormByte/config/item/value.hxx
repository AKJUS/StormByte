#pragma once

#include <StormByte/config/item.hxx>

/**
 * @namespace StormByte::Config
 * @brief All the classes for handling configuration files and items
 */
namespace StormByte::Config {
	/**
	 * @class Value
	 * @brief Abstract class to represent a configuration item with value
	 */
	class STORMBYTE_PUBLIC Value: public Item {
		public:
			/**
			 * Constructor
			 * @param type item Type
			 * @param name item name
			 */
			Value(const Type& type, const std::string& name);
			/**
			 * Constructor
			 * @param type item Type
			 * @param name item name
			 */
			Value(const Type& type, std::string&& name);
			/**
			 * Copy constructor
			 */
			Value(const Value&)					= default;
			/**
			 * Move constructor
			 */
			Value(Value&&) noexcept				= default;
			/**
			 * Assignment operator
			 */
			Value& operator=(const Value&)		= default;
			/**
			 * Move assignment operator
			 */
			Value& operator=(Value&&) noexcept	= default;
			/**
			 * Destructor
			 */
			~Value() noexcept override			= default;
	};
}
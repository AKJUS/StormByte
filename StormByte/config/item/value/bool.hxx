#pragma once

#include <StormByte/config/item/value.hxx>

#include <variant>

/**
 * @namespace StormByte::Config
 * @brief All the classes for handling configuration files and items
 */
namespace StormByte::Config {
	/**
	 * @class Bool
	 * @brief A boolean configuration item
	 * @code
	 * bool_item = false;
	 * @endcode
	 */
	class STORMBYTE_PUBLIC Bool final: public Value {
		public:
			/**
			 * Constructor
			 * @param name item name
			 */
			Bool(const std::string& name);
			/**
			 * Constructor
			 * @param name item name
			 */
			Bool(std::string&& name);
			/**
			 * Copy constructor
			 */
			Bool(const Bool&)					= default;
			/**
			 * Move constructor
			 */
			Bool(Bool&&) noexcept				= default;
			/**
			 * Assignment operator
			 */
			Bool& operator=(const Bool&)		= default;
			/**
			 * Move assignment operator
			 */
			Bool& operator=(Bool&&) noexcept	= default;
			/**
			 * Destructor
			 */
			~Bool() noexcept override			= default;

			/**
			 * Value getter
			 * @return boolean value
			 */
			bool 					AsBool() const override;

			/**
			 * Sets the boolean item value
			 * @param value boolean value
			 */
			void					SetBool(bool value) override;

			/**
			 * Serializes the boolean item
			 * @param indent_level intentation level
			 * @return serialized string
			 */
			std::string				Serialize(const int& indent_level) const noexcept override;

		private:
			/**
			 * Clones this object
			 * @return a shared pointer for this item
			 */
			std::shared_ptr<Item>	Clone() override;

			/**
			 * Current item's value
			 */
			bool m_value;
	};
}
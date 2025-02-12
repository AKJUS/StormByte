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
		/**
	 	 * @brief representing a boolean in configuration files (true or false in lowercase)
	 	 * @example bool_item = false;
	 	 */
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
			 * @return serialized string
			 */
			std::string				Serialize(const int& indent) const noexcept override;

		private:
			std::shared_ptr<Item>	Clone() override;

			bool m_value;
	};
}
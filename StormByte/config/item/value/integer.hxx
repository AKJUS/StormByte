#pragma once

#include <StormByte/config/item/value.hxx>

#include <variant>

/**
 * @namespace StormByte::Config
 * @brief All the classes for handling configuration files and items
 */
namespace StormByte::Config {
	/**
	 * @class Integer
	 * @brief Integer configuration item
	 * @code
	 * test_int = 8;
	 * @endcode
	 */
	class STORMBYTE_PUBLIC Integer final: public Value {
		public:
			/**
			 * Constructor
			 * @param name item name
			 */
			Integer(const std::string& name);
			/**
			 * Constructor
			 * @param name item name
			 */
			Integer(std::string&& name);
			/**
			 * Copy constructor
			 */
			Integer(const Integer&)					= default;
			/**
			 * Move constructor
			 */
			Integer(Integer&&) noexcept				= default;
			/**
			 * Assignment operator
			 */
			Integer& operator=(const Integer&)		= default;
			/**
			 * Move assignment operator
			 */
			Integer& operator=(Integer&&) noexcept	= default;
			/**
			 * Destructor
			 */
			~Integer() noexcept override			= default;

			/**
			 * Value getter
			 * @return integer value
			 */
			const int& 				AsInteger() const override;

			/**
			 * Value setter
			 * @param value value to set
			 */
			void					SetInteger(const int& value) override;

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
			int m_value;
	};
}
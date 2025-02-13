#pragma once

#include <StormByte/config/item/value.hxx>

#include <variant>

/**
 * @namespace StormByte::Config
 * @brief All the classes for handling configuration files and items
 */
namespace StormByte::Config {
	/**
	 * @class String
	 * @brief String configuration item
	 * @code
	 * test_str = "Hello, World!";
	 * @endcode
	 */
	class STORMBYTE_PUBLIC String final: public Value {
		public:
			/**
			 * Constructor
			 * @param name item name
			 */
			String(const std::string& name);
			/**
			 * Constructor
			 * @param name item name
			 */
			String(std::string&& name);
			/**
			 * Copy constructor
			 */
			String(const String&)					= default;
			/**
			 * Move constructor
			 */
			String(String&&) noexcept				= default;
			/**
			 * Assignment operator
			 */
			String& operator=(const String&)		= default;
			/**
			 * Move assignment operator
			 */
			String& operator=(String&&) noexcept	= default;
			/**
			 * Destructor
			 */
			~String() noexcept override				= default;

			/**
			 * Value getter
			 * @return string value
			 */
			const std::string& 		AsString() const override;

			/**
			 * Value setter
			 * @param value value to set
			 */
			void					SetString(const std::string& value) override;
			/**
			 * Value setter
			 * @param value value to move to this element
			 */
			void					SetString(std::string&& value) override;

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
			std::string m_value;
	};
}
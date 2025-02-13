#pragma once

#include <StormByte/config/item/value.hxx>

#include <variant>

/**
 * @namespace StormByte::Config
 * @brief All the classes for handling configuration files and items
 */
namespace StormByte::Config {
	/**
	 * @class Double
	 * @brief Double configuration item
	 * @code
	 * test_double1 = 78.9;
	 * test_double2 = 6.9e+3;
	 * @endcode 
	 */
	class STORMBYTE_PUBLIC Double final: public Value {
		public:
			/**
			 * Constructor
			 * @param name item name
			 */
			Double(const std::string& name);

			/**
			 * Constructor
			 * @param name item name
			 */
			Double(std::string&& name);

			/**
			 * Copy constructor
			 */
			Double(const Double&)					= default;

			/**
			 * Move constructor
			 */
			Double(Double&&) noexcept				= default;

			/**
			 * Assignment operator
			 */
			Double& operator=(const Double&)		= default;

			/**
			 * Move assignment operator
			 */
			Double& operator=(Double&&) noexcept	= default;

			/**
			 * Destructor
			 */
			~Double() noexcept override				= default;

			/**
			 * Value getter
			 * @return double value
			 */
			constexpr const double& AsDouble() const override {
				return m_value;
			}

			/**
			 * Value setter
			 * @param value double value to set
			 */
			inline void				SetDouble(const double& value) override {
				m_value = value;
			}

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
			double m_value;
	};
}
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
	 * test_double2 = 6.9e.3;
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
			const double& 			AsDouble() const override;

			/**
			 * Value setter
			 * @param value double value to set
			 */
			void					SetDouble(const double& value) override;

			/**
			 * Serializes the double item
			 * @return serialized string
			 */
			std::string				Serialize(const int&) const noexcept override;

		private:
			std::shared_ptr<Item>	Clone() override;

			double m_value;
	};
}
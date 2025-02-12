#pragma once

#include <StormByte/config/item/value.hxx>

#include <variant>

namespace StormByte::Config {
	/**
	 * Class representing a boolean in configuration files (true or false in lowercase)
	 */
	class STORMBYTE_PUBLIC Bool final: public Value {
		public:
			Bool(const std::string&);
			Bool(std::string&&);
			Bool(const Bool&)					= default;
			Bool(Bool&&) noexcept				= default;
			Bool& operator=(const Bool&)		= default;
			Bool& operator=(Bool&&) noexcept	= default;
			~Bool() noexcept override			= default;

			/**
			 * Gets the boolean item value
			 * @return boolean value
			 */
			bool 					AsBool() const override;

			/**
			 * Sets the boolean item value
			 * @param value boolean value
			 */
			void					SetBool(bool value) override;

			/**
			 * Serializes the boolean item:
			 * @return Serialized string, for example: item = false;
			 */
			std::string				Serialize(const int& indent) const noexcept override;

		private:
			std::shared_ptr<Item>	Clone() override;

			bool m_value;
	};
}
#pragma once

#include <StormByte/config/item/value.hxx>

#include <variant>

namespace StormByte::Config {
	class STORMBYTE_PUBLIC Bool final: public Value {
		public:
			Bool(const std::string&);
			Bool(std::string&&);
			Bool(const Bool&)					= default;
			Bool(Bool&&) noexcept				= default;
			Bool& operator=(const Bool&)		= default;
			Bool& operator=(Bool&&) noexcept	= default;
			~Bool() noexcept override			= default;

			bool 					AsBool() const override;

			void					SetBool(bool) override;

			std::string				Serialize(const int&) const noexcept override;

		private:
			std::shared_ptr<Item>	Clone() override;

			bool m_value;
	};
}
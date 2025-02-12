#pragma once

#include <StormByte/config/item/value.hxx>

#include <variant>

namespace StormByte::Config {
	class STORMBYTE_PUBLIC Integer final: public Value {
		public:
			Integer(const std::string&);
			Integer(std::string&&);
			Integer(const Integer&)					= default;
			Integer(Integer&&) noexcept				= default;
			Integer& operator=(const Integer&)		= default;
			Integer& operator=(Integer&&) noexcept	= default;
			~Integer() noexcept override			= default;

			const int& 				AsInteger() const override;

			void					SetInteger(const int&) override;

			std::string				Serialize(const int&) const noexcept override;

		private:
			std::shared_ptr<Item>	Clone() override;

			int m_value;
	};
}
#pragma once

#include <StormByte/config/item/value.hxx>

#include <variant>

namespace StormByte::Config {
	class STORMBYTE_PUBLIC String final: public Value {
		public:
			String(const std::string&);
			String(std::string&&);
			String(const String&)					= default;
			String(String&&) noexcept				= default;
			String& operator=(const String&)		= default;
			String& operator=(String&&) noexcept	= default;
			~String() noexcept override				= default;

			const std::string& 		AsString() const override;

			void					SetString(const std::string&) override;
			void					SetString(std::string&&) override;

			std::string				Serialize(const int&) const noexcept override;

		private:
			std::shared_ptr<Item>	Clone() override;

			std::string m_value;
	};
}
#pragma once

#include <StormByte/config/item/value.hxx>

#include <variant>

namespace StormByte::Config {
	class STORMBYTE_PUBLIC Double final: public Value {
		public:
			Double(const std::string&);
			Double(std::string&&);
			Double(const Double&)					= default;
			Double(Double&&) noexcept				= default;
			Double& operator=(const Double&)		= default;
			Double& operator=(Double&&) noexcept	= default;
			~Double() noexcept override				= default;

			const int& 				AsInteger() const override;
			const double& 			AsDouble() const override;
			const std::string& 		AsString() const override;

			void					SetInteger(const int&) override;
			void					SetDouble(const double&) override;
			void					SetString(const std::string&) override;
			void					SetString(std::string&&) override;

			std::string				Serialize(const int&) const noexcept override;

		private:
			std::shared_ptr<Item>	Clone() override;

			double m_value;
	};
}
#pragma once

#include <StormByte/config/item.hxx>

namespace StormByte::Config {
	class STORMBYTE_PUBLIC Comment final: public Item {
		public:
			Comment();
			Comment(const Comment&)					= default;
			Comment(Comment&&) noexcept				= default;
			Comment& operator=(const Comment&)		= default;
			Comment& operator=(Comment&&) noexcept	= default;
			~Comment() noexcept override			= default;

			Group&						AsGroup() override;
			const int&					AsInteger() const override;
			const double&				AsDouble() const override;
			const std::string&			AsString() const override;
			bool						AsBool() const override;

			void						SetInteger(const int&) override;
			void						SetDouble(const double&) override;
			void						SetString(const std::string&) override;
			void						SetString(std::string&&) override;
			void						SetBool(bool) override;

			std::shared_ptr<Item>		Clone() override;
			std::string					Serialize(const int& indent_level = 0) const noexcept override;

		private:
			std::string m_value;
	};
}
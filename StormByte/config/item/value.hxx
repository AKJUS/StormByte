#pragma once

#include <StormByte/config/item.hxx>

namespace StormByte::Config {
	class STORMBYTE_PUBLIC Value: public Item {
		public:
			Value(const Type&, const std::string&);
			Value(const Type&, std::string&&);
			Value(const Value&)					= default;
			Value(Value&&) noexcept				= default;
			Value& operator=(const Value&)		= default;
			Value& operator=(Value&&) noexcept	= default;
			~Value() noexcept override			= default;

			Group&				 		AsGroup() override;
			virtual const int& 			AsInteger() const override;
			virtual const double& 		AsDouble() const override;
			virtual const std::string& 	AsString() const override;
			virtual bool 				AsBool() const override;

			virtual void				SetInteger(const int&) override;
			virtual void				SetDouble(const double&) override;
			virtual void				SetString(const std::string&) override;
			virtual void				SetString(std::string&&) override;
			virtual void				SetBool(bool) override;
	};
}
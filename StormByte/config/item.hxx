#pragma once

#include <StormByte/visibility.h>

#include <memory>
#include <string>
#include <vector>

namespace StormByte::Config {
	class Group;
	class STORMBYTE_PUBLIC Item {
		friend class File;
		public:
			enum class Type: unsigned short {
				Group = 0,
				String,
				Integer,
				Double,
				Comment
			};

			Item(const Item&)					= default;
			Item(Item&&) noexcept				= default;
			Item& operator=(const Item&)		= default;
			Item& operator=(Item&&) noexcept	= default;
			virtual ~Item() noexcept			= default;

			const std::string&					GetName() const noexcept;
			const Type&							GetType() const noexcept;
			const std::string					GetTypeAsString() const noexcept;
			static const std::string			GetTypeAsString(const Type&) noexcept;
			
			virtual Group&						AsGroup()			= 0;
			virtual const int&					AsInteger() const 	= 0;
			virtual const double&				AsDouble() const 	= 0;
			virtual const std::string&			AsString() const	= 0;

			virtual void						SetInteger(const int&)			= 0;
			virtual void						SetDouble(const double&)		= 0;
			virtual void						SetString(const std::string&)	= 0;
			virtual void						SetString(std::string&&)		= 0;

			virtual std::shared_ptr<Item>		Clone() = 0;
			virtual std::string					Serialize(const int& indent_level = 0) const noexcept = 0;
		
		protected:
			Item(const Type&, const std::string&);
			Item(const Type&, std::string&&);
			std::string							Indent(const int&) const noexcept;

			std::string m_name;
			Type m_type;

		private:
			
	};
}
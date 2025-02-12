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
				Comment,
				Bool
			};
			static constexpr const char* GetTypeAsString(const Type& t) noexcept {
				switch(t) {
					case Type::Group:  	return "Group";
					case Type::String: 	return "String";
					case Type::Integer:	return "Integer";
					case Type::Double: 	return "Double";
					case Type::Comment:	return "Comment";
					case Type::Bool:	return "Bool";
					default:			return "Unknown";
				}
			}

			Item(const Item&)					= default;
			Item(Item&&) noexcept				= default;
			Item& operator=(const Item&)		= default;
			Item& operator=(Item&&) noexcept	= default;
			virtual ~Item() noexcept			= default;

			const std::string&					GetName() const noexcept;
			const Type&							GetType() const noexcept;
			const std::string					GetTypeAsString() const noexcept;
			
			virtual Group&						AsGroup()			= 0;
			virtual const int&					AsInteger() const 	= 0;
			virtual const double&				AsDouble() const 	= 0;
			virtual const std::string&			AsString() const	= 0;
			virtual bool 						AsBool() const 		= 0;

			virtual void						SetInteger(const int&)			= 0;
			virtual void						SetDouble(const double&)		= 0;
			virtual void						SetString(const std::string&)	= 0;
			virtual void						SetString(std::string&&)		= 0;
			virtual void 						SetBool(bool)					= 0;

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
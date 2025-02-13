#pragma once

#include <StormByte/visibility.h>

#include <memory>
#include <string>
#include <vector>

/**
 * @namespace StormByte::Config
 * @brief All the classes for handling configuration files and items
 */
namespace StormByte::Config {
	class Group; // Forward decl
	/**
	 * @class Item
	 * @brief Abstract class for a generic configuration item
	 */
	class STORMBYTE_PUBLIC Item {
		friend class File;
		friend class Group;
		public:
			/**
			 * @enum Type
			 * @brief Item type
			 */
			enum class Type: unsigned short {
				Group = 0,
				String,
				Integer,
				Double,
				Comment,
				Bool
			};
			/**
			 * Gets strings from Type
			 * @return string
			 */
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

			/**
			 * Copy constructor
			 */
			Item(const Item&)					= default;
			/**
			 * Move constructor
			 */
			Item(Item&&) noexcept				= default;
			/**
			 * Assignment operator
			 */
			Item& operator=(const Item&)		= default;
			/**
			 * Move operator
			 */
			Item& operator=(Item&&) noexcept	= default;
			/**
			 * Destructor
			 */
			virtual ~Item() noexcept			= default;

			/**
			 * Gets item name
			 * @return item name
			 */
			const std::string&					GetName() const noexcept;
			/**
			 * Gets item type
			 * @return item Type
			 */
			const Type&							GetType() const noexcept;
			/**
			 * Gets item type as string
			 * @return item type string
			 */
			constexpr std::string				GetTypeAsString() const noexcept {
				return GetTypeAsString(m_type);
			}
			
			/**
			 * Will throw exception since item is not a group
			 * @throw WrongValueTypeConversion
			 */
			virtual Group&						AsGroup();
			/**
			 * Will throw exception since item is not an integer
			 * @throw WrongValueTypeConversion
			 */
			virtual const int&					AsInteger() const;
			/**
			 * Will throw exception since item is not a double
			 * @throw WrongValueTypeConversion
			 */
			virtual const double&				AsDouble() const;
			/**
			 * Will throw exception since item is not a string
			 * @throw WrongValueTypeConversion
			 */
			virtual const std::string&			AsString() const;
			/**
			 * Will throw exception since item is not a bool
			 * @throw WrongValueTypeConversion
			 */
			virtual bool 						AsBool() const;

			/**
			 * Will throw exception since item is not an integer
			 * @throw ValueFailure
			 */
			virtual void						SetInteger(const int&);
			/**
			 * Will throw exception since item is not a double
			 * @throw ValueFailure
			 */
			virtual void						SetDouble(const double&);
			/**
			 * Will throw exception since item is not a string
			 * @throw ValueFailure
			 */
			virtual void						SetString(const std::string&);
			/**
			 * Will throw exception since item is not a string
			 * @throw ValueFailure
			 */
			virtual void						SetString(std::string&&);
			/**
			 * Will throw exception since item is not a bool
			 * @throw ValueFailure
			 */
			virtual void 						SetBool(bool);

			/**
			 * Serializes the boolean item
			 * @param indent_level intentation level
			 * @return serialized string
			 */
			virtual std::string					Serialize(const int& indent_level) const noexcept = 0;
		
		protected:
			/**
			 * Constructor
			 * @param type item Type
			 * @param name item name
			 */
			Item(const Type& type, const std::string& name);
			/**
			 * Constructor
			 * @param type item Type
			 * @param name item name
			 */
			Item(const Type& type, std::string&& name);
			/**
			 * Clones this object (must be implemented for derived classes)
			 * @return a shared pointer for the base class
			 */
			virtual std::shared_ptr<Item>		Clone() = 0;
			/**
			 * Indents the output
			 * @param level
			 */
			std::string							Indent(const int& level) const noexcept;

			/**
			 * Item name
			 */
			std::string m_name;
			/**
			 * Item type
			 */
			Type m_type;
	};
}
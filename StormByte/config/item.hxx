#pragma once

#include <StormByte/config/group.hxx>

#include <algorithm>
#include <memory>
#include <string>
#include <variant>
#include <vector>

/**
 * @namespace StormByte::Config
 * @brief All the classes for handling configuration files and items
 */
namespace StormByte::Config {
	/**
	 * @class Item
	 * @brief Class for a configuration item
	 */
	class STORMBYTE_PUBLIC Item final {
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
			 * Static function to check if a name is valid (only contains numbers, letters and underscore)
			 * @param name name to check
			 * @return bool valid?
			 */
			static constexpr bool 				IsNameValid(const std::string& name) noexcept {
				// Find unallowed chars and if not found then name is valid
				return !(std::find_if(name.begin(), name.end(), [](char c) { return !isalnum(c) && c != '_'; }) != name.end());
			}

			/**
			 * Creates an item with a group value
			 * @param name item name
			 * @param value item value
			 */
			Item(const std::string& name, const Group& value);
			/**
			 * Creates an item moving the group value
			 * @param name item name
			 * @param value item value
			 */
			Item(const std::string& name, Group&& value);
			/**
			 * Creates an item with a string value
			 * @param name item name
			 * @param value item value
			 */
			Item(const std::string& name, const std::string& value);
			/**
			 * Creates an item with a string value
			 * @param name item name
			 * @param value item value
			 */
			Item(const char* name, const char* value);
			/**
			 * Creates an item with a int value
			 * @param name item name
			 * @param value item value
			 */
			Item(const std::string& name, const int& value);
			/**
			 * Creates an item with a double value
			 * @param name item name
			 * @param value item value
			 */
			explicit Item(const std::string& name, const double& value);
			/**
			 * Creates an item with a comment value
			 * @param value item value
			 */
			Item(const std::string& value);
			/**
			 * Creates an item with a bool value
			 * @param name item name
			 * @param value item value
			 */
			explicit Item(const std::string& name, bool value);
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
			constexpr const std::string&		GetName() const noexcept {
				return m_name;
			}
			/**
			 * Gets item type
			 * @return item Type
			 */
			constexpr const Type&				GetType() const noexcept {
				return m_type;
			}
			/**
			 * Gets item type as string
			 * @return item type string
			 */
			constexpr const std::string			GetTypeAsString() const noexcept {
				return GetTypeAsString(m_type);
			}
			
			/**
			 * Value getter as reference
			 */
			template<typename T> T&				Value();
			/**
			 * Value getter as const reference
			 */
			template<typename T> const T&		Value() const;

			#ifdef MSVC
			template<> Group&					Value<Group>();
			template<> const Group&				Value<Group>() const;
			template<> std::string&				Value<std::string>();
			template<> const std::string&		Value<std::string>() const;
			template<> int&						Value<int>();
			template<> const int&				Value<int>() const;
			template<> double&					Value<double>();
			template<> const double&			Value<double>() const;
			template<> bool&					Value<bool>();
			template<> const bool&				Value<bool>() const;
			#endif

			/**
			 * Serializes the boolean item
			 * @param indent_level intentation level
			 * @return serialized string
			 */
			virtual std::string					Serialize(const int& indent_level) const noexcept;
		
		private:
			/**
			 * Item name
			 */
			std::string m_name;
			/**
			 * Item type
			 */
			Type m_type;
			/**
			 * Internal value
			 */
			std::variant<std::string, int, double, bool, Group> m_value;

			/**
			 * Add indentation
			 * @param level
			 */
			constexpr std::string					Indent(const int& level) const noexcept {
				return level == 0 ? std::string() : std::string(level, '\t');
			}
	};
}
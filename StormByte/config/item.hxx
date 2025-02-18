#pragma once

#include <StormByte/config/exception.hxx>

#include <memory>
#include <optional>
#include <variant>

/**
 * @namespace StormByte::Config
 * @brief All the classes for handling configuration files and items
 */
namespace StormByte::Config {
	class Group; // Needed for MSVC template specializations
	class List; // Needed for MSVC template specializations
	class Container;
	/**
	 * @class Item
	 * @brief Class for a configuration item
	 */
	class STORMBYTE_PUBLIC Item {
		friend class Container; // for AddComment
		/**
		 * Shortcut alias for internal storage
		 */
		using ItemStorage = std::variant<std::string, int, double, bool, std::shared_ptr<Container>>;

		public:
			/**
			 * @enum Type
			 * @brief Item type
			 */
			enum class Type: unsigned short {
				String,
				Integer,
				Double,
				Comment,
				Bool,
				Container
			};
			/**
			 * Gets strings from Type
			 * @return string
			 */
			static constexpr const char* TypeAsString(const Type& t) noexcept {
				switch(t) {
					case Type::String: 		return "String";
					case Type::Integer:		return "Integer";
					case Type::Double: 		return "Double";
					case Type::Comment:		return "Comment";
					case Type::Bool:		return "Bool";
					case Type::Container:	return "Container";
					default:				return "Unknown";
				}
			}
			/**
			 * Constructor for an empty item is not allowed
			 */
			Item()										= delete;

			/**
			 * Creates an item with a container value
			 * @param name item name
			 * @param value container value
			 */
			Item(const std::string& name, const Container& value);

			/**
			 * Creates an item with a group value
			 * @param value item value
			 */
			Item(const Container& value);

			/**
			 * Creates an item moving the group value
			 * @param name item name
			 * @param value item value
			 */
			Item(std::string&& name, Container&& value);

			/**
			 * Creates an item moving the group value
			 * @param value item value
			 */
			Item(Container&& value);

			/**
			 * Creates an item with a string value
			 * @param name item name
			 * @param value item value
			 */
			Item(const std::string& name, const std::string& value);

			/**
			 * Creates an item with a string value
			 * @param value item value
			 */
			Item(const std::string& value);

			/**
			 * Creates an item with a string value
			 * @param name item name
			 * @param value item value
			 */
			Item(std::string&& name, std::string&& value);

			/**
			 * Creates an item with a string value
			 * @param value item value
			 */
			Item(std::string&& value);

			/**
			 * Creates an item with a string value
			 * @param name item name
			 * @param value item value
			 */
			Item(const char* name, const char* value);

			/**
			 * Creates an item with a string value
			 * @param value item value
			 */
			Item(const char* value);

			/**
			 * Creates an item with a int value
			 * @param name item name
			 * @param value item value
			 */
			Item(const std::string& name, const int& value);

			/**
			 * Creates an item with a int value
			 * @param value item value
			 */
			Item(const int& value);

			/**
			 * Creates an item with a double value
			 * @param name item name
			 * @param value item value
			 */
			Item(const std::string& name, const double& value);

			/**
			 * Creates an item with a double value
			 * @param value item value
			 */
			explicit Item(const double& value);

			/**
			 * Creates an item with a bool value
			 * @param name item name
			 * @param value item value
			 */
			Item(const std::string& name, bool value);

			/**
			 * Creates an item with a bool value
			 * @param value item value
			 */
			explicit Item(bool value);

			/**
			 * Copy constructor
			 */
			Item(const Item&);

			/**
			 * Move constructor
			 */
			Item(Item&&) noexcept						= default;

			/**
			 * Assignment operator
			 */
			Item& operator=(const Item&);

			/**
			 * Move operator
			 */
			Item& operator=(Item&&) noexcept			= default;

			/**
			 * Destructor
			 */
			virtual ~Item() noexcept					= default;

			/**
			 * Equality operator
			 * @param other item to compare
			 * @return bool equal?
			 */
			bool 										operator==(const Item& other) const noexcept;

			/**
			 * Inequality operator
			 * @param other item to compare
			 * @return bool not equal?
			 */
			inline bool 								operator!=(const Item& other) const noexcept {
				return !(*this == other);
			}

			/**
			 * Static function to check if a name is valid (only contains numbers, letters and underscore)
			 * @param name name to check
			 * @return bool valid?
			 */
			static bool 								IsNameValid(const std::string& name) noexcept;
			
			/**
			 * Static function to check if a path is valid (only contains numbers, letters, underscore and slashes)
			 * @param path path to check
			 * @return bool valid?
			 */
			static bool									IsPathValid(const std::string& path) noexcept;

			/**
			 * Gets item name
			 * @return item name
			 */
			constexpr const std::optional<std::string>&	Name() const {
				return m_name;
			}

			/**
			 * Gets item name
			 * @return item name
			 */
			constexpr std::optional<std::string>&		Name() {
				return m_name;
			}

			/**
			 * Gets item type
			 * @return item Type
			 */
			constexpr const Type&						GetType() const noexcept {
				return m_type;
			}

			/**
			 * Gets item type as string
			 * @return item type string
			 */
			constexpr const std::string					TypeAsString() const noexcept {
				return TypeAsString(m_type);
			}
			
			/**
			 * Value getter as reference
			 * @return value reference
			 */
			template<typename T> T&						Value();

			/**
			 * Value getter as const reference
			 * @return value const reference
			 */
			template<typename T> const T&				Value() const;

			#ifdef MSVC
			/**
			 * Value getter as reference
			 * @return value reference
			 */
			template<> Group&							Value<Group>();

			/**
			 * Value getter as const reference
			 * @return value const reference
			 */
			template<> const Group&						Value<Group>() const;

			/**
			 * Value getter as reference
			 * @return value reference
			 */
			template<> List&							Value<List>();

			/**
			 * Value getter as const reference
			 * @return value const reference
			 */
			template<> const List&						Value<List>() const;

			/**
			 * Value getter as reference
			 * @return value reference
			 */
			template<> Container&						Value<Container>();

			/**
			 * Value getter as const reference
			 * @return value const reference
			 */
			template<> const Container&					Value<Container>() const;
			/**
			 * Value getter as reference
			 * @return value reference
			 */
			template<> std::string&						Value<std::string>();

			/**
			 * Value getter as const reference
			 * @return value const reference
			 */
			template<> const std::string&				Value<std::string>() const;

			/**
			 * Value getter as reference
			 * @return value reference
			 */
			template<> int&								Value<int>();

			/**
			 * Value getter as const reference
			 * @return value const reference
			 */
			template<> const int&						Value<int>() const;

			/**
			 * Value getter as reference
			 * @return value reference
			 */
			template<> double&							Value<double>();

			/**
			 * Value getter as const reference
			 * @return value const reference
			 */
			template<> const double&					Value<double>() const;

			/**
			 * Value getter as reference
			 * @return value reference
			 */
			template<> bool&							Value<bool>();

			/**
			 * Value getter as const reference
			 * @return value const reference
			 */
			template<> const bool&						Value<bool>() const;
			#endif

			/**
			 * Serializes the boolean item
			 * @param indent_level intentation level
			 * @return serialized string
			 */
			virtual inline std::string					Serialize(const int& indent_level) const noexcept {
				return ContentsToString(indent_level);
			}

		protected:
			/**
			 * Item optional name
			 */
			std::optional<std::string>					m_name;

			/**
			 * Item type
			 */
			Type										m_type;

			/**
			 * Internal value (it is a pointer to avoid cyclic references Item->Group->Item or Item->List->Item so it is enough with a forward declarations)
			 */
			std::unique_ptr<ItemStorage>				m_value;

			/**
			 * Internal function to get item contents as string
			 * @return item contents as std::string
			 */
			std::string 								ContentsToString(const int& level) const noexcept;

			/**
			 * Internal function to get "name = " string
			 */
			constexpr std::string 						NameEqualSignString() const noexcept {
				std::string result = "";
				if (m_name)
					result += *m_name + " = ";
				return result;
			}
	};
}
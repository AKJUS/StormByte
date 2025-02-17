#pragma once

#include <StormByte/config/item.hxx>

#include <algorithm>

/**
 * @namespace StormByte::Config
 * @brief All the classes for handling configuration files and items
 */
namespace StormByte::Config {
	/**
	 * @class NamedItem
	 * @brief Class for a configuration item
	 */
	class STORMBYTE_PUBLIC NamedItem final: public Item {
		friend class Config;
		public:
			/**
			 * Creates an item with a group value
			 * @param name item name
			 * @param value item value
			 */
			NamedItem(const std::string& name, const Group& value);
			/**
			 * Creates an item with a list value
			 * @param name item name
			 * @param value item value
			 */
			NamedItem(const std::string& name, const List& value);
			/**
			 * Creates an item moving the group value
			 * @param name item name
			 * @param value item value
			 */
			NamedItem(const std::string& name, Group&& value);
			/**
			 * Creates an item with moving a list value
			 * @param name item name
			 * @param value item value
			 */
			NamedItem(const std::string& name, List&& value);
			/**
			 * Creates an item with a string value
			 * @param name item name
			 * @param value item value
			 */
			NamedItem(const std::string& name, const std::string& value);
			/**
			 * Creates an item with a string value
			 * @param name item name
			 * @param value item value
			 */
			NamedItem(const char* name, const char* value);
			/**
			 * Creates an item with a int value
			 * @param name item name
			 * @param value item value
			 */
			NamedItem(const std::string& name, const int& value);
			/**
			 * Creates an item with a double value
			 * @param name item name
			 * @param value item value
			 */
			explicit NamedItem(const std::string& name, const double& value);
			/**
			 * Creates an item with a comment value
			 * @param value item value
			 */
			NamedItem(const std::string& value);
			/**
			 * Creates an item with a bool value
			 * @param name item name
			 * @param value item value
			 */
			explicit NamedItem(const std::string& name, bool value);
			/**
			 * Copy constructor
			 */
			NamedItem(const NamedItem&)					= default;
			/**
			 * Move constructor
			 */
			NamedItem(NamedItem&&) noexcept				= default;
			/**
			 * Assignment operator
			 */
			NamedItem& operator=(const NamedItem&)		= default;
			/**
			 * Move operator
			 */
			NamedItem& operator=(NamedItem&&) noexcept	= default;
			/**
			 * Destructor
			 */
			virtual ~NamedItem() noexcept			= default;

			/**
			 * Gets item name
			 * @return item name
			 */
			constexpr const std::string&		GetName() const noexcept {
				return m_name;
			}

			/**
			 * Static function to check if a name is valid (only contains numbers, letters and underscore)
			 * @param name name to check
			 * @return bool valid?
			 */
			static bool 						IsNameValid(const std::string& name) noexcept;
			
			/**
			 * Static function to check if a path is valid (only contains numbers, letters, underscore and slashes)
			 * @param path path to check
			 * @return bool valid?
			 */
			static bool							IsPathValid(const std::string& path) noexcept;

			/**
			 * Serializes the boolean item
			 * @param indent_level intentation level
			 * @return serialized string
			 */
			std::string							Serialize(const int& indent_level) const noexcept override;
		
		private:
			/**
			 * NamedItem name
			 */
			std::string m_name;
	};
}
#pragma once

#include <StormByte/config/exception.hxx>
#include <StormByte/config/group.hxx>
#include <StormByte/config/list.hxx>

#include <functional>
#include <memory>

/**
 * @namespace StormByte::Config
 * @brief All the classes for handling configuration files and items
 */
namespace StormByte::Config {
	/**
	 * @class Config
	 * @brief Abstract class for a configuration file
	 * 
	 * The configuration specs are defined by its elements, have no "root" element and can contain any number of the below:
	 * @see Bool
	 * @see Double
	 * @see Integer
	 * @see String
	 * @see Comment
	 * @see Group
	 */
	class STORMBYTE_PUBLIC Config {
		public:
			/**
			 * Constructor
			 */
			Config();
			/**
			 * Copy constructor
			 */
			Config(const Config&);
			/**
			 * Move constructor
			 */
			Config(Config&&) noexcept;
			/**
			 * Assignment operator
			 */
			Config& operator=(const Config&);
			/**
			 * Move assignment operator
			 */
			Config& operator=(Config&&) noexcept;
			/**
			 * Destructor
			 */
			virtual ~Config()						= default;

			/**
			 * Adds a copy of an item
			 * @param item item to add
			 * @throw InvalidName thrown when item name is not allowed
			 * @return a reference to the added item
			 */
			inline NamedItem&						Add(const NamedItem& item) {
				return m_root.Add(item, m_on_name_clash_action);
			}
			/**
			 * Moves an item to configuration
			 * @param item item to add
			 * @throw InvalidName thrown when item name is not allowed
			 * @return a reference to the added item
			 */
			inline NamedItem&						Add(NamedItem&& item) {
				return m_root.Add(std::move(item), m_on_name_clash_action);
			}
			/**
			 * Removes an item from configuration
			 * @param name item name to remove
			 * @throw NamedItemNotFound if item is not found
			 */
			inline void 							Remove(const std::string& name) {
				m_root.Remove(name);
			}
			/**
			 * Clears all items
			 */
			void 									Clear() noexcept;

			/**
			 * Gets item reference by path
			 * @param path path to item
			 * @return item
			 * @throw NamedItemNotFound if item is not found
			 */
			NamedItem&								operator[](const std::string& path);
			/**
			 * Gets item const reference by path
			 * @param path path to item
			 * @return item
			 * @throw NamedItemNotFound if item is not found
			 */
			const NamedItem&						operator[](const std::string& path) const;

			/* INPUT */
			/**
			 * Take data from another configuration
			 * @param source source configuration to import
			 * @return Reference to configuration
			 */
			Config& 								operator<<(const Config& source);
			/**
			 * Initialize configuration with an input stream
			 * @param istream input stream
			 */
			void 									operator<<(std::istream& istream); // 1
			/**
			 * Initialize configuration with a string
			 * @param str input string
			 */
			void 									operator<<(const std::string& str); // 2
			/**
			 * Initializes configuration with istream (when istream is in the left part)
			 * @param istream input stream
			 * @param file Config to put data to
			 */
			friend STORMBYTE_PUBLIC Config&			operator>>(std::istream& istream, Config& file); // 3
			/**
			 * Initializes configuration with string (when string is in the left part)
			 * @param str input string
			 * @param file Config to put data to
			 */
			friend STORMBYTE_PUBLIC Config&			operator>>(const std::string& str, Config& file); // 4
			
			/* OUTPUT */
			/**
			 * Outut current configuration to another configuration
			 * @param dest configuration destination
			 * @return a reference to destination conf
			 */
			Config& 								operator>>(Config& dest) const;
			/**
			 * Output configuration serialized to output stream
			 * @param ostream output stream
			 */
			std::ostream&							operator>>(std::ostream& ostream) const; // 5
			/**
			 * Output configuration serialized to string
			 * @param str output string
			 */
			std::string&							operator>>(std::string& str) const; // 6
			/**
			 * Output configuration serialized to output stream (when output stream is in the left part)
			 * @param ostream output stream
			 * @param file Config to get data from
			 */
			friend STORMBYTE_PUBLIC std::ostream&	operator<<(std::ostream& ostream, const Config& file); // 7
			/**
			 * Output configuration serialized to string (when string is in the left part)
			 * @param str output string
			 * @param file Config to get data from
			 */
			friend STORMBYTE_PUBLIC std::string& 	operator<<(std::string&, const Config&); // 8
			/**
			 * Converts current configuration to string
			 */
													operator std::string() const;
			
			/**
			 * Sets a function to execute on name collission when inserting
			 * @param on_clash function to select element on collission
			 * @see Group::OnNameClashFunctionType
			 */
			inline void								SetOnNameClashAction(const Group::OnNameClashAction& on_clash) {
				m_on_name_clash_action = on_clash;
			}
			/**
			 * Adds a hook which will take Config as parameter and will be executed before read start
			 * Hooks will be executed *in order*
			 */
			inline void 							AddHookBeforeRead(std::function<void(Config&)> hook) {
				m_before_read_hooks.push_back(hook);
			}
			
			/**
			 * Adds a hook which will take Config as parameter and will be executed on successful read
			 * Hooks will be executed *in order*
			 */
			inline void 							AddHookAfterRead(std::function<void(Config&)> hook) {
				m_after_read_hooks.push_back(hook);
			}

			/**
			 * Checks the existence of a child by path
			 * @param path path to child
			 * @return bool
			 */
			bool									Exists(const std::string& path) const noexcept;

			/**
			 * Gets the root group
			 * @return root group
			 */
			Group::Iterator							Begin() noexcept;
			/**
			 * Gets the root group
			 * @return root group
			 */
			Group::Const_Iterator					Begin() const noexcept;
			/**
			 * Gets the root group
			 * @return root group
			 */
			Group::Iterator							End() noexcept;
			/**
			 * Gets the root group
			 * @return root group
			 */
			Group::Const_Iterator					End() const noexcept;

		protected:
			/**
			 * Internal root Group item
			 */
			Group m_root;

			/**
			 * Ordered hook list which will be executed sequentially
			 * in their corresponding event
			 */
			std::vector<std::function<void(Config&)>> m_before_read_hooks, m_after_read_hooks;

			/**
			 * Function to override the default action when duplicate name is found when inserting
			 * Takes 3 parameters: current configuration, existing item and new item and will return
			 * the item to be inserted (or might throw to cancel the insert)
			 */
			Group::OnNameClashAction m_on_name_clash_action;

		private:
			/**
			 * Parses a container contents
			 * @param start start character
			 * @param end end character
			 * @param istream input stream
			 * @return parsed string
			 */
			template<const char start, const char end> std::string ParseContainerContents(std::istream& istream);	
			/**
			 * Parses a value
			 * @param istream input stream
			 * @return parsed value
			 */
			template<typename T> T					ParseValue(std::istream& istream);
			#ifdef MSVC
			template<> double						ParseValue<double>(std::istream& istream);
			template<> int 							ParseValue<int>(std::istream& istream);
			template<> std::string 					ParseValue<std::string>(std::istream& istream);
			template<> bool 						ParseValue<bool>(std::istream& istream);
			#endif
			/**
			 * Finds and parses a comment
			 * @param istream input stream
			 * @param container container to put comments to
			 * @return bool
			 */
			template<class C> bool					FindAndParseComment(std::istream& istream, C& container);
			/**
			 * Finds and parses comments
			 * @param istream input stream
			 * @param container container to put comments to
			 */
			template<class C> void					FindAndParseComments(std::istream& istream, C& container);
			/**
			 * Parses an item
			 * @param istream input stream
			 * @param type item type
			 * @return parsed item
			 */
			std::unique_ptr<Item>					ParseItem(std::istream& istream, const Item::Type& type);
			/**
			 * Parses a named item
			 * @param istream input stream
			 * @param type item type
			 * @param item_name item name
			 * @return parsed item
			 */
			std::unique_ptr<NamedItem>				ParseItem(std::istream& istream, const Item::Type& type, std::string&& item_name);
			/**
			 * Parses a group
			 * @param istream input stream
			 * @param group group to put data to
			 */
			void 									Parse(std::istream& istream, Group& group);
			/**
			 * Parses a list
			 * @param istream input stream
			 * @param list list to put data to
			 */
			void 									Parse(std::istream& istream, List& list);
			/**
			 * Gets current line from input stream
			 * @param istream input stream
			 * @return current line
			 */
			std::string 							GetCurrentLine(std::istream& istream);
			/**
			 * Gets current line from input stream with offset
			 * @param istream input stream
			 * @param offset offset
			 * @return current line
			 */
			std::string 							GetCurrentLine(std::istream& istream, const int& offset);
			/**
			 * Parses an item name
			 * @param istream input stream
			 * @return item name
			 */
			std::string 							ParseItemName(std::istream& istream);
			/**
			 * Parses an item type
			 * @param istream input stream
			 * @return item type
			 */
			Item::Type								ParseType(std::istream& istream);
	};
	/**
	 * Initializes configuration with istream (when istream is in the left part)
	 * @param istream input stream
	 * @param file Config to put data to
	 */
	STORMBYTE_PUBLIC Config&						operator>>(std::istream& istream, Config& file);
	/**
	 * Initializes configuration with string (when string is in the left part)
	 * @param str input string
	 * @param file Config to put data to
	 */
	STORMBYTE_PUBLIC Config&						operator>>(const std::string& str, Config& file);
	/**
	 * Output configuration serialized to output stream (when output stream is in the left part)
	 * @param ostream output stream
	 * @param file Config to get data from
	 */
	STORMBYTE_PUBLIC std::ostream&					operator<<(std::ostream& ostream, const Config& file);
	/**
	 * Output configuration serialized to string (when string is in the left part)
	 * @param str output string
	 * @param file Config to get data from
	 */
	STORMBYTE_PUBLIC std::string& 					operator<<(std::string& str, const Config& file);
}
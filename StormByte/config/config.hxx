#pragma once

#include <StormByte/config/exception.hxx>
#include <StormByte/config/group.hxx>

#include <functional>
#include <memory>
#include <istream>
#include <string>
#include <ostream>
#include <optional>

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
			inline Item&							Add(const Item& item) {
				return m_root.Add(item, m_on_name_clash_action);
			}
			/**
			 * Moves an item to configuration
			 * @param item item to add
			 * @throw InvalidName thrown when item name is not allowed
			 * @return a reference to the added item
			 */
			inline Item&							Add(Item&& item) {
				return m_root.Add(std::move(item), m_on_name_clash_action);
			}
			/**
			 * Removes an item from configuration
			 * @param name item name to remove
			 * @throw ItemNotFound if item is not found
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
			 * @throw ItemNotFound if item is not found
			 */
			Item&									operator[](const std::string& path);
			/**
			 * Gets item const reference by path
			 * @param path path to item
			 * @return item
			 * @throw ItemNotFound if item is not found
			 */
			const Item&								operator[](const std::string& path) const;

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

			Group::Iterator							Begin() noexcept;
			Group::Const_Iterator					Begin() const noexcept;
			Group::Iterator							End() noexcept;
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
			 * Parses a value
			 * @param stream
			 * @return value depending on template
			 * @throw ParseError is thrown if integer is illformed
			 */
			template<typename T> T					ParseValue(std::istream& stream);
			/**
			 * Starts the Parse process and execute hooks after
			 * @param stream stream with data
			 * @param group Group to add items
			 * @throw ParseError will be thrown on error
			 */
			void 									Parse(std::istream& stream, std::unique_ptr<Group>& group);
			/**
			 * Parses an stream and add found items to Group
			 * @param stream stream with data
			 * @param group Group to add items
			 * @throw ParseError will be thrown on error
			 */
			void 									Parse(std::istream& stream, Group& group);
			/**
			 * Parses config item name from stream
			 * @param stream stream with data
			 * @return item name string
			 */
			std::string 							ParseItemName(std::istream& stream);
			/**
			 * Expects equal sign in next read from stream
			 * @param stream stream with data
			 * @throw ParseError is thrown if equal sign is not the next read
			 */
			void 									ExpectEqualSign(std::istream& stream);
			/**
			 * Will parse the stream to guess Type and restore the pointer to where it was
			 * @param stream
			 * @return Type guessed Type
			 * @throw ParseError is thrown if Type can't be guessed
			 */
			Item::Type 								GuessType(std::istream& stream);
			/**
			 * Parses an string value
			 * @param stream
			 * @return string value
			 * @throw ParseError is thrown if string is illformed
			 */
			std::string 							ParseStringValue(std::istream& stream);
			/**
			 * Will get the whole group inner content as string
			 * @param stream
			 * @return string value
			 * @throw ParseError is thrown if braces are missmatches
			 */
			std::string 							ParseGroupContent(std::istream& stream);
			/**
			 * Parses an double value
			 * @param stream
			 * @return double value
			 * @throw ParseError is thrown if double is illformed
			 */
			double 									ParseDoubleValue(std::istream& stream);
			/**
			 * Parses the exponential part of a double value
			 * @param stream
			 * @return string value (containing the exponential part)
			 * @throw ParseError is thrown if the exponential part is illformed
			 */
			std::string 							ParseExpValue(std::istream& stream);
			/**
			 * Parses an bool value
			 * @param stream
			 * @return bool value
			 * @throw ParseError is thrown if bool is illformed
			 */
			bool 									ParseBoolValue(std::istream& stream);
			/**
			 * Parses an comment
			 * @param stream
			 * @return bool if it found a commentary
			 * @throw ParseError is thrown if string is illformed
			 */
			bool 									FindAndParseComment(std::istream&, Group& group);
			/**
			 * Consumes every empty character from string (spaces, tabs, newlines, etc)
			 * @param stream stream with data
			 */
			void 									ConsumeEmptyChars(std::istream& stream);
			/**
			 * Expects semicolon in next read from stream
			 * @param stream stream with data
			 * @throw ParseError is thrown if semicolon is not the next read
			 */
			void 									ExpectSemicolon(std::istream& stream);
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
#pragma once

#include <StormByte/config/exception.hxx>
#include <StormByte/config/item/group.hxx>

#include <functional>
#include <memory>
#include <istream>
#include <string>
#include <ostream>

/**
 * @namespace StormByte::Config
 * @brief All the classes for handling configuration files and items
 */
namespace StormByte::Config {
	/**
	 * @class File
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
	class STORMBYTE_PUBLIC File {
		public:
			/**
			 * Constructor
			 */
			File();
			/**
			 * Copy constructor
			 */
			File(const File&);
			/**
			 * Move constructor
			 */
			File(File&&) noexcept;
			/**
			 * Assignment operator
			 */
			File& operator=(const File&);
			/**
			 * Move assignment operator
			 */
			File& operator=(File&&) noexcept;
			/**
			 * Destructor
			 */
			virtual ~File()						= default;

			/**
			 * Adds an item
			 * @param name item name
			 * @param type item Type
			 * @throw InvalidName thrown when item name is not allowed
			 * @return a pointer to the added item
			 */
			inline std::shared_ptr<Item>	Add(const std::string& name, const Item::Type& type) {
				return m_root->Add(name, type);
			}
			/**
			 * Clears all items
			 */
			void 							Clear() noexcept;

			/* INPUT */
			/**
			 * Initialize configuration with an input stream
			 * @param istream input stream
			 */
			void 							operator<<(std::istream& istream); // 1
			/**
			 * Initialize configuration with a string
			 * @param str input string
			 */
			void 							operator<<(const std::string& str); // 2
			/**
			 * Initializes configuration with istream (when istream is in the left part)
			 * @param istream input stream
			 * @param file File to put data to
			 */
			friend File&					operator>>(std::istream& istream, File& file); // 3
			/**
			 * Initializes configuration with string (when string is in the left part)
			 * @param str input string
			 * @param file File to put data to
			 */
			friend File&					operator>>(const std::string& str, File& file); // 4
			
			/* OUTPUT */
			/**
			 * Output configuration serialized to output stream
			 * @param ostream output stream
			 */
			std::ostream&					operator>>(std::ostream& ostream); // 5
			/**
			 * Output configuration serialized to string
			 * @param str output string
			 */
			std::string&					operator>>(std::string& str); // 6
			/**
			 * Output configuration serialized to output stream (when output stream is in the left part)
			 * @param ostream output stream
			 * @param file File to get data from
			 */
			friend std::ostream&			operator<<(std::ostream& ostream, const File& file); // 7
			/**
			 * Output configuration serialized to string (when string is in the left part)
			 * @param str output string
			 * @param file File to get data from
			 */
			friend std::string& 			operator<<(std::string&, const File&); // 8
			/**
			 * Converts current configuration to string
			 */
											operator std::string() const;
			
			/**
			 * Adds a hook which will take File as parameter and will be executed before read start
			 * Hooks will be executed *in order*
			 */
			inline void 					AddHookBeforeRead(std::function<void(File&)> hook) {
				m_before_read_hooks.push_back(hook);
			}
			
			/**
			 * Adds a hook which will take File as parameter and will be executed on successful read
			 * Hooks will be executed *in order*
			 */
			inline void 					AddHookAfterRead(std::function<void(File&)> hook) {
				m_after_read_hooks.push_back(hook);
			}

			/**
			 * Gets the child by path
			 * @param path path to child
			 * @return pointer to found Item or nullptr
			 */
			std::shared_ptr<Item>			Child(const std::string& path) const;
			/**
			 * Checks the existence of a child by path
			 * @param path path to child
			 * @return bool
			 */
			bool							Exists(const std::string& path) const noexcept;
			/**
			 * Looks up a child by path
			 * @param path path to child
			 * @throw ItemNotFound if not found
			 */
			std::shared_ptr<Item>			LookUp(const std::string& path) const;

		protected:
			/**
			 * Internal root Group item
			 */
			std::unique_ptr<Group> 	m_root;

			/**
			 * Ordered hook list which will be executed sequentially
			 * in their corresponding event
			 */
			std::vector<std::function<void(File&)>> m_before_read_hooks, m_after_read_hooks;

		private:
			/**
			 * Starts the Parse process and execute hooks after
			 * @param stream stream with data
			 * @param group Group to add items
			 * @throw ParseError will be thrown on error
			 */
			void 							Parse(std::istream& stream, std::unique_ptr<Group>& group);
			/**
			 * Parses an stream and add found items to Group
			 * @param stream stream with data
			 * @param group Group to add items
			 * @throw ParseError will be thrown on error
			 */
			void 							Parse(std::istream& stream, Group& group);
			/**
			 * Parses config item name from stream
			 * @param stream stream with data
			 * @return item name string
			 */
			std::string 					ParseItemName(std::istream& stream);
			/**
			 * Expects equal sign in next read from stream
			 * @param stream stream with data
			 * @throw ParseError is thrown if equal sign is not the next read
			 */
			void 							ExpectEqualSign(std::istream& stream);
			/**
			 * Will parse the stream to guess Type and restore the pointer to where it was
			 * @param stream
			 * @return Type guessed Type
			 * @throw ParseError is thrown if Type can't be guessed
			 */
			Item::Type 						GuessType(std::istream& stream);
			/**
			 * Parses an int value
			 * @param stream
			 * @return integer value
			 * @throw ParseError is thrown if integer is illformed
			 */
			int								ParseIntValue(std::istream& stream);
			/**
			 * Parses an string value
			 * @param stream
			 * @return string value
			 * @throw ParseError is thrown if string is illformed
			 */
			std::string 					ParseStringValue(std::istream& stream);
			/**
			 * Will get the whole group inner content as string
			 * @param stream
			 * @return string value
			 * @throw ParseError is thrown if braces are missmatches
			 */
			std::string 					ParseGroupContent(std::istream& stream);
			/**
			 * Parses an double value
			 * @param stream
			 * @return double value
			 * @throw ParseError is thrown if double is illformed
			 */
			double 							ParseDoubleValue(std::istream& stream);
			/**
			 * Parses the exponential part of a double value
			 * @param stream
			 * @return string value (containing the exponential part)
			 * @throw ParseError is thrown if the exponential part is illformed
			 */
			std::string 					ParseExpValue(std::istream& stream);
			/**
			 * Parses an bool value
			 * @param stream
			 * @return bool value
			 * @throw ParseError is thrown if bool is illformed
			 */
			bool 							ParseBoolValue(std::istream& stream);
			/**
			 * Parses an comment
			 * @param stream
			 * @return bool if it found a commentary
			 * @throw ParseError is thrown if string is illformed
			 */
			bool 							FindAndParseComment(std::istream&, Group& group);
			/**
			 * Consumes every empty character from string (spaces, tabs, newlines, etc)
			 * @param stream stream with data
			 */
			void 							ConsumeEmptyChars(std::istream& stream);
			/**
			 * Expects semicolon in next read from stream
			 * @param stream stream with data
			 * @throw ParseError is thrown if semicolon is not the next read
			 */
			void 							ExpectSemicolon(std::istream& stream);
	};
	/**
	 * Initializes configuration with istream (when istream is in the left part)
	 * @param istream input stream
	 * @param file File to put data to
	 */
	inline File&							operator>>(std::istream& istream, File& file) { // 3
		file << istream;
		return file;
	}
	/**
	 * Initializes configuration with string (when string is in the left part)
	 * @param str input string
	 * @param file File to put data to
	 */
	inline File&							operator>>(const std::string& str, File& file) { // 4
		file << str;
		return file;
	}
	/**
	 * Output configuration serialized to output stream (when output stream is in the left part)
	 * @param ostream output stream
	 * @param file File to get data from
	 */
	inline std::ostream&					operator<<(std::ostream& ostream, const File& file) { // 7
		ostream << (std::string)file;
		return ostream;
	}
	/**
	 * Output configuration serialized to string (when string is in the left part)
	 * @param str output string
	 * @param file File to get data from
	 */
	inline std::string& 					operator<<(std::string& str, const File& file) { // 8
		str += file;
		return str;
	}
}
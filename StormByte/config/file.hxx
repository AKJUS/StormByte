#pragma once

#include <StormByte/config/exception.hxx>
#include <StormByte/config/item/group.hxx>

#include <filesystem>
#include <memory>
#include <string>

/**
 * @namespace StormByte::Config
 * @brief All the classes for handling configuration files and items
 */
namespace StormByte::Config {
	/**
	 * @class File
	 * @brief Abstract class for a configuration file
	 */
	class STORMBYTE_PUBLIC File {
		public:
			/**
			 * Constructor
			 * @param path Path to filename (to read from/save to)
			 */
			File(const std::filesystem::path& path);
			/**
			 * Constructor
			 * @param path Path to filename (to read from/save to)
			 */
			File(std::filesystem::path&& path);
			/**
			 * Copy constructor
			 */
			File(const File&);
			/**
			 * Move constructor
			 */
			File(File&&) noexcept				= default;
			/**
			 * Assignment operator
			 */
			File& operator=(const File&);
			/**
			 * Move assignment operator
			 */
			File& operator=(File&&) noexcept	= default;
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
			std::shared_ptr<Item>	Add(const std::string& name, const Item::Type& type);
			/**
			 * Clears all items
			 */
			void 					Clear() noexcept;
			/**
			 * Read from file
			 */
			void 					Read();
			/**
			 * Read from string
			 */
			void					ReadFromString(const std::string&);
			/**
			 * Write to file
			 */
			void 					Write();

			/**
			 * Gets the child by path
			 * @param path path to child
			 * @return pointer to found Item or nullptr
			 */
			std::shared_ptr<Item>	Child(const std::string& path) const;
			/**
			 * Checks the existence of a child by path
			 * @param path path to child
			 * @return bool
			 */
			bool					Exists(const std::string& path) const noexcept;
			/**
			 * Looks up a child by path
			 * @param path path to child
			 * @throw ItemNotFound if not found
			 */
			std::shared_ptr<Item>	LookUp(const std::string& path) const;

		protected:
			/**
			 * Post read operations that will be called once Read operation is successful
			 * Must be reimplemented in derived classes
			 */
			virtual void			PostRead() noexcept = 0;

			/**
			 * Internal root Group item
			 */
			std::shared_ptr<Group> 	m_root;
			/**
			 * File that will be used for read from/save to
			 */
			std::filesystem::path 	m_file;

		private:
			/**
			 * Parses an stream and add found items to Group
			 * @param stream stream with data
			 * @param group Group to add items
			 */
			void 					Parse(std::istream& stream, std::shared_ptr<Group>& group);
			/**
			 * Parses config item name from stream
			 * @param stream stream with data
			 * @return item name string
			 */
			std::string 			ParseItemName(std::istream& stream);
			/**
			 * Expects equal sign in next read from stream
			 * @param stream stream with data
			 * @throw ParseError is thrown if equal sign is not the next read
			 */
			void 					ExpectEqualSign(std::istream& stream);
			/**
			 * Will parse the stream to guess Type and restore the pointer to where it was
			 * @param stream
			 * @return Type guessed Type
			 * @throw ParseError is thrown if Type can't be guessed
			 */
			Item::Type 				GuessType(std::istream& stream);
			/**
			 * Parses an int value
			 * @param stream
			 * @return integer value
			 * @throw ParseError is thrown if integer is illformed
			 */
			int						ParseIntValue(std::istream& stream);
			/**
			 * Parses an string value
			 * @param stream
			 * @return string value
			 * @throw ParseError is thrown if string is illformed
			 */
			std::string 			ParseStringValue(std::istream& stream);
			/**
			 * Will get the whole group inner content as string
			 * @param stream
			 * @return string value
			 * @throw ParseError is thrown if braces are missmatches
			 */
			std::string 			ParseGroupContent(std::istream& stream);
			/**
			 * Parses an double value
			 * @param stream
			 * @return double value
			 * @throw ParseError is thrown if double is illformed
			 */
			double 					ParseDoubleValue(std::istream& stream);
			/**
			 * Parses the exponential part of a double value
			 * @param stream
			 * @return string value (containing the exponential part)
			 * @throw ParseError is thrown if the exponential part is illformed
			 */
			std::string 			ParseExpValue(std::istream& stream);
			/**
			 * Parses an bool value
			 * @param stream
			 * @return bool value
			 * @throw ParseError is thrown if bool is illformed
			 */
			bool 					ParseBoolValue(std::istream& stream);
			/**
			 * Parses an comment
			 * @param stream
			 * @return bool if it found a commentary
			 * @throw ParseError is thrown if string is illformed
			 */
			bool 					FindAndParseComment(std::istream&, std::shared_ptr<Group>& group);
			/**
			 * Consumes every empty character from string (spaces, tabs, newlines, etc)
			 * @param stream stream with data
			 */
			void 					ConsumeEmptyChars(std::istream& stream);
			/**
			 * Expects semicolon in next read from stream
			 * @param stream stream with data
			 * @throw ParseError is thrown if semicolon is not the next read
			 */
			void 					ExpectSemicolon(std::istream& stream);
	};
}
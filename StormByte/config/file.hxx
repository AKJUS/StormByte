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

			std::shared_ptr<Group> 	m_root;
			std::filesystem::path 	m_file;

		private:
			void 					Parse(std::istream&, std::shared_ptr<Group>& group);
			std::string 			ParseItemName(std::istream&);
			void 					ExpectEqualSign(std::istream&);
			Item::Type 				GuessType(std::istream&);
			int						ParseIntValue(std::istream&);
			std::string 			ParseStringValue(std::istream&);
			std::string 			ParseGroupContent(std::istream&);
			double 					ParseDoubleValue(std::istream&);
			std::string 			ParseExpValue(std::istream&);
			bool 					ParseBoolValue(std::istream&);
			bool 					FindAndParseComment(std::istream&, std::shared_ptr<Group>& group);
			void 					ConsumeEmptyChars(std::istream&);
			void 					ExpectSemicolon(std::istream&);
	};
}
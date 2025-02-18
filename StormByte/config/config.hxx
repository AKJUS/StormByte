#pragma once

#include <StormByte/config/group.hxx>

#include <functional>

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
	class STORMBYTE_PUBLIC Config: private Group {
		public:
			/**
			 * @class Iterator
			 * @brief Iterator for Config
			 */
			using Iterator = Group::Iterator;

			/**
			 * @class ConstIterator
			 * @brief Const iterator for Config
			 */
			using ConstIterator = Group::ConstIterator;

			/**
			 * Constructor
			 */
			constexpr Config():m_on_existing_action(Container::OnExistingAction::ThrowException) {}

			/**
			 * Copy constructor
			 */
			constexpr Config(const Config&)					= default;

			/**
			 * Move constructor
			 */
			constexpr Config(Config&&) noexcept				= default;

			/**
			 * Assignment operator
			 */
			Config& operator=(const Config&)				= default;

			/**
			 * Move assignment operator
			 */
			constexpr Config& operator=(Config&&) noexcept	= default;

			/**
			 * Destructor
			 */
			virtual constexpr ~Config()	override			= default;

			/**
			 * Gets a reference to item by path
			 * @param path path to item
			 * @return item reference
			 */
			constexpr Item&									operator[](const std::string& path) override {
				return const_cast<Item&>(static_cast<const Config&>(*this)[path]);
			}

			/**
			 * Gets a const reference to item by path
			 * @param path path to item
			 * @return item const reference
			 */
			constexpr const Item&							operator[](const std::string& path) const override {
				return Group::operator[](path);
			}

			/**
			 * Gets a reference to item by index
			 * @param index index of item
			 * @throw OutOfBounds if index is out of bounds
			 * @return item reference
			 */
			constexpr Item& 								operator[](const size_t& index) override {
				return const_cast<Item&>(static_cast<const Config&>(*this)[index]);
			}

			/**
			 * Gets a const reference to item by index
			 * @param index index of item
			 * @throw OutOfBounds if index is out of bounds
			 * @return item const reference
			 */
			constexpr const Item& 							operator[](const size_t& index) const override {
				return Group::operator[](index);
			}

			/**
			 * Compares two configurations
			 * @param other configuration to compare to
			 * @return bool are they equal?
			 */
			constexpr bool 									operator==(const Config& other) const noexcept {
				return Group::operator==(other);
			}

			/**
			 * Compares two configurations
			 * @param other configuration to compare to
			 * @return bool are they not equal?
			 */
			constexpr bool 									operator!=(const Config& other) const noexcept {
				return !(*this == other);
			}

			/* INPUT */
			/**
			 * Take data from another configuration
			 * @param source source configuration to import
			 * @return Reference to configuration
			 */
			Config& 										operator<<(const Config& source);

			/**
			 * Initialize configuration with an input stream
			 * @param istream input stream
			 */
			void 											operator<<(std::istream& istream); // 1

			/**
			 * Initialize configuration with a string
			 * @param str input string
			 */
			void 											operator<<(const std::string& str); // 2

			/**
			 * Initializes configuration with istream (when istream is in the left part)
			 * @param istream input stream
			 * @param file Config to put data to
			 */
			friend STORMBYTE_PUBLIC Config&					operator>>(std::istream& istream, Config& file); // 3

			/**
			 * Initializes configuration with string (when string is in the left part)
			 * @param str input string
			 * @param file Config to put data to
			 */
			friend STORMBYTE_PUBLIC Config&					operator>>(const std::string& str, Config& file); // 4
			
			/* OUTPUT */
			/**
			 * Outut current configuration to another configuration
			 * @param dest configuration destination
			 * @return a reference to destination conf
			 */
			Config& 										operator>>(Config& dest) const;

			/**
			 * Output configuration serialized to output stream
			 * @param ostream output stream
			 */
			std::ostream&									operator>>(std::ostream& ostream) const; // 5

			/**
			 * Output configuration serialized to string
			 * @param str output string
			 */
			std::string&									operator>>(std::string& str) const; // 6

			/**
			 * Output configuration serialized to output stream (when output stream is in the left part)
			 * @param ostream output stream
			 * @param file Config to get data from
			 */
			friend STORMBYTE_PUBLIC std::ostream&			operator<<(std::ostream& ostream, const Config& file); // 7

			/**
			 * Output configuration serialized to string (when string is in the left part)
			 * @param str output string
			 * @param file Config to get data from
			 */
			friend STORMBYTE_PUBLIC std::string& 			operator<<(std::string&, const Config&); // 8

			/**
			 * Converts current configuration to string
			 */
															operator std::string() const;

			/**
			 * Adds an item to the container
			 * @param item item to add
			 * @throw ItemNameAlreadyExists if item name already exists
			 * @return reference to added item
			 */
			constexpr Item&									Add(const Item& item) {
				return Group::Add(Item(item), m_on_existing_action);
			}

			/**
			 * Adds an item to the container
			 * @param item item to add
			 * @throw ItemNameAlreadyExists if item name already exists
			 * @return reference to added item
			 */
			constexpr Item&									Add(Item&& item) {
				return Group::Add(std::move(item), m_on_existing_action);
			}

			/**
			 * Adds a comment to the container
			 * @param comment comment value
			 */
			inline void										AddComment(const std::string& comment) {
				Group::AddComment(comment);
			}

			/**
			 * Checks if item exists by path
			 * @param path path to item
			 * @return bool exists?
			 */
			inline bool 									Exists(const std::string& path) const {
				return Group::Exists(path);
			}

			/**
			 * Removes an item by path
			 * @param item item to add
			 * @throw ItemNotFound if item is not found
			 */
			inline void										Remove(const std::string& path) {
				Group::Remove(path);
			}

			/**
			 * Removes an item by position
			 * @param item item to add
			 * @throw OutOfBounds if index is out of bounds
			 */
			constexpr void									Remove(const size_t& path) {
				Group::Remove(path);
			}

			/**
			 * Gets a reference to item by path
			 * @param path path to item
			 * @return item reference
			 */
			constexpr size_t 								Size() const noexcept {
				return Group::Size();
			}

			/**
			 * Sets a function to execute on name collission when inserting
			 * @param on_existing function to select element on collission
			 * @see Container::OnExistingAction
			 */
			constexpr void									SetOnExistingAction(const Container::OnExistingAction& on_existing) {
				m_on_existing_action = on_existing;
			}

			/**
			 * Adds a hook which will take Config as parameter and will be executed before read start
			 * Hooks will be executed *in order*
			 */
			inline void 									AddHookBeforeRead(std::function<void(Config&)> hook) {
				m_before_read_hooks.push_back(hook);
			}
			
			/**
			 * Adds a hook which will take Config as parameter and will be executed on successful read
			 * Hooks will be executed *in order*
			 */
			inline void 									AddHookAfterRead(std::function<void(Config&)> hook) {
				m_after_read_hooks.push_back(hook);
			}

			/**
			 * Gets an iterator pointing to the first element
			 * @return iterator
			 */
			constexpr Iterator								Begin() noexcept override {
				return Group::Begin();
			}

			/**
			 * Gets a const iterator pointing to the first element
			 * @return const iterator
			 */
			constexpr ConstIterator							Begin() const noexcept override {
				return Group::Begin();
			}

			/**
			 * Gets an iterator pointing to past last element
			 * @return iterator
			 */
			constexpr Iterator								End() noexcept override {
				return Group::End();
			}

			/**
			 * Gets a const iterator pointing to past last element
			 * @return const iterator
			 */
			constexpr ConstIterator							End() const noexcept override {
				return Group::End();
			}

		protected:
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
			Container::OnExistingAction m_on_existing_action;

		private:
			/**
			 * @enum ParseMode
			 * @brief Parsing mode
			 */
			enum class ParseMode: unsigned short {
				Named,
				Unnamed
			};

			/**
			 * Parses a container contents
			 * @param start start character
			 * @param end end character
			 * @param istream input stream
			 * @return parsed string
			 */
			template<const char start, const char end> std::string ParseContainerContents(std::istream& istream);

			/**
			 * Parses a container contents
			 * @param container_type container type
			 * @param istream input stream
			 * @return parsed string
			 */
			std::string ParseContainerContents(std::istream& istream, const Container::Type& container_type);

			/**
			 * Parses a value
			 * @param istream input stream
			 * @return parsed value
			 */
			template<typename T> T							ParseValue(std::istream& istream);
			#ifdef MSVC
			/**
			 * Parses a value
			 * @param istream input stream
			 * @return parsed value
			 */
			template<> double								ParseValue<double>(std::istream& istream);

			/**
			 * Parses a value
			 * @param istream input stream
			 * @return parsed value
			 */
			template<> int 									ParseValue<int>(std::istream& istream);

			/**
			 * Parses a value
			 * @param istream input stream
			 * @return parsed value
			 */
			template<> std::string 							ParseValue<std::string>(std::istream& istream);

			/**
			 * Parses a value
			 * @param istream input stream
			 * @return parsed value
			 */
			template<> bool 								ParseValue<bool>(std::istream& istream);
			#endif

			/**
			 * Finds and parses a comment
			 * @param istream input stream
			 * @param container container to put comments to
			 * @return bool
			 */
			template<class C> bool							FindAndParseComment(std::istream& istream, C& container);

			/**
			 * Finds and parses comments
			 * @param istream input stream
			 * @param container container to put comments to
			 */
			template<class C> void							FindAndParseComments(std::istream& istream, C& container);

			/**
			 * Parses an item
			 * @param istream input stream
			 * @param type item type
			 * @return parsed item
			 */
			std::unique_ptr<Item>							ParseItem(std::istream& istream, const Item::Type& type);

			/**
			 * Parses a group
			 * @param istream input stream
			 * @param container Container to put data to
			 */
			void 											Parse(std::istream& istream, Container& group, const ParseMode& mode);

			/**
			 * Gets current line from input stream
			 * @param istream input stream
			 * @return current line
			 */
			std::string 									GetCurrentLine(std::istream& istream);

			/**
			 * Gets current line from input stream with offset
			 * @param istream input stream
			 * @param offset offset
			 * @return current line
			 */
			std::string 									GetCurrentLine(std::istream& istream, const int& offset);

			/**
			 * Parses an item name
			 * @param istream input stream
			 * @return item name
			 */
			std::string 									ParseItemName(std::istream& istream);

			/**
			 * Parses an item type
			 * @param istream input stream
			 * @return item type
			 */
			Item::Type										ParseType(std::istream& istream);

			/**
			 * Parses a container type
			 * @param istream input stream
			 * @return container type
			 */
			Container::Type 								ParseContainerType(std::istream& istream);
	};
	/**
	 * Initializes configuration with istream (when istream is in the left part)
	 * @param istream input stream
	 * @param file Config to put data to
	 */
	STORMBYTE_PUBLIC Config&								operator>>(std::istream& istream, Config& file);

	/**
	 * Initializes configuration with string (when string is in the left part)
	 * @param str input string
	 * @param file Config to put data to
	 */
	STORMBYTE_PUBLIC Config&								operator>>(const std::string& str, Config& file);

	/**
	 * Output configuration serialized to output stream (when output stream is in the left part)
	 * @param ostream output stream
	 * @param file Config to get data from
	 */
	STORMBYTE_PUBLIC std::ostream&							operator<<(std::ostream& ostream, const Config& file);

	/**
	 * Output configuration serialized to string (when string is in the left part)
	 * @param str output string
	 * @param file Config to get data from
	 */
	STORMBYTE_PUBLIC std::string& 							operator<<(std::string& str, const Config& file);
}
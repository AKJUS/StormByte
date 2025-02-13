#pragma once

#include <StormByte/config/item.hxx>

#include <cstddef>
#include <iterator>
#include <map>
#include <queue>

/**
 * @namespace StormByte::Config
 * @brief All the classes for handling configuration files and items
 */
namespace StormByte::Config {
	/**
	 * @class Group
	 * @brief Group in configuration item which can hold other items and also subgroups recursivelly
	 * @code
	 * settings = {
	 * 	username = "StormByte";
	 * 	credit = 66.5;
	 * };
	 * @endcode
	 */
	class STORMBYTE_PUBLIC Group final: public Item {
		friend class File;
		using GroupStorage = std::vector<std::shared_ptr<Item>>;
		public:
			/**
			 * Constructor
			 * @param name item name
			 */
			Group(const std::string& name);
			/**
			 * Constructor
			 * @param name item name
			 */
			Group(std::string&& name);
			/**
			 * Copy constructor
			 */
			Group(const Group&);
			/**
			 * Move constructor
			 */
			Group(Group&&) noexcept				= default;
			/**
			 * Assignment operator
			 */
			Group& operator=(const Group&);
			/**
			 * Move assignment operator
			 */
			Group& operator=(Group&&) noexcept	= default;
			/**
			 * Destructor
			 */
			~Group() noexcept override			= default;

			/**
			 * Value getter
			 * @return current Group
			 */
			Group&						AsGroup() override;

			/**
			 * Add a named item to this group
			 * @param name item name
			 * @param type item Type
			 * @throw InvalidName is thrown when item name is not allowed
			 * @return a pointer to the added item
			 */
			std::shared_ptr<Item>		Add(const std::string& name, const Type& type);
			/**
			 * Add an already created item to this group
			 * @param item item to add
			 * @throw InvalidName is thrown when item name is not allowed
			 * @return a pointer to the added item
			 */
			std::shared_ptr<Item>		Add(std::shared_ptr<Item> item);
			/**
			 * Removes an item from this group
			 * @param name item name to remove
			 * @throw ItemNotFound might be thrown if item name is not found
			 */
			void						Remove(const std::string& name);
			/**
			 * Gets the child by path
			 * @param path path to child
			 * @return pointer to found Item or nullptr
			 */
			std::shared_ptr<Item>		Child(const std::string& path) const;
			/**
			 * Checks the existence of a child by path
			 * @param path path to child
			 * @return bool
			 */
			bool						Exists(const std::string& path) const noexcept;
			/**
			 * Looks up a child by path
			 * @param path path to child
			 * @throw ItemNotFound if not found
			 * @return pointer to found Item
			 */
			std::shared_ptr<Item>		LookUp(const std::string& path) const;

			/**
			 * Serializes the boolean item
			 * @param indent_level intentation level
			 * @return serialized string
			 */
			std::string					Serialize(const int& indent_level) const noexcept override;

			/**
			 * @class Iterator
			 * @brief Group iterator
			 */
			class STORMBYTE_PUBLIC Iterator {
				friend class Group;
				public:
					/**
					 * Copy constructor
					 */
					Iterator(const Iterator&)					= default;
					/**
					 * Move constructor
					 */
					Iterator(Iterator&&) noexcept				= default;
					/**
					 * Assignment operator
					 */
					Iterator& operator=(const Iterator&)		= default;
					/**
					 * Move assignment operator
					 */
					Iterator& operator=(Iterator&&) noexcept	= default;
					/**
					 * Destructor
					 */
					~Iterator() noexcept 						= default;

					/**
					 * Pre increment operator
					 */
					Iterator& operator++() noexcept;
					/**
					 * Post increment operator
					 */
					Iterator operator++(int) noexcept;
					/**
					 * Pre decrement operator
					 */
					Iterator& operator--() noexcept;
					/**
					 * Post decrement operator
					 */
					Iterator operator--(int) noexcept;
					/**
					 * Equal comparison operator
					 */
					bool operator==(const Iterator&) const noexcept;
					/**
					 * Different comparison operator
					 */
					bool operator!=(const Iterator&) const noexcept;
					/**
					 * Indirect operator
					 */
					Item* operator->() noexcept;
					/**
					 * Access operator
					 */
					Item operator*() noexcept = delete;

				private:
					/**
					 * Constructor
					 */
					Iterator() noexcept							= default;

					/**
					 * Internal iterator to original data
					 */
					GroupStorage::iterator m_it;
			};

			/**
			 * @class Const_Iterator
			 * @brief Constant group iterator
			 */
			class STORMBYTE_PUBLIC Const_Iterator {
				friend class Group;
				public:
					/**
					 * Copy constructor
					 */
					Const_Iterator(const Const_Iterator&)					= default;
					/**
					 * Move constructor
					 */
					Const_Iterator(Const_Iterator&&) noexcept				= default;
					/**
					 * Assignment operator
					 */
					Const_Iterator& operator=(const Const_Iterator&)		= default;
					/**
					 * Move assignment operator
					 */
					Const_Iterator& operator=(Const_Iterator&&) noexcept	= default;
					/**
					 * Destructor
					 */
					~Const_Iterator() noexcept 								= default;

					/**
					 * Pre increment operator
					 */
					Const_Iterator& operator++() noexcept;
					/**
					 * Post increment operator
					 */
					Const_Iterator operator++(int) noexcept;
					/**
					 * Pre decrement operator
					 */
					Const_Iterator& operator--() noexcept;
					/**
					 * Post decrement operator
					 */
					Const_Iterator operator--(int) noexcept;
					/**
					 * Equal comparison operator
					 */
					bool operator==(const Const_Iterator&) const noexcept;
					/**
					 * Different comparison operator
					 */
					bool operator!=(const Const_Iterator&) const noexcept;
					/**
					 * Indirect operator
					 */
					const Item* operator->() const noexcept;
					/**
					 * Access operator
					 */
					const Item operator*() const noexcept = delete;

				private:
					/**
					 * Constructor
					 */
					Const_Iterator() noexcept								= default;

					/**
					 * Internal iterator to original data
					 */
					GroupStorage::const_iterator m_it;
			};

			/**
			 * Gets an Iterator pointing to first element
			 * @return Iterator
			 */
			Iterator 					Begin() noexcept;
			/**
			 * Gets a Const_Iterator pointing to first element
			 * @return Const_Iterator
			 */
			Const_Iterator				Begin() const noexcept;
			/**
			 * Gets an Iterator pointing past last element
			 * @return Iterator
			 */
			Iterator 					End() noexcept;
			/**
			 * Gets a Const_Iterator pointing past last element
			 */
			Const_Iterator 				End() const noexcept;
			/**
			 * Gets a Const_Iterator pointing to first element
			 * @return Const_Iterator
			 */
			Const_Iterator 				CBegin() const noexcept;
			/**
			 * Gets a Const_Iterator pointing past last element
			 */
			Const_Iterator 				CEnd() const noexcept;

		private:
			/**
			 * Clones this object
			 * @return a shared pointer for this item
			 */
			std::shared_ptr<Item>		Clone() override;
			/**
			 * Internal version for Lookup
			 * @param queue queue for path representation
			 * @return shared pointer to Item base class with found instance
			 * @throw runtime_error
			 */
			std::shared_ptr<Item>		LookUp(std::queue<std::string>& queue) const;
			/**
			 * Internal version for checking if item exists
			 * @param queue search path represented as a queue
			 */
			bool						Exists(std::queue<std::string>& queue) const noexcept;
			/**
			 * Internal function for exploding a path to a queue
			 * @param path path as string
			 * @return queue from path
			 */
			std::queue<std::string> 	ExplodePath(const std::string& path) const noexcept;

			/**
			 * Internal map with children so name accesses are fast
			 */
			std::map<std::string, std::shared_ptr<Item>>	m_children;
			/**
			 * Ordered items vector
			 */
			std::vector<std::shared_ptr<Item>> 				m_ordered;
	};
}
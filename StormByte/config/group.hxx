#pragma once

#include <StormByte/visibility.h>

#include <cstddef>
#include <iterator>
#include <functional>
#include <list>
#include <map>
#include <memory>
#include <optional>
#include <queue>
#include <string>

/**
 * @namespace StormByte::Config
 * @brief All the classes for handling configuration files and items
 */
namespace StormByte::Config {
	class Item; // Forward
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
	class STORMBYTE_PUBLIC Group final {
		public:
			/**
			 * Constructor
			 */
			Group()								= default;
			/**
			 * Copy constructor
			 */
			Group(const Group&)					= default;
			/**
			 * Move constructor
			 */
			Group(Group&&) noexcept				= default;
			/**
			 * Assignment operator
			 */
			Group& operator=(const Group&)		= default;
			/**
			 * Move assignment operator
			 */
			Group& operator=(Group&&) noexcept	= default;
			/**
			 * Destructor
			 */
			~Group() noexcept					= default;

			/**
			 * @enum OnNameClashAction
			 * @brief Action to take when a name is already in use
			 */
			enum class OnNameClashAction {
				KeepExistingItem,
				OverwriteExistingItem,
				ThrowException
			};

			/**
			 * Gets item reference by path
			 * @param path path to item
			 * @return item
			 * @throw ItemNotFound if item is not found
			 */
			Item&						operator[](const std::string& path);
			/**
			 * Gets item const reference by path
			 * @param path path to item
			 * @return item
			 * @throw ItemNotFound if item is not found
			 */
			const Item&					operator[](const std::string& path) const;

			/**
			 * Add a copy of an already created item to this group
			 * @param item item to add
			 * @param on_clash function to execute when name already exists
			 * @throw InvalidName is thrown when item name is not allowed
			 * @throw ItemNameAlreadyExists is thrown when item name already exists
			 * @return a pointer to the added item
			 */
			Item&						Add(const Item& item, const OnNameClashAction& on_clash = OnNameClashAction::ThrowException);
			/**
			 * Move an already created item to this group
			 * @param item item to add
			 * @param on_clash function to execute when name already exists
			 * @throw InvalidName is thrown when item name is not allowed
			 * @throw ItemNameAlreadyExists is thrown when item name already exists
			 * @return a pointer to the added item
			 */
			Item&						Add(Item&& item, const OnNameClashAction& on_clash = OnNameClashAction::ThrowException);
			/**
			 * Removes an item from this group
			 * @param name item name to remove which can be a full path
			 * @throw ItemNotFound might be thrown if item name is not found
			 */
			void						Remove(const std::string& name);
			/**
			 * Checks the existence of a child by path
			 * @param path path to child
			 * @return bool
			 */
			bool						Exists(const std::string& path) const noexcept;

			/**
			 * Serializes the boolean item
			 * @param indent_level intentation level
			 * @return serialized string
			 */
			std::string					Serialize(const int& indent_level) const noexcept;

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
					Item& operator*() noexcept;

				private:
					/**
					 * Constructor
					 */
					Iterator() noexcept							= default;

					/**
					 * Internal iterator to original data
					 */
					std::list<Item>::iterator m_it;
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
					const Item& operator*() const noexcept;

				private:
					/**
					 * Constructor
					 */
					Const_Iterator() noexcept								= default;

					/**
					 * Internal iterator to original data
					 */
					std::list<Item>::const_iterator m_it;
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

		private:
			/**
			 * Ordered items vector
			 */
			std::list<Item> 			m_ordered;

			/**
			 * Looks up a child by path
			 * @param path path to child
			 * @throw ItemNotFound if not found
			 * @return reference to found Item
			 */
			Item&						LookUp(const std::string& path);
			/**
			 * Looks up a child by path
			 * @param path path to child
			 * @throw ItemNotFound if not found
			 * @return const reference to found Item
			 */
			const Item&					LookUp(const std::string& path) const;
			/**
			 * Internal version for Lookup
			 * @param queue queue for path representation
			 * @return const reference to found Item
			 * @throw runtime_error
			 */
			const Item&					LookUp(std::queue<std::string>& queue) const;
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
			 * Internal function for removing an item by a queue of its path
			 * @param path path to item
			 * @throw std::runtime_error if item is not found
			 */
			void						Remove(std::queue<std::string>& path);
	};
}
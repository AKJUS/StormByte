#pragma once

#include <StormByte/config/item.hxx>

#include <vector>
#include <queue>

/**
 * @namespace StormByte::Config
 * @brief All the classes for handling configuration files and items
 */
namespace StormByte::Config {
	/**
	 * @class List
	 * @brief List in configuration item which can hold other items and also subgroups recursivelly
	 * @code
	 * include_dirs = [
	 * 		"/usr/include",
	 * 		"/usr/local/include"
	 * ];
	 * @endcode
	 */
	class STORMBYTE_PUBLIC List final {
		public:
			/**
			 * Constructor
			 */
			List()								= default;
			/**
			 * Copy constructor
			 */
			List(const List&)					= default;
			/**
			 * Move constructor
			 */
			List(List&&) noexcept				= default;
			/**
			 * Assignment operator
			 */
			List& operator=(const List&)		= default;
			/**
			 * Move assignment operator
			 */
			List& operator=(List&&) noexcept	= default;
			/**
			 * Destructor
			 */
			~List() noexcept					= default;

			/**
			 * Gets item reference by index
			 * @param path path to item
			 * @return item
			 * @throw OutOfBounds if item index is out of bounds
			 */
			Item&							operator[](const size_t& idx);
			/**
			 * Gets item const reference by index
			 * @param path path to item
			 * @return item
			 * @throw OutOfBounds if item index is out of bounds
			 */
			const Item&						operator[](const size_t& idx) const;

			/**
			 * Add a copy of an already created item to this group
			 * @param item item to add
			 * @return a pointer to the added item
			 */
			Item&							Add(const Item& item);
			/**
			 * Move an already created item to this group
			 * @param item item to add
			 * @param on_clash function to execute when name already exists
			 * @throw InvalidName is thrown when item name is not allowed
			 * @throw NamedItemNameAlreadyExists is thrown when item name already exists
			 * @return a pointer to the added item
			 */
			Item&							Add(Item&& item);
			/**
			 * Adds a new comment to this group
			 * @param value comment value
			 */
			void							AddComment(const std::string& value);
			/**
			 * Removes an item from this group
			 * @param index item index to remove
			 * @throw OutOfBounds if item index is out of bounds
			 */
			void							Remove(const size_t& index);

			/**
			 * Serializes the boolean item
			 * @param indent_level intentation level
			 * @return serialized string
			 */
			std::string						Serialize(const int& indent_level) const noexcept;

			/**
			 * @class Iterator
			 * @brief List iterator
			 */
			class STORMBYTE_PUBLIC Iterator {
				friend class List;
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
					std::vector<Item>::iterator m_it;
			};

			/**
			 * @class Const_Iterator
			 * @brief Constant group iterator
			 */
			class STORMBYTE_PUBLIC Const_Iterator {
				friend class List;
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
					std::vector<Item>::const_iterator m_it;
			};

			/**
			 * Gets an Iterator pointing to first element
			 * @return Iterator
			 */
			Iterator 						Begin() noexcept;
			/**
			 * Gets a Const_Iterator pointing to first element
			 * @return Const_Iterator
			 */
			Const_Iterator					Begin() const noexcept;
			/**
			 * Gets an Iterator pointing past last element
			 * @return Iterator
			 */
			Iterator 						End() noexcept;
			/**
			 * Gets a Const_Iterator pointing past last element
			 */
			Const_Iterator 					End() const noexcept;

		private:
			/**
			 * Ordered items vector
			 */
			std::vector<Item> 				m_ordered;
	};
}
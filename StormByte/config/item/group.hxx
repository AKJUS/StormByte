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
			std::shared_ptr<Item>		Add(const std::string&, const Type& type);
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
			 * Serializes the string item
			 * @return serialized string
			 */
			std::string					Serialize(const int&) const noexcept override;

			class STORMBYTE_PUBLIC Iterator {
				friend class Group;
				public:
					Iterator(const Iterator&)					= default;
					Iterator(Iterator&&) noexcept				= default;
					Iterator& operator=(const Iterator&)		= default;
					Iterator& operator=(Iterator&&) noexcept	= default;
					~Iterator() noexcept 						= default;

					Iterator& operator++() noexcept;
					Iterator operator++(int) noexcept;
					Iterator& operator--() noexcept;
					Iterator operator--(int) noexcept;
					bool operator==(const Iterator&) const noexcept;
					bool operator!=(const Iterator&) const noexcept;
					Item* operator->() noexcept;
					Item operator*() noexcept = delete;

				private:
					Iterator() noexcept							= default;

					GroupStorage::iterator m_it;
			};

			class STORMBYTE_PUBLIC Const_Iterator {
				friend class Group;
				public:
					Const_Iterator(const Const_Iterator&)					= default;
					Const_Iterator(Const_Iterator&&) noexcept				= default;
					Const_Iterator& operator=(const Const_Iterator&)		= default;
					Const_Iterator& operator=(Const_Iterator&&) noexcept	= default;
					~Const_Iterator() noexcept 						= default;

					Const_Iterator& operator++() noexcept;
					Const_Iterator operator++(int) noexcept;
					Const_Iterator& operator--() noexcept;
					Const_Iterator operator--(int) noexcept;
					bool operator==(const Const_Iterator&) const noexcept;
					bool operator!=(const Const_Iterator&) const noexcept;
					const Item* operator->() const noexcept;
					const Item operator*() const noexcept = delete;

				private:
					Const_Iterator() noexcept								= default;

					GroupStorage::const_iterator m_it;
			};

			Iterator 					Begin() noexcept;
			Const_Iterator				Begin() const noexcept;
			Iterator 					End() noexcept;
			Const_Iterator 				End() const noexcept;
			Const_Iterator 				CBegin() const noexcept;
			Const_Iterator 				CEnd() const noexcept;

		private:
			std::shared_ptr<Item>		Clone() override;
			std::shared_ptr<Item>		LookUp(std::queue<std::string>&) const;
			bool						Exists(std::queue<std::string>&) const noexcept;
			std::queue<std::string> 	ExplodePath(const std::string&) const noexcept;

			std::map<std::string, std::shared_ptr<Item>>	m_children;
			std::vector<std::shared_ptr<Item>> 				m_ordered;
	};
}
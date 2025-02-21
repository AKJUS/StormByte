#pragma once

#include <StormByte/database/value.hxx>
#include <StormByte/util/iterator.hxx>

#include <map>

/**
 * @namespace Database
 * @brief Contains classes and functions for database operations.
 */
namespace StormByte::Database {
	/**
	 * @class Row
	 * @brief Row class for databases
	 */
	class STORMBYTE_PUBLIC Row {
		public:
			using Storage = std::map<std::string, std::unique_ptr<Value>>;	///< Shortcut alias for internal storage
			using Iterator = Util::Iterator<Storage>;						///< Iterator for Row
			using ConstIterator = Util::ConstIterator<Storage>;				///< ConstIterator for Row

			/**
			 * Constructor
			 */
			Row() noexcept 										= default;

			/**
			 * Copy Constructor
			 */
			Row(const Row&)										= default;

			/**
			 * Move Constructor
			 */
			Row(Row&&) noexcept									= default;

			/**
			 * Assignment operator
			 */
			Row& operator=(const Row&)							= default;

			/**
			 * Move operator
			 */
			Row& operator=(Row&&) noexcept						= default;

			/**
			 * Destructor
			 */
			virtual ~Row() noexcept								= default;

			/**
			 * Adds a value to the row (or gets a non const reference to it)
			 * @param columnName column name
			 * @param value value
			 */
			Value& 												operator[](const std::string& columnName);

			/**
			 * Gets value for column
			 * @param columnName column name
			 * @throw ColumnNotFound if column is not found
			 * @return Value
			 */
			const Value& 										operator[](const std::string& columnName) const;

			/**
			 * Checks if result is empty
			 */
			inline 												operator bool() const noexcept {
				return m_values.size() > 0;
			}

			/**
			 * Gets an iterator pointing to the first element
			 * @return Iterator
			 */
			Iterator 											Begin() noexcept;

			/**
			 * Gets a ConstIterator pointing to the first element
			 * @return ConstIterator
			 */
			ConstIterator 										Begin() const noexcept;

			/**
			 * Gets an iterator pointing to past last element
			 * @return Iterator
			 */
			Iterator 											End() noexcept;

			/**
			 * Gets a ConstIterator pointing to past last element
			 * @return ConstIterator
			 */
			ConstIterator 										End() const noexcept;

		protected:
			/**
			 * Internal storage
			 */
			Storage m_values; //< Internal storage
	};
}
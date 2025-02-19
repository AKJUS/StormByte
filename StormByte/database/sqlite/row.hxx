#pragma once

#include <StormByte/visibility.h>

#include <map>
#include <memory>
#include <vector>
#include <string>

/**
 * @namespace Database::SQLite
 * @brief All the classes for handling SQLite databases
 */
namespace StormByte::Database::SQLite {
	class Result;
	/**
	 * @class Row
	 * @brief Row class for SQLite
	 */
	class STORMBYTE_PUBLIC Row {
		friend class PreparedSTMT;
		public:
			/**
			 * Constructor
			 */
			Row()							= default;

			/**
			 * Copy constructor
			 */
			Row(const Row&)					= default;

			/**
			 * Move constructor
			 */
			Row(Row&&)						= default;

			/**
			 * Assignment operator
			 */
			Row& operator=(const Row&)		= default;

			/**
			 * Move operator
			 */
			Row& operator=(Row&&) noexcept	= default;

			/**
			 * Destructor
			 */
			~Row() noexcept					= default;

			/**
			 * Gets the number of columns
			 * @return number of columns
			 */
			size_t 							Columns() const noexcept;

			/**
			 * Gets the column result given its index
			 * @return column result
			 */
			const std::shared_ptr<Result>	operator[](const size_t&) const;

			/**
			 * Gets the column result given its name
			 * @return column result
			 */
			const std::shared_ptr<Result>	operator[](const std::string&) const;

			/**
			 * Gets the column result given its index
			 * @return column result
			 */
			const std::shared_ptr<Result>	At(const size_t&) const;

			/**
			 * Gets the column result given its name
			 * @return column result
			 */
			const std::shared_ptr<Result>	At(const std::string&) const;

		private:
			/**
			 * Adds a column to the row
			 * @param name column name
			 * @param res column result
			 */
			void add(std::string&& name, std::shared_ptr<Result> res);

			/**
			 * Gets the column result given its index
			 * @return column result
			 * @throws OutOfBounds if the index is out of bounds
			 */
			std::shared_ptr<Result> get(const size_t&);

			/**
			 * Gets the column result given its index
			 * @return column result
			 * @throws OutOfBounds if the index is out of bounds
			 */
			const std::shared_ptr<Result> get(const size_t&) const;

			/**
			 * Gets the column result given its name
			 * @return column result
			 * @throws ColumnNotFound if the column is not found
			 */
			std::shared_ptr<Result> get(const std::string&);

			/**
			 * Gets the column result given its name
			 * @return column result
			 * @throws ColumnNotFound if the column is not found
			 */
			const std::shared_ptr<Result> get(const std::string&) const;

			/**
			 * Gets the column result given its index
			 * @return column result
			 * @throws OutOfBounds if the index is out of bounds
			 */
			std::map<std::string, std::shared_ptr<Result>> m_column_name_assoc;

			/**
			 * Gets the column result given its name
			 * @return column result
			 * @throws ColumnNotFound if the column is not found
			 */
			std::map<size_t, std::shared_ptr<Result>> m_column_pos_assoc;
	};
}

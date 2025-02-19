#pragma once

#include <StormByte/database/sqlite/row.hxx>

#include <cstdint>
#include <memory>
#include <optional>
#include <string>

class sqlite3_stmt; // Forward declaration so we don't have to depend on sqlite3 headers
/**
 * @namespace Database::SQLite
 * @brief All the classes for handling SQLite databases
 */
namespace StormByte::Database::SQLite {
	/**
	 * @class PreparedSTMT
	 * @brief Prepared statement for SQLite
	 */
	class STORMBYTE_PUBLIC PreparedSTMT {
		friend class SQLite3;
		public:
			/**
			 * Copy constructor
			 */
			PreparedSTMT(const PreparedSTMT&) 					= delete;

			/**
			 * Move constructor
			 */
			PreparedSTMT(PreparedSTMT&&) noexcept				= default;

			/**
			 * Assignment operator
			 */
			PreparedSTMT& operator=(const PreparedSTMT&)		= delete;

			/**
			 * Move operator
			 */
			PreparedSTMT& operator=(PreparedSTMT&&) noexcept	= default;

			/**
			 * Destructor
			 */
			~PreparedSTMT() noexcept;

			/**
			 * Binds a value to a prepared statement
			 * @param index parameter index
			 * @param value nullptr
			 */
			void 					Bind(const int& index, const void*) noexcept; // Sintactic sugar for bind NULL

			/**
			 * Binds a value to a prepared statement
			 * @param index parameter index
			 * @param value integer
			 */
			void					Bind(const int& index, const std::optional<int64_t>& value) noexcept;

			/**
			 * Binds a value to a prepared statement
			 * @param index parameter index
			 * @param value double
			 */
			void 					Bind(const int& index, const std::optional<std::string>& value) noexcept;


			/**
			 * Resets the prepared statement
			 */
			void 					Reset() noexcept;

			/**
			 * Executes the prepared statement
			 * @return row pointer (nullptr if there are no more rows)
			 */
			std::shared_ptr<Row> 	Step();

		private:
			/**
			 * Constructor
			 * @param query query
			 */
			PreparedSTMT(const std::string& query);

			/**
			 * Constructor
			 * @param query query
			 */
			PreparedSTMT(std::string&& query) noexcept;

			/**
			 * Query string
			 */
			std::string m_query;

			/**
			 * SQLite3 statement
			 */
			sqlite3_stmt* m_stmt;
	};
}

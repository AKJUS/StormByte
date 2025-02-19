#pragma once

#include <StormByte/visibility.h>

#include <filesystem>
#include <list>
#include <map>
#include <memory>

class sqlite3; // Forward declaration so we don't have to depend on sqlite3 headers
/**
 * @namespace Database::SQLite
 * @brief All the classes for handling SQLite databases
 */
namespace StormByte::Database::SQLite {
	class PreparedSTMT;
	class Row;
	/**
	 * @class SQLite3
	 * @brief SQLite3 database class
	 */
	class STORMBYTE_PUBLIC SQLite3 {
		public:
			/**
			 * Copy constructor
			 */
			SQLite3(const SQLite3& db) 					= delete;

			/**
			 * Move constructor
			 */
			SQLite3(SQLite3&& db) noexcept				= default;

			/**
			 * Assignment operator
			 */
			SQLite3& operator=(const SQLite3& db) 		= delete;

			/**
			 * Move operator
			 */
			SQLite3& operator=(SQLite3&& db) noexcept 	= default;

			/**
			 * Destructor
			 */
			virtual ~SQLite3() noexcept;

		protected:
			/**
			 * Constructor
			 */
			SQLite3();

			/**
			 * Constructor
			 * @param dbfile database file
			 */
			SQLite3(const std::filesystem::path& dbfile);

			/**
			 * Constructor
			 * @param dbfile database file
			 */
			SQLite3(std::filesystem::path&& dbfile);

			/**
			 * Initializes the database
			 */
			void 							init_database();

			/**
			 * Begins a transaction
			 */
			void 							begin_transaction();

			/**
			 * Begins an exclusive transaction
			 */
			void 							begin_exclusive_transaction();

			/**
			 * Commits the transaction
			 */
			void 							commit_transaction();

			/**
			 * Rolls back the transaction
			 */
			void 							rollback_transaction();

			/**
			 * Prepares a sentence
			 * @param query query
			 * @param name name
			 * @return prepared statement
			 */
			std::shared_ptr<PreparedSTMT>	prepare_sentence(const std::string& query, const std::string& name);

			/**
			 * Gets a prepared statement
			 * @param name name
			 * @return prepared statement
			 */
			std::shared_ptr<PreparedSTMT>	get_prepared(const std::string& name);

			/**
			 * Executes a query ignoring its results
			 * @param query query
			 */
			void							silent_query(const std::string& query);

			/**
			 * Gets last error
			 * @return std::string last error
			 */
			const std::string				last_error();

		private:
			/**
			 * Database file
			 */
			std::filesystem::path m_database_file;	///< Database file

			/**
			 * SQLite3 database
			 * (can not use std::unique_ptr because sqlite3 is an incomplete type)
			 */
			sqlite3* m_database;					///< SQLite3 database

			/**
			 * Prepared statements
			 */
			std::map<std::string, std::shared_ptr<PreparedSTMT>> m_prepared;	///< Prepared statements

			/* Database internals */
			/**
			 * Action to execute after database connection
			 */
			virtual void post_init_action() noexcept = 0;

			/**
			 * Closes the database
			 */
			void close_database();

			/**
			 * Enable the foreign keys for SQLite3 (default is disabled)
			 */
			void enable_foreign_keys();
	};
}

#pragma once

#include <StormByte/database/preparedSTMT.hxx>
#include <StormByte/database/query.hxx>

/**
 * @namespace Database
 * @brief Contains classes and functions for database operations.
 */
namespace StormByte::Database {
	class STORMBYTE_PUBLIC Database {
		public:
			/**
			 * Default constructor.
			 */
			Database()											= default;

			/**
			 * Default copy constructor (deleted)
			 */
			Database(const Database&)							= delete;

			/**
			 * Default move constructor
			 */
			Database(Database&&)								= default;

			/**
			 * Default copy assignment operator (deleted)
			 */
			Database& operator=(const Database&)				= delete;

			/**
			 * Default move assignment operator
			 */
			Database& operator=(Database&&)						= default;

			/**
			 * Default destructor.
			 */
			virtual ~Database()									= default;

			/**
			 * Connects to the database.
			 */
			virtual void 										Connect()		= 0;

			/**
			 * Disconnects from the database.
			 */
			virtual void 										Disconnect()	= 0;

		protected:
			std::map<std::string, std::unique_ptr<PreparedSTMT>> m_prepared_stmts;	///< Prepared statements

			/**
			 * Executes a query
			 * @param query The query to execute.
			 * @return The created query
			 */
			virtual std::unique_ptr<Query>						Query(const std::string& query) = 0;

			/**
			 * Executes a query without returning any result
			 * @param query The query to execute.
			 */
			void 												SilentQuery(const std::string& query);

			/**
			 * Prepares a statement
			 * @param name The name of the prepared statement
			 * @param query The query to prepare
			 * @return The created prepared statement
			 */
			virtual void 										Prepare(const std::string& name, const std::string& query) = 0;

			/**
			 * Prepares all the statements
			 * @param queries The queries to prepare
			 */
			void 												PrepareAll(const std::map<std::string, std::string>& queries);

			/**
			 * Gets a prepared statement
			 * @param name The name of the prepared statement
			 * @return The prepared statement
			 */
			PreparedSTMT& 										GetPreparedSTMT(const std::string& name) const;

			/**
			 * Begins a transaction
			 */
			virtual void 										BeginTransaction() = 0;

			/**
			 * Begins an exclusive transaction
			 */
			virtual void 										BeginExclusiveTransaction() = 0;

			/**
			 * Commits the transaction
			 */
			virtual void 										CommitTransaction() = 0;

			/**
			 * Rolls back the transaction
			 */
			virtual void 										RollbackTransaction() = 0;
	};
}
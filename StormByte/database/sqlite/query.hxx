#pragma once

#include <StormByte/database/preparedSTMT.hxx>
#include <StormByte/database/query.hxx>

/**
 * @namespace SQLite3
 * @brief Contains classes and functions for SQLite3 database operations.
 */
namespace StormByte::Database::SQLite {
	/**
	 * @class Query
	 * @brief Represents a query to be executed in a SQLite3 database.
	 * In SQLite3 a query is a Prepared Statement so we wrap it
	 */
	class STORMBYTE_PUBLIC Query final: public StormByte::Database::Query {
		friend class SQLite3;
		public:
			/**
			 * Constructor
			 * @param query The query string
			 */
			Query(const std::string& query);

			/**
			 * Constructor
			 * @param query The query string
			 */
			Query(std::string&& query);

			/**
			 * Default copy constructor (deleted)
			 */
			Query(const Query&)								= delete;

			/**
			 * Default move constructor
			 */
			constexpr Query(Query&&)						= default;

			/**
			 * Default copy assignment operator (deleted)
			 */
			Query& operator=(const Query&)					= delete;

			/**
			 * Default move assignment operator
			 */
			constexpr Query& operator=(Query&&)				= default;

			/**
			 * Default destructor.
			 */
			~Query() override								= default;

			/**
			 * Step into the query results
			 */
			const Row&										Step() noexcept override;

		private:
			/**
			 * The SQLite3 statement
			 */
			std::unique_ptr<StormByte::Database::PreparedSTMT> m_stmt; //< The SQLite3 statement
	};
}
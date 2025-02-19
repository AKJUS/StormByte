#pragma once

#include <StormByte/database/sqlite/type.hxx>
#include <StormByte/exception.hxx>

#include <cstdint>

/**
 * @namespace Database::SQLite
 * @brief All the classes for handling SQLite databases
 */
namespace StormByte::Database::SQLite {
	/**
	 * @class Exception
	 * @brief Exception base class for SQLite
	 */
	class STORMBYTE_PUBLIC Exception: public StormByte::Exception {
		public:
			/**
			 * Constructor
			 * @param message message
			 */
			Exception(const std::string& message);

			/**
			 * Copy constructor
			 */
			Exception(const Exception&)								= default;

			/**
			 * Move constructor
			 */
			Exception(Exception&&) noexcept 						= default;

			/**
			 * Assignment operator
			 */
			Exception& operator=(const Exception&)					= default;

			/**
			 * Move operator
			 */
			Exception& operator=(Exception&&) noexcept				= default;

			/**
			 * Destructor
			 */
			virtual ~Exception() noexcept override 					= default;
	};

	/**
	 * @class WrongResultType
	 * @brief Exception for wrong result type
	 */
	class STORMBYTE_PUBLIC WrongResultType: public Exception {
		public:
			/**
			 * Constructor
			 * @param source_type source type
			 * @param asked_type asked type
			 */
			WrongResultType(const Type& source_type, const Type& asked_type);
			
			/**
			 * Copy constructor
			 */
			WrongResultType(const WrongResultType&)					= default;

			/**
			 * Move constructor
			 */
			WrongResultType(WrongResultType&&) noexcept				= default;

			/**
			 * Assignment operator
			 */
			WrongResultType& operator=(const WrongResultType&)		= default;

			/**
			 * Move operator
			 */
			WrongResultType& operator=(WrongResultType&&) noexcept 	= default;

			/**
			 * Destructor
			 */
			~WrongResultType() noexcept override					= default;
	};

	/**
	 * @class Overflow
	 * @brief Exception for overflow
	 */
	class STORMBYTE_PUBLIC Overflow: public Exception {
		public:
			/**
			 * Constructor
			 * @param source source value
			 */
			Overflow(const int64_t& source);

			/**
			 * Copy constructor
			 */
			Overflow(const Overflow&)					= default;

			/**
			 * Move constructor
			 */
			Overflow(Overflow&&) noexcept				= default;

			/**
			 * Assignment operator
			 */
			Overflow& operator=(const Overflow&)		= default;

			/**
			 * Move operator
			 */
			Overflow& operator=(Overflow&&) noexcept 	= default;

			/**
			 * Destructor
			 */
			~Overflow() noexcept override				= default;
	};

	/**
	 * @class ColumnNotFound
	 * @brief Exception when accessing a not found column
	 */
	class STORMBYTE_PUBLIC ColumnNotFound: public Exception {
		public:
			/**
			 * Constructor
			 * @param max maximum number of columns
			 * @param asked asked column number
			 */
			ColumnNotFound(const size_t& max, const size_t& asked);

			/**
			 * Constructor
			 * @param name column name
			 */
			ColumnNotFound(const std::string& name);

			/**
			 * Copy constructor
			 */
			ColumnNotFound(const ColumnNotFound&)					= default;

			/**
			 * Move constructor
			 */
			ColumnNotFound(ColumnNotFound&&) noexcept				= default;

			/**
			 * Assignment operator
			 */
			ColumnNotFound& operator=(const ColumnNotFound&)		= default;

			/**
			 * Move operator
			 */
			ColumnNotFound& operator=(ColumnNotFound&&) noexcept 	= default;

			/**
			 * Destructor
			 */
			~ColumnNotFound() noexcept override						= default;
	};

	/**
	 * @class ConnectionError
	 * @brief Exception for connection error
	 */
	class STORMBYTE_PUBLIC ConnectionError: public Exception {
		public:
			/**
			 * Constructor
			 * @param reason reason
			 */
			ConnectionError(const std::string& reason);

			/**
			 * Constructor
			 * @param reason reason
			 */
			ConnectionError(std::string&& reason);

			/**
			 * Copy constructor
			 */
			ConnectionError(const ConnectionError&)					= default;

			/**
			 * Move constructor
			 */
			ConnectionError(ConnectionError&&) noexcept				= default;

			/**
			 * Assignment operator
			 */
			ConnectionError& operator=(const ConnectionError&)		= default;

			/**
			 * Move operator
			 */
			ConnectionError& operator=(ConnectionError&&) noexcept 	= default;

			/**
			 * Destructor
			 */
			~ConnectionError() noexcept override					= default;
	};

	/**
	 * @class QueryError
	 * @brief Exception for query error
	 */
	class STORMBYTE_PUBLIC QueryError: public Exception {
		public:
			/**
			 * Constructor
			 * @param reason reason
			 */
			QueryError(const std::string& reason);

			/**
			 * Constructor
			 * @param reason reason
			 */
			QueryError(std::string&& reason);

			/**
			 * Copy constructor
			 */
			QueryError(const QueryError&)					= default;

			/**
			 * Move constructor
			 */
			QueryError(QueryError&&) noexcept				= default;

			/**
			 * Assignment operator
			 */
			QueryError& operator=(const QueryError&)		= default;

			/**
			 * Move operator
			 */
			QueryError& operator=(QueryError&&) noexcept 	= default;

			/**
			 * Destructor
			 */
			~QueryError() noexcept override					= default;
	};
}

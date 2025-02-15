#pragma once

#ifdef STORMBYTE_ENABLE_SQLITE
	#include <StormByte/database/sqlite/type.hxx>
	#include <StormByte/system/exception.hxx>

	#include <cstdint>

	namespace StormByte::Database::SQLite {
		class STORMBYTE_PUBLIC Exception: public System::Exception {
			public:
				Exception(const std::string&);
				Exception(std::string&&) noexcept;
				Exception(const Exception&)								= default;
				Exception(Exception&&) noexcept 						= default;
				Exception& operator=(const Exception&)					= default;
				Exception& operator=(Exception&&) noexcept				= default;
				virtual ~Exception() noexcept override 					= default;
		};

		class STORMBYTE_PUBLIC WrongResultType: public Exception {
			public:
				WrongResultType(const Type&, const Type&);
				WrongResultType(const WrongResultType&)					= default;
				WrongResultType(WrongResultType&&) noexcept				= default;
				WrongResultType& operator=(const WrongResultType&)		= default;
				WrongResultType& operator=(WrongResultType&&) noexcept 	= default;
				~WrongResultType() noexcept override					= default;
		};

		class STORMBYTE_PUBLIC Overflow: public Exception {
			public:
				Overflow(const int64_t&);
				Overflow(const Overflow&)					= default;
				Overflow(Overflow&&) noexcept				= default;
				Overflow& operator=(const Overflow&)		= default;
				Overflow& operator=(Overflow&&) noexcept 	= default;
				~Overflow() noexcept override				= default;
		};

		class STORMBYTE_PUBLIC OutOfBounds: public Exception {
			public:
				OutOfBounds(const size_t&, const size_t&);
				OutOfBounds(const std::string&);
				OutOfBounds(const OutOfBounds&)					= default;
				OutOfBounds(OutOfBounds&&) noexcept				= default;
				OutOfBounds& operator=(const OutOfBounds&)		= default;
				OutOfBounds& operator=(OutOfBounds&&) noexcept 	= default;
				~OutOfBounds() noexcept override				= default;
		};

		class STORMBYTE_PUBLIC ConnectionError: public Exception {
			public:
				ConnectionError(const std::string&);
				ConnectionError(std::string&&);
				ConnectionError(const ConnectionError&)					= default;
				ConnectionError(ConnectionError&&) noexcept				= default;
				ConnectionError& operator=(const ConnectionError&)		= default;
				ConnectionError& operator=(ConnectionError&&) noexcept 	= default;
				~ConnectionError() noexcept override					= default;
		};

		class STORMBYTE_PUBLIC QueryError: public Exception {
			public:
				QueryError(const std::string&);
				QueryError(std::string&&);
				QueryError(const QueryError&)					= default;
				QueryError(QueryError&&) noexcept				= default;
				QueryError& operator=(const QueryError&)		= default;
				QueryError& operator=(QueryError&&) noexcept 	= default;
				~QueryError() noexcept override					= default;
		};
	}
#endif
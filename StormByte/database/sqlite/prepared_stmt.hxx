#pragma once

#include <StormByte/database/sqlite/row.hxx>

#include <cstdint>
#include <memory>
#include <optional>
#include <string>

class sqlite3_stmt;
namespace StormByte::Database::SQLite {
	class STORMBYTE_PUBLIC PreparedSTMT {
		friend class SQLite3;
		public:
			PreparedSTMT(const PreparedSTMT&) 					= delete;
			PreparedSTMT(PreparedSTMT&&) noexcept				= default;
			PreparedSTMT& operator=(const PreparedSTMT&)		= delete;
			PreparedSTMT& operator=(PreparedSTMT&&) noexcept	= default;
			~PreparedSTMT() noexcept;

			void 					Bind(const int&, const void*) noexcept; // Sintactic sugar for bind NULL
			void					Bind(const int&, const std::optional<int64_t>&) noexcept;
			void 					Bind(const int&, const std::optional<std::string>&) noexcept;

			void 					Reset() noexcept;
			std::shared_ptr<Row> 	Step();

		private:
			PreparedSTMT(const std::string&);
			PreparedSTMT(std::string&&) noexcept;

			std::string m_query;
			sqlite3_stmt* m_stmt;
	};
}

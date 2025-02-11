#include <StormByte/database/sqlite/prepared_stmt.hxx>
#include <StormByte/database/sqlite/result.hxx>

#include <limits>
#include <sqlite3.h>

using namespace StormByte::Database::SQLite;

PreparedSTMT::PreparedSTMT(const std::string& query):m_query(query), m_stmt(nullptr) {}

PreparedSTMT::PreparedSTMT(std::string&& query) noexcept:m_query(std::move(query)), m_stmt(nullptr) {}

PreparedSTMT::~PreparedSTMT() noexcept {
	if (m_stmt) {
		sqlite3_finalize(m_stmt);
		m_stmt = nullptr;
	}
}

void PreparedSTMT::Bind(const int& column, const void*) noexcept {
	sqlite3_bind_null(m_stmt, column + 1);
}

void PreparedSTMT::Bind(const int& column, const std::optional<int64_t>& val) noexcept {
	if (val) {
		if (*val < std::numeric_limits<int64_t>::max())
			sqlite3_bind_int(m_stmt, column + 1, *val);
		else
			sqlite3_bind_int64(m_stmt, column + 1, *val);
	}
	else
		Bind(column, nullptr);
}

void PreparedSTMT::Bind(const int& column, const std::optional<std::string>& val) noexcept {
	if (val)
		sqlite3_bind_text(m_stmt, column + 1, val->c_str(), -1, SQLITE_STATIC);
	else
		Bind(column, nullptr);
}

void PreparedSTMT::Reset() noexcept {
	sqlite3_clear_bindings(m_stmt);
	sqlite3_reset(m_stmt);
}

std::shared_ptr<Row> PreparedSTMT::Step() {
	std::shared_ptr<Row> result = nullptr;
	if (sqlite3_step(m_stmt) == SQLITE_ROW) {
		result = std::make_shared<Row>();
		for (auto i = 0; i < sqlite3_column_count(m_stmt); i++) {
			std::shared_ptr<Result> item;
			switch(sqlite3_column_type(m_stmt, i)) {
				case SQLITE_INTEGER:
					item = std::make_shared<Result>((int64_t)sqlite3_column_int(m_stmt, i));
					break;

				case SQLITE_TEXT:
					item = std::make_shared<Result>(std::string(reinterpret_cast<const char*>(sqlite3_column_text(m_stmt, i))));
					break;

				case SQLITE_NULL:
					item = std::make_shared<Result>(nullptr);
					break;

				case SQLITE_FLOAT:
					item = std::make_shared<Result>(sqlite3_column_double(m_stmt, i));
					break;

				default:
					/* Not implemented all the cases */
					item = nullptr;
			}
			result->add(std::string(sqlite3_column_name(m_stmt, i)), item);
		}
	}
	return result;
}
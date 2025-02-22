#include <StormByte/database/sqlite/query.hxx>

using namespace StormByte::Database::SQLite;

Query::Query(const std::string& query):StormByte::Database::Query(query) {}

const StormByte::Database::Row& Query::Step() noexcept {
	return m_stmt->Step();
}
#include <StormByte/database/database.hxx>

using namespace StormByte::Database;

void Database::BeginTransaction() {
	SilentQuery("BEGIN TRANSACTION");
}

void Database::BeginExclusiveTransaction() {
	SilentQuery("BEGIN EXCLUSIVE TRANSACTION");
}

void Database::CommitTransaction() {
	SilentQuery("COMMIT");
}

void Database::RollbackTransaction() {
	SilentQuery("ROLLBACK");
}

void Database::Prepare(const std::string& name, const std::string& query) {
	m_prepared_stmts.insert({name, InternalPrepare(name, query)});
}

void Database::PrepareAll(const std::map<std::string, std::string>& queries) {
	for (const auto& [name, query]: queries) {
		Prepare(name, query);
	}
}

PreparedSTMT& Database::GetPreparedSTMT(const std::string& name) const {
	return *m_prepared_stmts.at(name);
}

std::unique_ptr<Query> Database::Query(const std::string& query) {
	return InternalQuery(query);
}

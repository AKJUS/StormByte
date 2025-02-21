#include <StormByte/database/database.hxx>

using namespace StormByte::Database;

void Database::PrepareAll(const std::map<std::string, std::string>& queries) {
	for (const auto& [name, query]: queries) {
		Prepare(name, query);
	}
}

PreparedSTMT& Database::GetPreparedSTMT(const std::string& name) const {
	return *m_prepared_stmts.at(name);
}

void Database::SilentQuery(const std::string& query) {
	Query(query)->Step();
}
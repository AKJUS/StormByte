#include <StormByte/database/sqlite/exception.hxx>
#include <StormByte/database/sqlite/prepared_stmt.hxx>
#include <StormByte/database/sqlite/sqlite3.hxx>

#include <sqlite3.h>

using namespace StormByte::Database::SQLite;

SQLite3::SQLite3():m_database_file(":memory:") {}

SQLite3::SQLite3(const std::filesystem::path& dbfile):m_database_file(dbfile) {}

SQLite3::SQLite3(std::filesystem::path&& dbfile):m_database_file(std::move(dbfile)) {}

SQLite3::~SQLite3() noexcept { close_database(); }

void SQLite3::init_database() {
	// This is undefined behavior if called more than once for the same object
	// Windows needs this string intermediate conversion
	if (sqlite3_open(m_database_file.string().c_str(), &m_database) != SQLITE_OK) {
		std::string message = "Cannot open database " + m_database_file.string() + ": " + last_error(); // SQLite3 handles internally freeing message's memory
		close_database(); // Need to close database here as exception throwing might skip destructor
        throw ConnectionError(std::move(message));
    }
	enable_foreign_keys();
	this->post_init_action();
}

void SQLite3::close_database() {
	if (m_database) {
		m_prepared.clear();
		sqlite3_close(m_database);
		m_database = nullptr;
	}
}

void SQLite3::enable_foreign_keys() {
	silent_query("PRAGMA foreign_keys = OFF");
}

void SQLite3::begin_transaction() {
	silent_query("BEGIN TRANSACTION");
}

void SQLite3::begin_exclusive_transaction() {
	silent_query("BEGIN EXCLUSIVE TRANSACTION");
}

void SQLite3::commit_transaction() {
	silent_query("COMMIT");
}

void SQLite3::rollback_transaction() {
	silent_query("ROLLBACK");
}

std::shared_ptr<PreparedSTMT> SQLite3::prepare_sentence(const std::string& name, const std::string& query) {
	std::shared_ptr<PreparedSTMT> stmt = std::make_shared<PreparedSTMT>(PreparedSTMT(query));
	sqlite3_prepare_v2( m_database, stmt->m_query.c_str(), static_cast<int>(stmt->m_query.length()), &stmt->m_stmt, nullptr);
	if (!stmt->m_stmt)
		throw QueryError("Prepared sentence " + name + " can not be loaded\n" + last_error());
	else
		m_prepared.insert({ name, stmt });
	return stmt;
}

std::shared_ptr<PreparedSTMT> SQLite3::get_prepared(const std::string& name) {
	std::shared_ptr<PreparedSTMT> stmt = nullptr;
	if (m_prepared.find(name) != m_prepared.end())
		stmt = m_prepared.at(name);
	return stmt;
}

void SQLite3::silent_query(const std::string& query) {
	char* error;
	if (sqlite3_exec(m_database, query.c_str(), nullptr, nullptr, &error) != SQLITE_OK) {
		std::string err { error };
		sqlite3_free(error);
		throw QueryError(std::move(err));
	}
}

const std::string SQLite3::last_error() {
	return sqlite3_errmsg(m_database);
}


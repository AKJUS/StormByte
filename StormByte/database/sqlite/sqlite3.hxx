#pragma once

#ifdef STORMBYTE_ENABLE_SQLITE
	#include <StormByte/visibility.h>

	#include <filesystem>
	#include <list>
	#include <map>
	#include <memory>

	class sqlite3;
	namespace StormByte::Database::SQLite {
		class PreparedSTMT;
		class Row;
		class STORMBYTE_PUBLIC SQLite3 {
			public:
				SQLite3(const SQLite3& db) 					= delete;
				SQLite3(SQLite3&& db) noexcept				= default;
				SQLite3& operator=(const SQLite3& db) 		= delete;
				SQLite3& operator=(SQLite3&& db) noexcept 	= default;
				virtual ~SQLite3() noexcept;

			protected:
				SQLite3();
				SQLite3(const std::filesystem::path& dbfile);
				SQLite3(std::filesystem::path&& dbfile);

				void 							init_database();
				void 							begin_transaction();
				void 							begin_exclusive_transaction();
				void 							commit_transaction();
				void 							rollback_transaction();
				std::shared_ptr<PreparedSTMT>	prepare_sentence(const std::string&, const std::string&);
				std::shared_ptr<PreparedSTMT>	get_prepared(const std::string&);
				void							silent_query(const std::string&);
				const std::string				last_error();

			private:
				std::filesystem::path m_database_file;
				sqlite3* m_database;
				std::map<std::string, std::shared_ptr<PreparedSTMT>> m_prepared;

				/* Database internals */
				virtual void post_init_action() noexcept = 0;
				void close_database();
				void enable_foreign_keys();
		};
	}
 #endif
#pragma once

#ifdef STORMBYTE_ENABLE_SQLITE
	#include <StormByte/visibility.h>

	#include <map>
	#include <memory>
	#include <vector>
	#include <string>

	namespace StormByte::Database::SQLite {
		class Result;
		class STORMBYTE_PUBLIC Row {
			friend class PreparedSTMT;
			public:
				Row()							= default;
				Row(const Row&)					= default;
				Row(Row&&)						= default;
				Row& operator=(const Row&)		= default;
				Row& operator=(Row&&) noexcept	= default;
				~Row() noexcept					= default;

				size_t 							Columns() const noexcept;
				std::shared_ptr<Result>			operator[](const size_t&);
				const std::shared_ptr<Result>	operator[](const size_t&) const;
				std::shared_ptr<Result> 		operator[](const std::string&);
				const std::shared_ptr<Result>	operator[](const std::string&) const;
				std::shared_ptr<Result> 		At(const size_t&);
				const std::shared_ptr<Result>	At(const size_t&) const;
				std::shared_ptr<Result> 		At(const std::string&);
				const std::shared_ptr<Result>	At(const std::string&) const;

			private:
				void add(std::string&&, std::shared_ptr<Result>);
				std::shared_ptr<Result> get(const size_t&);
				const std::shared_ptr<Result> get(const size_t&) const;
				std::shared_ptr<Result> get(const std::string&);
				const std::shared_ptr<Result> get(const std::string&) const;

				std::map<std::string, std::shared_ptr<Result>> m_column_name_assoc;
				std::map<size_t, std::shared_ptr<Result>> m_column_pos_assoc;
		};
	}
#endif
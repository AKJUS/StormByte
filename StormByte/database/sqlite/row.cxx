#include <StormByte/database/sqlite/exception.hxx>
#include <StormByte/database/sqlite/result.hxx>
#include <StormByte/database/sqlite/row.hxx>

using namespace StormByte::Database::SQLite;

void Row::add(std::string&& name, std::shared_ptr<Result> res) {
	size_t curr_pos = m_column_pos_assoc.size();
	m_column_name_assoc.insert({ std::move(name), res });
	m_column_pos_assoc.insert({ std::move(curr_pos), res});
}

size_t Row::Columns() const noexcept {
	return m_column_pos_assoc.size();
}

const std::shared_ptr<Result> Row::operator[](const size_t& pos) const {
	return get(pos);
}

const std::shared_ptr<Result> Row::operator[](const std::string& name) const {
	return get(name);
}

const std::shared_ptr<Result> Row::At(const size_t& pos) const {
	return get(pos);
}

const std::shared_ptr<Result> Row::At(const std::string& name) const {
	return get(name);
}

std::shared_ptr<Result> Row::get(const size_t& pos) {
	if (pos >= m_column_pos_assoc.size())
		throw ColumNotFound(m_column_pos_assoc.size(), pos);

	return m_column_pos_assoc.at(pos);
}

const std::shared_ptr<Result> Row::get(const size_t& pos) const {
	if (pos >= m_column_pos_assoc.size())
		throw ColumNotFound(m_column_pos_assoc.size(), pos);

	return m_column_pos_assoc.at(pos);
}

std::shared_ptr<Result> Row::get(const std::string& name) {
	if (m_column_name_assoc.find(name) == m_column_name_assoc.end())
		throw ColumNotFound(name);

	return m_column_name_assoc.at(name);
}

const std::shared_ptr<Result> Row::get(const std::string& name) const {
	if (m_column_name_assoc.find(name) == m_column_name_assoc.end())
		throw ColumNotFound(name);

	return m_column_name_assoc.at(name);
}

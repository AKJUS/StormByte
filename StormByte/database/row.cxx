#include <StormByte/database/row.hxx>

using namespace StormByte::Database;

Value& Row::operator[](const std::string& columnName) {
	auto it = std::find_if(m_values.begin(), m_values.end(), [&columnName](const Pair& item ) { return item.first == columnName; });
	if (it == m_values.end())
		throw ColumnNotFound(columnName);
	return *(it->second);
}

const Value& Row::operator[](const std::string& columnName) const {
	auto it = std::find_if(m_values.begin(), m_values.end(), [&columnName](const Pair& item ) { return item.first == columnName; });
	if (it == m_values.end())
		throw ColumnNotFound(columnName);
	return *(it->second);
}

Value& Row::operator[](const size_t& columnIndex) {
	if (columnIndex >= m_values.size())
		throw OutOfBounds(columnIndex);
	return *m_values[columnIndex].second;
}

const Value& Row::operator[](const size_t& columnIndex) const {
	if (columnIndex >= m_values.size())
		throw OutOfBounds(columnIndex);
	return *m_values[columnIndex].second;
}

void Row::Add(const std::string& columnName, std::unique_ptr<Value>&& value) {
	m_values.push_back({columnName, std::move(value)});
}

size_t Row::Columns() const noexcept {
	return m_values.size();
}
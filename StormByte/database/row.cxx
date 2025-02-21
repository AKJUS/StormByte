#include <StormByte/database/row.hxx>

using namespace StormByte::Database;

const Value& Row::operator[](const std::string& columnName) const {
	return *m_values.at(columnName);
}

Value& Row::operator[](const std::string& columnName) {
	return *m_values[columnName];
}
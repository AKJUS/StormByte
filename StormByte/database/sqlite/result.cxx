#include <StormByte/database/sqlite/exception.hxx>
#include <StormByte/database/sqlite/result.hxx>

#include <limits>

using namespace StormByte::Database::SQLite;

Result::Result(const int64_t& value):m_type(Type::Integer), m_value(value) {}

Result::Result(const std::string& value):m_type(Type::String), m_value(value) {}

Result::Result(std::string&& value):m_type(Type::String), m_value(std::move(value)) {}

Result::Result(std::nullptr_t):m_type(Type::Null), m_value() {}

Result::Result(const double& value):m_type(Type::Double), m_value(value) {}

Result::Result(double&& value):m_type(Type::Double), m_value(std::move(value)) {}

bool Result::IsNull() const noexcept { return m_type == Type::Null; }

const Type& Result::GetType() const noexcept { return m_type; }

template<> const int& Result::Value<int>() const {
	if (m_type != Type::Integer)
		throw WrongResultType(m_type, Type::Integer);

	if (std::get<int64_t>(m_value) > std::numeric_limits<int>::max())
		throw Overflow(std::get<int64_t>(m_value));

	m_int_conversion = std::get<int64_t>(m_value);

	return m_int_conversion;
}

template<> const int64_t& Result::Value<int64_t>() const {
	if (m_type != Type::Integer)
		throw WrongResultType(m_type, Type::Integer);

	return std::get<int64_t>(m_value);
}

template<> const bool& Result::Value<bool>() const {
	if (m_type != Type::Integer || std::get<int64_t>(m_value) < 0 || std::get<int64_t>(m_value) > 1)
		throw WrongResultType(m_type, Type::Bool);

	m_bool_conversion = std::get<int64_t>(m_value) == 1;

	return m_bool_conversion;
}

template<> const double& Result::Value<double>() const {
	if (m_type != Type::Double)
		throw WrongResultType(m_type, Type::Double);

	if (std::get<int64_t>(m_value) > std::numeric_limits<int>::max())
		throw Overflow(std::get<int64_t>(m_value));

	return std::get<double>(m_value);
}

template<> const std::string& Result::Value<std::string>() const {
	if (m_type != Type::String)
		throw WrongResultType(m_type, Type::String);

	return std::get<std::string>(m_value);
}

#include <StormByte/database/exception.hxx>
#include <StormByte/database/value.hxx>

using namespace StormByte::Database;


template<> const int& Value::Get<int>() const {
	try {
		return std::get<int>(m_value);
	} catch (const std::bad_variant_access&) {
		throw WrongValueType("Integer");
	}
}

template<> const int64_t& Value::Get<int64_t>() const {
	try {
		return std::get<int64_t>(m_value);
	} catch (const std::bad_variant_access&) {
		throw WrongValueType("Integer64");
	}
}

template<> const double& Value::Get<double>() const {
	try {
		return expected_value_cref(std::get<double>(m_value));
	} catch (const std::bad_variant_access&) {
		throw WrongValueType("Double");
	}
}

template<> const bool& Value::Get<bool>() const {
	try {
		return std::get<bool>(m_value);
	} catch (const std::bad_variant_access&) {
		throw WrongValueType("Bool");
	}
}

template<> const std::string& Value::Get<std::string>() const {
	try {
		return std::get<std::string>(m_value);
	} catch (const std::bad_variant_access&) {
		throw WrongValueType("String");
	}
}
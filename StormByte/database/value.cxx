#include <StormByte/database/exception.hxx>
#include <StormByte/database/value.hxx>

using namespace StormByte::Database;


template<> expected_type_cref(int, WrongValueType) Value::Get<int>() const {
	try {
		return expected_value_cref(std::get<int>(m_value));
	} catch (const std::bad_variant_access&) {
		return unexpected_value(WrongValueType("Integer"));
	}
}

template<> expected_type_cref(int64_t, WrongValueType) Value::Get<int64_t>() const {
	try {
		return expected_value_cref(std::get<int64_t>(m_value));
	} catch (const std::bad_variant_access&) {
		return unexpected_value(WrongValueType("Integer64"));
	}
}

template<> expected_type_cref(double, WrongValueType) Value::Get<double>() const {
	try {
		return expected_value_cref(std::get<double>(m_value));
	} catch (const std::bad_variant_access&) {
		return unexpected_value(WrongValueType("Integer64"));
	}
}

template<> expected_type_cref(bool, WrongValueType)	Value::Get<bool>() const {
	try {
		return expected_value_cref(std::get<bool>(m_value));
	} catch (const std::bad_variant_access&) {
		return unexpected_value(WrongValueType("Integer64"));
	}
}

template<> expected_type_cref(std::string, WrongValueType) Value::Get<std::string>() const {
	try {
		return expected_value_cref(std::get<std::string>(m_value));
	} catch (const std::bad_variant_access&) {
		return unexpected_value(WrongValueType("Integer64"));
	}
}
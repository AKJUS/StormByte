#include <StormByte/config/item/value/integer.hxx>
#include <StormByte/config/exception.hxx>

using namespace StormByte::Config;

Integer::Integer(const std::string& name):
Value(Type::Integer, name), m_value(0) {}

Integer::Integer(std::string&& name):
Value(Type::Integer, std::move(name)), m_value(0) {}

const int& Integer::AsInteger() const {
	return m_value;
}

const double& Integer::AsDouble() const {
	throw WrongValueTypeConversion(*this, "AsDouble");
}

const std::string& Integer::AsString() const {
	throw WrongValueTypeConversion(*this, "AsString");
}

void Integer::SetInteger(const int& val) {
	m_value = val;
}

void Integer::SetDouble(const double&) {
	throw ValueFailure(*this, Type::Double);
}

void Integer::SetString(const std::string&) {
	throw ValueFailure(*this, Type::String);
}

void Integer::SetString(std::string&&) {
	throw ValueFailure(*this, Type::String);
}

std::string Integer::Serialize(const int& indent_level) const noexcept {
	return Indent(indent_level) + m_name + " = " + std::to_string(m_value) + ";";
}

std::shared_ptr<Item> Integer::Clone() {
	return std::make_shared<Integer>(*this);
}
#include <StormByte/config/item/value/string.hxx>
#include <StormByte/config/exception.hxx>

using namespace StormByte::Config;

String::String(const std::string& name):
Value(Type::String, name) {}

String::String(std::string&& name):
Value(Type::String, std::move(name)) {}

const int& String::AsInteger() const {
	throw WrongValueTypeConversion(*this, "AsInteger");
}

const double& String::AsDouble() const {
	throw WrongValueTypeConversion(*this, "AsDouble");
}

const std::string& String::AsString() const {
	return m_value;
}

void String::SetInteger(const int&) {
	throw ValueFailure(*this, Type::Integer);
}

void String::SetDouble(const double&) {
	throw ValueFailure(*this, Type::Double);
}

void String::SetString(const std::string& val) {
	m_value = val;
}

void String::SetString(std::string&& val) {
	m_value = std::move(val);
}

std::string String::Serialize(const int& indent_level) const noexcept {
	return Indent(indent_level) + m_name + " = \"" + m_value + "\";";
}

std::shared_ptr<Item> String::Clone() {
	return std::make_shared<String>(*this);
}
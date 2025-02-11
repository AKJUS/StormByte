#include <StormByte/config/item/value/double.hxx>
#include <StormByte/config/exception.hxx>

using namespace StormByte::Config;

Double::Double(const std::string& name):
Value(Type::Double, name), m_value(0) {}

Double::Double(std::string&& name):
Value(Type::Double, std::move(name)), m_value(0) {}

const int& Double::AsInteger() const {
	throw WrongValueTypeConversion(*this, "AsInteger");
}

const double& Double::AsDouble() const {
	return m_value;
}

const std::string& Double::AsString() const {
	throw WrongValueTypeConversion(*this, "AsString");
}

void Double::SetInteger(const int&) {
	throw ValueFailure(*this, Type::Integer);
}

void Double::SetDouble(const double& val) {
	m_value = val;
}

void Double::SetString(const std::string&) {
	throw ValueFailure(*this, Type::String);
}

void Double::SetString(std::string&&) {
	throw ValueFailure(*this, Type::String);
}

std::string Double::Serialize(const int& indent_level) const noexcept {
	return Indent(indent_level) + m_name + " = " + std::to_string(m_value) + ";";
}

std::shared_ptr<Item> Double::Clone() {
	return std::make_shared<Double>(*this);
}
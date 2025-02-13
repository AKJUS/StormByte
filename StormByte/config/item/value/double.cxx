#include <StormByte/config/item/value/double.hxx>
#include <StormByte/config/exception.hxx>

using namespace StormByte::Config;

Double::Double(const std::string& name):
Value(Type::Double, name), m_value(0) {}

Double::Double(std::string&& name):
Value(Type::Double, std::move(name)), m_value(0) {}

std::string Double::Serialize(const int& indent_level) const noexcept {
	return Indent(indent_level) + m_name + " = " + std::to_string(m_value) + ";";
}

std::shared_ptr<Item> Double::Clone() {
	return std::make_shared<Double>(*this);
}
#include <StormByte/config/item/value/bool.hxx>
#include <StormByte/config/exception.hxx>

using namespace StormByte::Config;

Bool::Bool(const std::string& name):
Value(Type::Bool, name), m_value(0) {}

Bool::Bool(std::string&& name):
Value(Type::Bool, std::move(name)), m_value(0) {}

std::string Bool::Serialize(const int& indent_level) const noexcept {
	std::string serialization = Indent(indent_level) + m_name + " = ";
	if (m_value)
		serialization += "true";
	else
		serialization += "false";
	serialization += ";";
	return serialization;
}

std::shared_ptr<Item> Bool::Clone() {
	return std::make_shared<Bool>(*this);
}
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

void Integer::SetInteger(const int& val) {
	m_value = val;
}

std::string Integer::Serialize(const int& indent_level) const noexcept {
	return Indent(indent_level) + m_name + " = " + std::to_string(m_value) + ";";
}

std::shared_ptr<Item> Integer::Clone() {
	return std::make_shared<Integer>(*this);
}
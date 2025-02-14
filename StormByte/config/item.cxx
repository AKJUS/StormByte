#include <StormByte/config/item.hxx>
#include <StormByte/config/group.hxx>
#include <StormByte/config/exception.hxx>

using namespace StormByte::Config;

Item::Item(const std::string& name, const Group& value):m_name(name), m_type(Type::Group), m_value(value) {}

Item::Item(const std::string& name, Group&& value):m_name(name), m_type(Type::Group), m_value(std::move(value)) {}

Item::Item(const std::string& name, const std::string& value):m_name(name), m_type(Type::String), m_value(value) {}

Item::Item(const char* name, const char* value):m_name(name), m_type(Type::String), m_value(value) {}

Item::Item(const std::string& name, const int& value):m_name(name), m_type(Type::Integer), m_value(value) {}

Item::Item(const std::string& name, const double& value):m_name(name), m_type(Type::Double), m_value(value) {}

Item::Item(const std::string& value):m_name(""), m_type(Type::Comment), m_value(value) {}

Item::Item(const std::string& name, bool value):m_name(name), m_type(Type::Bool), m_value(value) {}

template<> Group& Item::Value<Group>() {
	if (m_type != Type::Group)
		throw WrongValueTypeConversion(*this, "Group");
	return std::get<Group>(m_value);
}

template<> int& Item::Value<int>() {
	if (m_type != Type::Integer)
		throw WrongValueTypeConversion(*this, "Integer");
	else
		return std::get<int>(m_value);
}

template<> double& Item::Value<double>() {
	if (m_type != Type::Double)
		throw WrongValueTypeConversion(*this, "Double");
	else
		return std::get<double>(m_value);
}

template<> std::string& Item::Value<std::string>() {
	if (m_type != Type::String && m_type != Type::Comment)
		throw WrongValueTypeConversion(*this, "String");
	else
		return std::get<std::string>(m_value);
}

template<> bool& Item::Value<bool>() {
	if (m_type != Type::Bool)
		throw WrongValueTypeConversion(*this, "Bool");
	else
		return std::get<bool>(m_value);
}

template<> const Group& Item::Value<Group>() const {
	if (m_type != Type::Group)
		throw WrongValueTypeConversion(*this, "Group");
	return std::get<Group>(m_value);
}

template<> const int& Item::Value<int>() const {
	if (m_type != Type::Integer)
		throw WrongValueTypeConversion(*this, "Integer");
	else
		return std::get<int>(m_value);
}

template<> const double& Item::Value<double>() const {
	if (m_type != Type::Double)
		throw WrongValueTypeConversion(*this, "Double");
	else
		return std::get<double>(m_value);
}

template<> const std::string& Item::Value<std::string>() const {
	if (m_type != Type::String && m_type != Type::Comment)
		throw WrongValueTypeConversion(*this, "String");
	else
		return std::get<std::string>(m_value);
}

template<> const bool& Item::Value<bool>() const {
	if (m_type != Type::Bool)
		throw WrongValueTypeConversion(*this, "Bool");
	else
		return std::get<bool>(m_value);
}

std::string Item::Serialize(const int& indent_level) const noexcept {
	std::string serial = Indent(indent_level) + m_name + " = ";
	switch (m_type) {
		case Type::Integer:
			serial += std::to_string(Value<int>()) + ";\n";
			break;
		case Type::String:
			serial += "\"" + Value<std::string>() + "\";\n";
			break;
		case Type::Double:
			serial += std::to_string(Value<double>()) + ";\n";
			break;
		case Type::Bool:
			serial += std::string(Value<bool>() ? "true" : "false") + ";\n";
			break;
		case Type::Group:
			serial += "{\n";
			serial += Value<Group>().Serialize(indent_level + 1);
			serial += Indent(indent_level) + "};\n";
			break;
		case Type::Comment:
			serial = Indent(indent_level) + "#" + Value<std::string>() + "\n";
			break;
	}
	return serial;
}
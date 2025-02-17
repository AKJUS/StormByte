#include <StormByte/config/item.hxx>
#include <StormByte/config/group.hxx>
#include <StormByte/config/list.hxx>
#include <StormByte/config/exception.hxx>

using namespace StormByte::Config;

Item::Item(const Group& value):m_type(Type::Group), m_value(new ItemStorage(value)) {}

Item::Item(Group&& value):m_type(Type::Group), m_value(new ItemStorage(std::move(value))) {}

Item::Item(const List& value):m_type(Type::List), m_value(new ItemStorage(value)) {}

Item::Item(List&& value):m_type(Type::List), m_value(new ItemStorage(std::move(value))) {}

Item::Item(const std::string& value):m_type(Type::String), m_value(new ItemStorage(value)) {}

Item::Item(const char* value):m_type(Type::String), m_value(new ItemStorage(value)) {}

Item::Item(const int& value):m_type(Type::Integer), m_value(new ItemStorage(value)) {}

Item::Item(const double& value):m_type(Type::Double), m_value(new ItemStorage(value)) {}

Item::Item(bool value):m_type(Type::Bool), m_value(new ItemStorage(value)) {}

Item::Item(const Item& src):m_type(src.m_type), m_value(new ItemStorage(*src.m_value)) {}

Item::Item(Item&& src) noexcept:m_type(src.m_type), m_value(src.m_value) {
	src.m_value = nullptr;
}

Item& Item::operator=(const Item& src) {
	if (this != &src) {
		delete m_value;
		m_type = src.m_type;
		m_value = new ItemStorage(*src.m_value); // Item is assumed to always have content
	}
	return *this;
}

Item& Item::operator=(Item&& src) noexcept {
	if (this != &src) {
		delete m_value;
		m_type = src.m_type;
		m_value = src.m_value;
		src.m_value = nullptr;
	}
	return *this;
}

Item::~Item() noexcept {
	delete m_value;
}

template<> Group& Item::Value<Group>() {
	if (m_type != Type::Group)
		throw WrongValueTypeConversion(*this, "Group");
	return std::get<Group>(*m_value);
}

template<> int& Item::Value<int>() {
	if (m_type != Type::Integer)
		throw WrongValueTypeConversion(*this, "Integer");
	else
		return std::get<int>(*m_value);
}

template<> double& Item::Value<double>() {
	if (m_type != Type::Double)
		throw WrongValueTypeConversion(*this, "Double");
	else
		return std::get<double>(*m_value);
}

template<> std::string& Item::Value<std::string>() {
	if (m_type != Type::String && m_type != Type::Comment)
		throw WrongValueTypeConversion(*this, "String");
	else
		return std::get<std::string>(*m_value);
}

template<> bool& Item::Value<bool>() {
	if (m_type != Type::Bool)
		throw WrongValueTypeConversion(*this, "Bool");
	else
		return std::get<bool>(*m_value);
}

template<> const Group& Item::Value<Group>() const {
	if (m_type != Type::Group)
		throw WrongValueTypeConversion(*this, "Group");
	return std::get<Group>(*m_value);
}

template<> const int& Item::Value<int>() const {
	if (m_type != Type::Integer)
		throw WrongValueTypeConversion(*this, "Integer");
	else
		return std::get<int>(*m_value);
}

template<> const double& Item::Value<double>() const {
	if (m_type != Type::Double)
		throw WrongValueTypeConversion(*this, "Double");
	else
		return std::get<double>(*m_value);
}

template<> const std::string& Item::Value<std::string>() const {
	if (m_type != Type::String && m_type != Type::Comment)
		throw WrongValueTypeConversion(*this, "String");
	else
		return std::get<std::string>(*m_value);
}

template<> const bool& Item::Value<bool>() const {
	if (m_type != Type::Bool)
		throw WrongValueTypeConversion(*this, "Bool");
	else
		return std::get<bool>(*m_value);
}

template<> List& Item::Value<List>() {
	if (m_type != Type::List)
		throw WrongValueTypeConversion(*this, "List");
	return std::get<List>(*m_value);
}

template<> const List& Item::Value<List>() const {
	if (m_type != Type::List)
		throw WrongValueTypeConversion(*this, "List");
	return std::get<List>(*m_value);
}

std::string Item::Serialize(const int& indent_level) const noexcept {
	return Indent(indent_level) + ContentsToString(indent_level);
}

std::string Item::ContentsToString(const int& indent_level) const noexcept {
	std::string serial = "";
	switch (m_type) {
		case Type::Integer:
			serial += std::to_string(Value<int>());
			break;
		case Type::String:
			serial += "\"" + Value<std::string>() + "\"";
			break;
		case Type::Double:
			serial += std::to_string(Value<double>());
			break;
		case Type::Bool:
			serial += std::string(Value<bool>() ? "true" : "false");
			break;
		case Type::Group:
			serial += "{\n" + Value<Group>().Serialize(indent_level) + Item::Indent(indent_level) + "}";
			break;
		case Type::Comment:
			serial = "#" + Value<std::string>();
			break;
		case Type::List:
			serial += "[\n" + Value<List>().Serialize(indent_level) + Item::Indent(indent_level) + "]";
			break;
	}
	return serial + "\n";
}
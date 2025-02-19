#include <StormByte/config/comment.hxx>
#include <StormByte/config/group.hxx>
#include <StormByte/config/list.hxx>
#include <StormByte/config/exception.hxx>
#include <StormByte/util/string/string.hxx>

#include <regex>

using namespace StormByte::Config;

Item::Item(const std::string& name, const Container& value):m_name(name), m_type(Type::Container), m_value(std::make_unique<ItemStorage>(value.Clone())) {}

Item::Item(const Container& value):m_type(Type::Container), m_value(std::make_unique<ItemStorage>(value.Clone())) {}

Item::Item(std::string&& name, Container&& value):m_name(std::move(name)), m_type(Type::Container), m_value(std::make_unique<ItemStorage>(value.Move())) {}

Item::Item(Container&& value):m_type(Type::Container), m_value(std::make_unique<ItemStorage>(value.Move())) {}

Item::Item(const Comment::Comment& value):m_type(Type::Comment), m_value(std::make_unique<ItemStorage>(value.Clone())) {}

Item::Item(Comment::Comment&& value):m_type(Type::Comment), m_value(std::make_unique<ItemStorage>(value.Move())) {}

Item::Item(const std::string& name, const std::string& value):m_name(name), m_type(Type::String), m_value(std::make_unique<ItemStorage>(value)) {}

Item::Item(const std::string& value):m_type(Type::String), m_value(std::make_unique<ItemStorage>(value)) {}

Item::Item(std::string&& name, std::string&& value):m_name(std::move(name)), m_type(Type::String), m_value(std::make_unique<ItemStorage>(std::move(value))) {}

Item::Item(std::string&& value):m_type(Type::String), m_value(std::make_unique<ItemStorage>(std::move(value))) {}

Item::Item(const char* name, const char* value):m_name(name), m_type(Type::String), m_value(std::make_unique<ItemStorage>(value)) {}

Item::Item(const char* value):m_type(Type::String), m_value(std::make_unique<ItemStorage>(value)) {}

Item::Item(const std::string& name, const int& value):m_name(name), m_type(Type::Integer), m_value(std::make_unique<ItemStorage>(value)) {}

Item::Item(const int& value):m_type(Type::Integer), m_value(std::make_unique<ItemStorage>(value)) {}

Item::Item(const std::string& name, const double& value):m_name(name), m_type(Type::Double), m_value(std::make_unique<ItemStorage>(value)) {}

Item::Item(const double& value):m_type(Type::Double), m_value(std::make_unique<ItemStorage>(value)) {}

Item::Item(const std::string& name, bool value):m_name(name), m_type(Type::Bool), m_value(std::make_unique<ItemStorage>(value)) {}

Item::Item(bool value):m_type(Type::Bool), m_value(std::make_unique<ItemStorage>(value)) {}

Item::Item(const Item& src):m_name(src.m_name), m_type(src.m_type), m_value(std::make_unique<ItemStorage>(*src.m_value)) {}

Item& Item::operator=(const Item& src) {
	if (this != &src) {
		m_name = src.m_name;
		m_type = src.m_type;
		m_value = std::make_unique<ItemStorage>(*src.m_value); // Item is assumed to always have content
	}
	return *this;
}

bool Item::IsNameValid(const std::string& name) noexcept {
	std::regex name_regex(R"(^[A-Za-z][A-Za-z0-9_]*$)");
	return std::regex_match(name, name_regex);
}

bool Item::IsPathValid(const std::string& name) noexcept {
    std::regex name_regex(R"(^[A-Za-z][A-Za-z0-9_]*(/[A-Za-z0-9_]+)*$)");
	return std::regex_match(name, name_regex);
}

bool Item::operator==(const Item& other) const noexcept {
	return m_name == other.m_name && m_type == other.m_type && *m_value == *other.m_value;
}

template<> Serializable& Item::Value<Serializable>() {
	if (m_type != Type::Container && m_type != Type::Comment)
		throw WrongValueTypeConversion(TypeAsString(), "Serializable");
	return *std::get<std::shared_ptr<Serializable>>(*m_value);
}

template<> const Serializable& Item::Value<Serializable>() const {
	if (m_type != Type::Container && m_type != Type::Comment)
		throw WrongValueTypeConversion(TypeAsString(), "Serializable");
	return *std::get<std::shared_ptr<Serializable>>(*m_value);
}

template<> Container& Item::Value<Container>() {
	if (m_type != Type::Container)
		throw WrongValueTypeConversion(TypeAsString(), TypeAsString(Type::Container));
	return *std::static_pointer_cast<Container>(std::get<std::shared_ptr<Serializable>>(*m_value));
}

template<> const Container& Item::Value<Container>() const {
	if (m_type != Type::Container)
		throw WrongValueTypeConversion(TypeAsString(), TypeAsString(Type::Container));
	return *std::static_pointer_cast<Container>(std::get<std::shared_ptr<Serializable>>(*m_value));
}

template<> Group& Item::Value<Group>() {
	if (m_type != Type::Container)
		throw WrongValueTypeConversion(TypeAsString(), Container::TypeAsString(Container::Type::Group));
	Container& container = *std::static_pointer_cast<Container>(std::get<std::shared_ptr<Serializable>>(*m_value));
	if (container.GetType() != Container::Type::Group)
		throw WrongValueTypeConversion(TypeAsString(), Container::TypeAsString(Container::Type::Group));
	return *static_cast<Group*>(&container);
}

template<> const Group& Item::Value<Group>() const {
	if (m_type != Type::Container)
		throw WrongValueTypeConversion(TypeAsString(), Container::TypeAsString(Container::Type::Group));
	const Container& container = *std::static_pointer_cast<Container>(std::get<std::shared_ptr<Serializable>>(*m_value));
	if (container.GetType() != Container::Type::Group)
		throw WrongValueTypeConversion(TypeAsString(), Container::TypeAsString(Container::Type::Group));
	return *static_cast<const Group*>(&container);
}

template<> List& Item::Value<List>() {
	if (m_type != Type::Container)
		throw WrongValueTypeConversion(TypeAsString(), Container::TypeAsString(Container::Type::List));
	Container& container = *std::static_pointer_cast<Container>(std::get<std::shared_ptr<Serializable>>(*m_value));
	if (container.GetType() != Container::Type::List)
		throw WrongValueTypeConversion(TypeAsString(), Container::TypeAsString(Container::Type::List));
	return *static_cast<List*>(&container);
}

template<> const List& Item::Value<List>() const {
	if (m_type != Type::Container)
		throw WrongValueTypeConversion(TypeAsString(), Container::TypeAsString(Container::Type::List));
	const Container& container = *std::static_pointer_cast<Container>(std::get<std::shared_ptr<Serializable>>(*m_value));
	if (container.GetType() != Container::Type::List)
		throw WrongValueTypeConversion(TypeAsString(), Container::TypeAsString(Container::Type::List));
	return *static_cast<const List*>(&container);
}

template<> Comment::Comment& Item::Value<Comment::Comment>() {
	if (m_type != Type::Comment)
		throw WrongValueTypeConversion(TypeAsString(), TypeAsString(Type::Comment));
	return *std::static_pointer_cast<Comment::Comment>(std::get<std::shared_ptr<Serializable>>(*m_value));
}

template<> const Comment::Comment& Item::Value<Comment::Comment>() const {
	if (m_type != Type::Comment)
		throw WrongValueTypeConversion(TypeAsString(), TypeAsString(Type::Comment));
	return *std::static_pointer_cast<Comment::Comment>(std::get<std::shared_ptr<Serializable>>(*m_value));
}

template<> int& Item::Value<int>() {
	if (m_type != Type::Integer)
		throw WrongValueTypeConversion(TypeAsString(), TypeAsString(Type::Integer));
	else
		return std::get<int>(*m_value);
}

template<> const int& Item::Value<int>() const {
	if (m_type != Type::Integer)
		throw WrongValueTypeConversion(TypeAsString(), TypeAsString(Type::Integer));
	else
		return std::get<int>(*m_value);
}

template<> double& Item::Value<double>() {
	if (m_type != Type::Double)
		throw WrongValueTypeConversion(TypeAsString(), TypeAsString(Type::Double));
	else
		return std::get<double>(*m_value);
}

template<> const double& Item::Value<double>() const {
	if (m_type != Type::Double)
		throw WrongValueTypeConversion(TypeAsString(), TypeAsString(Type::Double));
	else
		return std::get<double>(*m_value);
}

template<> std::string& Item::Value<std::string>() {
	if (m_type != Type::String)
		throw WrongValueTypeConversion(TypeAsString(), TypeAsString(Type::String));
	else
		return std::get<std::string>(*m_value);
}

template<> const std::string& Item::Value<std::string>() const {
	if (m_type != Type::String)
		throw WrongValueTypeConversion(TypeAsString(), TypeAsString(Type::String));
	else
		return std::get<std::string>(*m_value);
}

template<> bool& Item::Value<bool>() {
	if (m_type != Type::Bool)
		throw WrongValueTypeConversion(TypeAsString(), TypeAsString(Type::Bool));
	else
		return std::get<bool>(*m_value);
}

template<> const bool& Item::Value<bool>() const {
	if (m_type != Type::Bool)
		throw WrongValueTypeConversion(TypeAsString(), TypeAsString(Type::Bool));
	else
		return std::get<bool>(*m_value);
}

std::string Item::ContentsToString(const int& indent_level) const noexcept {
	std::string serial = Util::String::Indent(indent_level);
	switch (m_type) {
		case Type::Integer:
			serial += NameEqualSignString() + std::to_string(Value<int>());
			break;
		case Type::String:
			serial += NameEqualSignString() + "\"" + Value<std::string>() + "\"";
			break;
		case Type::Double:
			serial += NameEqualSignString() + std::to_string(Value<double>());
			break;
		case Type::Bool:
			serial += NameEqualSignString() + std::string(Value<bool>() ? "true" : "false");
			break;
		case Type::Comment:
			serial += Value<Comment::Comment>().Serialize(indent_level);
			break;
		case Type::Container:
			serial += NameEqualSignString() + Value<Container>().Serialize(indent_level);
			break;
	}
	return serial + "\n";
}

std::shared_ptr<Serializable> Item::Clone() const {
	return std::make_shared<Item>(*this);
}

std::shared_ptr<Serializable> Item::Move() {
	return std::make_shared<Item>(std::move(*this));
}
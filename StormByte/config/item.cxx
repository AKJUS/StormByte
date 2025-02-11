#include <StormByte/config/item.hxx>
#include <StormByte/config/item/group.hxx>
#include <StormByte/config/exception.hxx>

using namespace StormByte::Config;

Item::Item(const Type& type, const std::string& name):
m_name(name), m_type(type) {}

Item::Item(const Type& type, std::string&& name):
m_name(std::move(name)), m_type(type) {}

const std::string& Item::GetName() const noexcept { return m_name; }

const Item::Type& Item::GetType() const noexcept { return m_type; }

const std::string Item::GetTypeAsString() const noexcept {
	return GetTypeAsString(m_type);
}

const std::string Item::GetTypeAsString(const Type& t) noexcept {
	std::string str;
	switch(t) {
		case Type::Group:
			str = "Group";
			break;

		case Type::String:
			str = "String";
			break;

		case Type::Integer:
			str = "Integer";
			break;

		case Type::Double:
			str = "Double";
			break;

		case Type::Comment:
			str = "Comment";
			break;
	}
	return str;
}

std::string Item::Indent(const int& level) const noexcept {
	return level == 0 ? std::string() : std::string(level, '\t');
}
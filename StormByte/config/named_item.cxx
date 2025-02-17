#include <StormByte/config/named_item.hxx>
#include <StormByte/config/group.hxx>
#include <StormByte/config/list.hxx>
#include <StormByte/config/exception.hxx>

#include <regex>

using namespace StormByte::Config;

NamedItem::NamedItem(const std::string& name, const Group& value):Item(value), m_name(name) {}

NamedItem::NamedItem(const std::string& name, Group&& value):Item(std::move(value)), m_name(name) {}

NamedItem::NamedItem(const std::string& name, const List& value):Item(value), m_name(name) {}

NamedItem::NamedItem(const std::string& name, List&& value):Item(std::move(value)), m_name(name) {}

NamedItem::NamedItem(const std::string& name, const std::string& value):Item(value), m_name(name) {}

NamedItem::NamedItem(const char* name, const char* value):Item(value),m_name(name) {}

NamedItem::NamedItem(const std::string& name, const int& value):Item(value), m_name(name) {}

NamedItem::NamedItem(const std::string& name, const double& value):Item(value), m_name(name) {}

NamedItem::NamedItem(const std::string& name, bool value):Item(value), m_name(name) {}

bool NamedItem::IsNameValid(const std::string& name) noexcept {
	std::regex name_regex(R"(^[A-Za-z][A-Za-z0-9_]*$)");
	return std::regex_match(name, name_regex);
}

bool NamedItem::IsPathValid(const std::string& name) noexcept {
    std::regex name_regex(R"(^[A-Za-z][A-Za-z0-9_]*(/[A-Za-z0-9_]+)*$)");
	return std::regex_match(name, name_regex);
}

std::string NamedItem::Serialize(const int& indent_level) const noexcept {
	std::string serial = "";
	if (m_type == Item::Type::Comment)
		return serial + Item::Serialize(indent_level);
	else
		return serial + Indent(indent_level) + m_name + " = " + ContentsToString(indent_level);
}
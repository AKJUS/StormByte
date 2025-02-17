#include <StormByte/config/config.hxx>
#include <StormByte/config/named_item.hxx>

#include <memory>
#include <sstream>

using namespace StormByte::Config;

Config::Config():m_root(), m_on_name_clash_action(Group::OnNameClashAction::ThrowException) {}

Config::Config(const Config& config):
m_root(config.m_root),m_before_read_hooks(config.m_before_read_hooks),
m_after_read_hooks(config.m_after_read_hooks), m_on_name_clash_action(config.m_on_name_clash_action) {}

Config::Config(Config&& config) noexcept:
m_root(std::move(config.m_root)),m_before_read_hooks(std::move(config.m_before_read_hooks)),
m_after_read_hooks(std::move(config.m_after_read_hooks)), m_on_name_clash_action(config.m_on_name_clash_action) {}

Config& Config::operator=(const Config& config) {
	if (this != &config) {
		m_root = Group(config.m_root);
		m_on_name_clash_action = config.m_on_name_clash_action;
		m_before_read_hooks = config.m_before_read_hooks;
		m_after_read_hooks = config.m_after_read_hooks;
	}
	return *this;
}

Config& Config::operator=(Config&& config) noexcept {
	if (this != &config) {
		m_root = std::move(config.m_root);
		m_on_name_clash_action = config.m_on_name_clash_action;
		m_before_read_hooks = std::move(config.m_before_read_hooks);
		m_after_read_hooks = std::move(config.m_after_read_hooks);
	}
	return *this;
}

void Config::Clear() noexcept {
	m_root = Group();
}

NamedItem& Config::operator[](const std::string& path) {
	return m_root[path];
}

const NamedItem& Config::operator[](const std::string& path) const {
	return m_root[path];
}

Config& Config::operator<<(const Config& source) {
	// We will not use serialize for performance reasons
	for (auto it = source.Begin(); it != source.End(); it++)
		m_root.Add(*it, m_on_name_clash_action);
	return *this;
}

void Config::operator<<(std::istream& istream) { // 1
	Parse(istream, m_root);
}

void Config::operator<<(const std::string& str) { // 2
	std::istringstream istream(str);
	*this << istream;
}

Config& StormByte::Config::operator>>(std::istream& istream, Config& config) { // 3
	config << istream;
	return config;
}

Config& StormByte::Config::operator>>(const std::string& str, Config& config) { // 4
	config << str;
	return config;
}

Config& Config::operator>>(Config& dest) const {
	dest << *this;
	return dest;
}

std::ostream& Config::operator>>(std::ostream& ostream) const { // 5
	ostream << (std::string)*this;
	return ostream;
}

std::string& Config::operator>>(std::string& str) const { // 6
	str += *this; // Conversion should be done automatically by operator std::string()
	return str;
}

std::ostream& StormByte::Config::operator<<(std::ostream& ostream, const Config& config) { // 7
	ostream << (std::string)config;
	return ostream;
}

std::string& operator<<(std::string& str, const Config& config) { // 8
	str += config;
	return str;
}

Config::operator std::string() const {
	std::string serialized = "";
	for (auto it = Begin(); it != End(); it++) {
		serialized += it->Serialize(0);
		if (it->GetType() == Item::Type::Comment)
			serialized += "\n";
		else if (it->GetType() != Item::Type::Group && it->GetType() != Item::Type::List)
			serialized += ";\n";
	}
	return serialized;
}

bool Config::Exists(const std::string& path) const noexcept {
	return m_root.Exists(path);
}

Group::Iterator Config::Begin() noexcept {
	return m_root.Begin();
}

Group::Const_Iterator Config::Begin() const noexcept {
	return m_root.Begin();
}

Group::Iterator Config::End() noexcept {
	return m_root.End();
}

Group::Const_Iterator Config::End() const noexcept {
	return m_root.End();
}


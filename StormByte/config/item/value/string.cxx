#include <StormByte/config/item/value/string.hxx>
#include <StormByte/config/exception.hxx>

using namespace StormByte::Config;

String::String(const std::string& name):
Value(Type::String, name) {}

String::String(std::string&& name):
Value(Type::String, std::move(name)) {}

const std::string& String::AsString() const {
	return m_value;
}

void String::SetString(const std::string& val) {
	m_value = val;
}

void String::SetString(std::string&& val) {
	m_value = std::move(val);
}

std::string String::Serialize(const int& indent_level) const noexcept {
    std::string escaped_value = m_value;
    // Escape the " characters in the string value
    size_t pos = 0;
    while ((pos = escaped_value.find('"', pos)) != std::string::npos) {
        escaped_value.insert(pos, "\\");
        pos += 2; // Skip the escaped quote
    }
    return Indent(indent_level) + m_name + " = \"" + escaped_value + "\";";
}

std::shared_ptr<Item> String::Clone() {
	return std::make_shared<String>(*this);
}
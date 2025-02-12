#include <StormByte/config/item/comment.hxx>
#include <StormByte/config/exception.hxx>

using namespace StormByte::Config;

Comment::Comment():
Item(Type::Comment, std::string()) {}

std::string Comment::Serialize(const int& indent_level) const noexcept {
	return Indent(indent_level) + "#" + m_value;
}

std::shared_ptr<Item> Comment::Clone() {
	return std::make_shared<Comment>(*this);
}

const std::string& Comment::AsString() const {
	return m_value;
}

void Comment::SetString(const std::string& val) {
	m_value = val;
}

void Comment::SetString(std::string&& val) {
	m_value = std::move(val);
}
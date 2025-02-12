#include <StormByte/config/item/comment.hxx>
#include <StormByte/config/exception.hxx>

using namespace StormByte::Config;

Comment::Comment():
Item(Type::Comment, std::string()) {}

Group& Comment::AsGroup() {
	throw WrongValueTypeConversion(*this, "AsGroup");
}

const int& Comment::AsInteger() const {
	throw WrongValueTypeConversion(*this, "AsInteger");
}

const double& Comment::AsDouble() const {
	throw WrongValueTypeConversion(*this, "AsDouble");
}

const std::string& Comment::AsString() const {
	return m_value;
}

bool Comment::AsBool() const {
	throw WrongValueTypeConversion(*this, "AsBool");
}

void Comment::SetInteger(const int&) {
	throw ValueFailure(*this, Type::Integer);
}

void Comment::SetDouble(const double&) {
	throw ValueFailure(*this, Type::Double);
}

void Comment::SetString(const std::string& val) {
	m_value = val;
}

void Comment::SetString(std::string&& val) {
	m_value = std::move(val);
}

void Comment::SetBool(bool) {
	throw ValueFailure(*this, Type::Bool);
}

std::string Comment::Serialize(const int& indent_level) const noexcept {
	return Indent(indent_level) + "#" + m_value;
}

std::shared_ptr<Item> Comment::Clone() {
	return std::make_shared<Comment>(*this);
}
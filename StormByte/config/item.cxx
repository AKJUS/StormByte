#include <StormByte/config/item.hxx>
#include <StormByte/config/item/group.hxx>
#include <StormByte/config/exception.hxx>

using namespace StormByte::Config;

Item::Item(const Type& type, const std::string& name):
m_name(name), m_type(type) {}

Item::Item(const Type& type, std::string&& name):
m_name(std::move(name)), m_type(type) {}

Group& Item::AsGroup() {
	throw WrongValueTypeConversion(*this, "AsGroup");
}

const int& Item::AsInteger() const {
	throw WrongValueTypeConversion(*this, "AsInteger");
}

const double& Item::AsDouble() const {
	throw WrongValueTypeConversion(*this, "AsDouble");
}

const std::string& Item::AsString() const {
	throw WrongValueTypeConversion(*this, "AsString");
}

bool Item::AsBool() const {
	throw WrongValueTypeConversion(*this, "AsBool");
}

void Item::SetInteger(const int&) {
	throw ValueFailure(*this, Type::Integer);
}

void Item::SetDouble(const double&) {
	throw ValueFailure(*this, Type::Double);
}

void Item::SetString(const std::string&) {
	throw ValueFailure(*this, Type::String);
}

void Item::SetString(std::string&&) {
	throw ValueFailure(*this, Type::String);
}

void Item::SetBool(bool) {
	throw ValueFailure(*this, Type::Bool);
}

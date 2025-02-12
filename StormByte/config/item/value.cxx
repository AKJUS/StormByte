#include <StormByte/config/item/value.hxx>
#include <StormByte/config/exception.hxx>

using namespace StormByte::Config;

Value::Value(const Type& type, const std::string& name):
Item(type, name) {}

Value::Value(const Type& type, std::string&& name):
Item(type, name) {}

Group& Value::AsGroup() {
	throw WrongValueTypeConversion(*this, "AsGroup");
}

const int& Value::AsInteger() const {
	throw WrongValueTypeConversion(*this, "AsInteger");
}

const double& Value::AsDouble() const {
	throw WrongValueTypeConversion(*this, "AsDouble");
}

const std::string& Value::AsString() const {
	throw WrongValueTypeConversion(*this, "AsString");
}

bool Value::AsBool() const {
	throw WrongValueTypeConversion(*this, "AsBool");
}

void Value::SetInteger(const int&) {
	throw ValueFailure(*this, Type::Integer);
}

void Value::SetDouble(const double&) {
	throw ValueFailure(*this, Type::Double);
}

void Value::SetString(const std::string&) {
	throw ValueFailure(*this, Type::String);
}

void Value::SetString(std::string&&) {
	throw ValueFailure(*this, Type::String);
}

void Value::SetBool(bool) {
	throw ValueFailure(*this, Type::Bool);
}
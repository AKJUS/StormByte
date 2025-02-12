#include <StormByte/config/item/value.hxx>
#include <StormByte/config/exception.hxx>

using namespace StormByte::Config;

Value::Value(const Type& type, const std::string& name):
Item(type, name) {}

Value::Value(const Type& type, std::string&& name):
Item(type, name) {}

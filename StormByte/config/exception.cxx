#include <StormByte/config/exception.hxx>

using namespace StormByte::Config;

Exception::Exception(const std::string& msg):System::Exception(msg) {}

WrongValueTypeConversion::WrongValueTypeConversion(const Item& item, const std::string& method):
Exception(method + " conversion failed for " + item.GetName() + "(" + item.GetTypeAsString() + ")") {}

ValueFailure::ValueFailure(const Item& item, const Item::Type& type):
Exception("Try to add/set " + std::string(Item::GetTypeAsString(type)) + " value to " + item.GetName() + " which is of type " + item.GetTypeAsString()) {}

InvalidName::InvalidName(const std::string& name):
Exception("Invalid name " + name + " given as identifier, only alfanumeric is allowed") {}

ParseError::ParseError(const std::string& reason):
Exception("Parse error: " + reason) {}

ItemNotFound::ItemNotFound(const std::string& name):
Exception("Item " + name + " do not exist") {}

ItemNameAlreadyExists::ItemNameAlreadyExists(const std::string& name):
Exception("Can not add item: Another item with name " + name + " already exists") {}
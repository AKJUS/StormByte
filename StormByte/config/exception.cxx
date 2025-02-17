#include <StormByte/config/exception.hxx>

using namespace StormByte::Config;

Exception::Exception(const std::string& msg):StormByte::Exception(msg) {}

WrongValueTypeConversion::WrongValueTypeConversion(const Item& item, const std::string& method):
Exception(method + " conversion failed for " + item.GetTypeAsString()) {}

ValueFailure::ValueFailure(const Item& item, const Item::Type& type):
Exception("Try to add/set " + std::string(Item::GetTypeAsString(type)) + " value which is of type " + item.GetTypeAsString()) {}

InvalidName::InvalidName(const std::string& name):
Exception("Invalid name " + name + " given as identifier, only alfanumeric is allowed") {}

ParseError::ParseError(const std::string& reason):
Exception("Parse error: " + reason) {}

NamedItemNotFound::NamedItemNotFound(const std::string& name):
Exception("NamedItem " + name + " do not exist") {}

NamedItemNameAlreadyExists::NamedItemNameAlreadyExists(const std::string& name):
Exception("Can not add item: Another item with name " + name + " already exists") {}

OutOfBounds::OutOfBounds(const size_t& index, const size_t& size):
Exception("Index " + std::to_string(index) + " is out of bounds when size is " + std::to_string(size)) {}
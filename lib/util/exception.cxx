#include <util/exception.hxx>

using namespace StormByte::Util;

Exception::Exception(const std::string& reason):
StormByte::Exception(reason) {}

Exception::Exception(std::string&& reason):
StormByte::Exception(std::move(reason)) {}

ValueTypeMissmatch::ValueTypeMissmatch(const std::string& targettype):
Exception("Value requested is not a " + targettype) {}
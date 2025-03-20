#include <StormByte/util/exception.hxx>

#include <format>

using namespace StormByte::Util;

Exception::Exception(const std::string& reason):
StormByte::Exception(reason) {}

Exception::Exception(std::string&& reason):
StormByte::Exception(std::move(reason)) {}

DeserializeError::DeserializeError(const std::string& reason):
Exception(std::format("Deserialize error: {}", reason)) {}

DeserializeError::DeserializeError(std::string&& reason):
Exception(std::format("Deserialize error: {}", std::move(reason))) {}
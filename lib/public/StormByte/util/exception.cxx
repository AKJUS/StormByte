#include <StormByte/util/exception.hxx>

#include <format>

using namespace StormByte::Util;

Exception::Exception(const std::string& reason):
StormByte::Exception(reason) {}

Exception::Exception(std::string&& reason):
StormByte::Exception(std::move(reason)) {}

BufferException::BufferException(const std::string& reason):
Exception(std::format("Buffer error {}", reason)) {}

BufferException::BufferException(std::string&& reason):
Exception(std::format("Buffer error {}", std::move(reason))) {}

BufferOverflow::BufferOverflow(const std::string& reason):
BufferException(std::format("Overflow: {}", reason)) {}

BufferOverflow::BufferOverflow(std::string&& reason):
BufferException(std::format("Overflow: {}", std::move(reason))) {}
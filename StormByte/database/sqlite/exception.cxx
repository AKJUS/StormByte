#include <StormByte/database/sqlite/exception.hxx>

#include <limits>

using namespace StormByte::Database::SQLite;

Exception::Exception(const std::string& msg):System::Exception(msg) {}

Exception::Exception(std::string&& msg) noexcept:System::Exception(std::move(msg)) {}

WrongResultType::WrongResultType(const Type& source_type, const Type& asked_type):
Exception("Wrong value type: Requested " + std::string(GetTypeAsString(asked_type))
	+ " whereas contained type is " + GetTypeAsString(source_type)) {}

Overflow::Overflow(const int64_t& source):
Exception("Value " + std::to_string(source) + " exceeds int max (" +
std::to_string(std::numeric_limits<int>::max()) + ")") {}

ColumNotFound::ColumNotFound(const size_t& max, const size_t& asked):
Exception("Out of bounds: Asked for column number " +
std::to_string(asked) + " when there is only " + std::to_string(max)) {}

ColumNotFound::ColumNotFound(const std::string& name):
Exception("Out of bounds: Column name " + name + " do not exist") {}

ConnectionError::ConnectionError(const std::string& reason):
Exception(reason) {}

ConnectionError::ConnectionError(std::string&& reason):
Exception(std::move(reason)) {}

QueryError::QueryError(const std::string& reason):
Exception(reason) {}

QueryError::QueryError(std::string&& reason):
Exception(std::move(reason)) {}

#pragma once

#include <StormByte/visibility.h>

namespace StormByte::Database::SQLite {
	enum class STORMBYTE_PUBLIC Type: unsigned short {
		Integer = 0,
		Double,
		Bool,
		String,
		Null
	};
	constexpr STORMBYTE_PUBLIC const char* GetTypeAsString(const Type& t) noexcept {
		switch(t) {
			case Type::Integer:	return "Integer";
			case Type::Double:	return "Double";
			case Type::Bool:	return "Bool";
			case Type::String:	return "String";
			case Type::Null:	return "Null";
			default:			return "Unknown";
		}
	}
}

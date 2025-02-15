#pragma once

#include <StormByte/visibility.h>

#ifdef STORMBYTE_ENABLE_SQLITE
	namespace StormByte::Database::SQLite {
		enum class STORMBYTE_PUBLIC Type: unsigned short {
			Integer = 0,
			Double,
			Bool,
			String,
			Null
		};
		constexpr const char* STORMBYTE_PUBLIC GetTypeAsString(const Type& t) noexcept {
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
#endif
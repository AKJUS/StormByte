#pragma once

#include <StormByte/visibility.h>

/**
 * @namespace StormByte::Database::SQLite
 * @brief All the classes for handling SQLite databases
 */
namespace StormByte::Database::SQLite {
	/**
	 * @enum Type
	 * @brief Type of a value
	 */
	enum class STORMBYTE_PUBLIC Type: unsigned short {
		Integer = 0,
		Double,
		Bool,
		String,
		Null
	};
	/**
	 * Gets Type string
	 * @return string
	 */
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

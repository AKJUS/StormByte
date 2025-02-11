#pragma once

#ifdef STORMBYTE_ENABLE_SQLITE
	#include <StormByte/visibility.h>

	namespace StormByte::Database::SQLite {
		enum class STORMBYTE_PUBLIC Type: unsigned short {
			Integer = 0,
			Double,
			Bool,
			String,
			Null
		};
	}
#endif
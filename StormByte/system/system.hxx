#pragma once

#include <StormByte/visibility.h>

#include <filesystem>
#include <string>

namespace StormByte::System {
	#ifdef WINDOWS
	STORMBYTE_PUBLIC std::string			UTF8Encode(const std::wstring&);
	STORMBYTE_PUBLIC std::wstring			UTF8Decode(const std::string&);
	#endif
	STORMBYTE_PUBLIC std::filesystem::path	TempFileName();
}
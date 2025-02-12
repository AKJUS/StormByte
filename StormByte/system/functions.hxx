#pragma once

#include <StormByte/visibility.h>

#include <filesystem>
#include <string>

namespace StormByte::System {
	class STORMBYTE_PUBLIC Functions {
	public:
		#ifdef WINDOWS
		static std::string				UTF8Encode(const std::wstring&);
		static std::wstring				UTF8Decode(const std::string&);
		#endif
		static std::filesystem::path	TempFileName();
	};
	#define CurrentFileDirectory std::filesystem::path(__FILE__).parent_path()
}
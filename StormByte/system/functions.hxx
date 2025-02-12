#include <StormByte/visibility.h>

#include <string>

namespace StormByte::System {
	class STORMBYTE_PUBLIC Functions {
	public:
		#ifdef WINDOWS
		static std::string		UTF8Encode(const std::wstring&);
		static std::wstring		UTF8Decode(const std::string&);
		#endif
	};
}
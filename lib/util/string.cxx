#include <util/string.hxx>

#include <iomanip> // For std::setprecision
#include <sstream> // For std::ostringstream

#ifdef WINDOWS
#include <windows.h> // For MAX_PATH
#include <tchar.h>
#include <direct.h> // For _getcwd
#endif

using namespace StormByte::Util;

std::queue<std::string> String::Explode(const std::string& str, const char& delimiter) {
	std::queue<std::string> result;
	std::stringstream ss(str);
	std::string item;

	while (std::getline(ss, item, delimiter)) {
		result.push(item);
	}

	return result;
}

std::string String::HumanReadableByteSize(const uint64_t& bytes) noexcept {
	constexpr uint64_t KB = 1024;
	constexpr uint64_t MB = KB * 1024;
	constexpr uint64_t GB = MB * 1024;

	std::ostringstream result;
    result << std::fixed << std::setprecision(2); // Set precision to 2 decimals

    if (bytes >= GB) {
        result << (static_cast<double>(bytes) / GB) << " GiB";
    } else if (bytes >= MB) {
        result << (static_cast<double>(bytes) / MB) << " MiB";
    } else if (bytes >= KB) {
        result << (static_cast<double>(bytes) / KB) << " KiB";
    } else {
        result << bytes << " Bytes";
    }

    return result.str();
}

#ifdef WINDOWS
std::string String::UTF8Encode(const std::wstring& wstr) {
	if (wstr.empty()) return std::string();
	int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
	std::string strTo(size_needed, 0);
	WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
	return strTo;
}

std::wstring String::UTF8Decode(const std::string& str) {
	if (str.empty()) return std::wstring();
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
	std::wstring wstrTo(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
	return wstrTo;
}
#endif
#include <StormByte/util/string.hxx>

#include <format> // For std::format
#include <ranges>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
#include <regex>

#ifdef WINDOWS
#include <windows.h> // For MAX_PATH
#include <tchar.h>
#include <direct.h> // For _getcwd
#endif

using namespace StormByte::Util;

std::queue<std::string> String::Explode(const std::string& str, const char delimiter) {
    std::queue<std::string> result;

    // Use ranges to split the string by the delimiter and iterate over parts
    for (auto part : std::string_view(str) | std::views::split(delimiter)) {
        // Convert each part into a std::string and push it to the queue
        result.emplace(part.begin(), part.end());
    }

    return result;
}

std::vector<std::string> String::Split(const std::string& str) {
    std::istringstream iss(str);
    std::vector<std::string> result;
    std::string word;
    while (iss >> word) {
        result.push_back(word); // Insert each word into the vector
    }
    return result;
}

std::string String::HumanReadableByteSize(const uint64_t& bytes) noexcept {
    constexpr uint64_t KB = 1024;
    constexpr uint64_t MB = KB * 1024;
    constexpr uint64_t GB = MB * 1024;

    if (bytes >= GB) return std::format("{:.2f} GiB", static_cast<double>(bytes) / GB);
    if (bytes >= MB) return std::format("{:.2f} MiB", static_cast<double>(bytes) / MB);
    if (bytes >= KB) return std::format("{:.2f} KiB", static_cast<double>(bytes) / KB);
    return std::format("{} Bytes", bytes);
}


std::string String::ToLower(const std::string& str) noexcept {
	std::string result = str;
	std::transform(result.begin(), result.end(), result.begin(), ::tolower);
	return result;
}

std::string String::ToUpper(const std::string& str) noexcept {
	std::string result = str;
	std::transform(result.begin(), result.end(), result.begin(), ::toupper);
	return result;
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

std::string String::SanitizeNewlines(const std::string& str) noexcept {
	std::string result = str;
	return std::regex_replace(str, std::regex("\r\n"), "\n");
}
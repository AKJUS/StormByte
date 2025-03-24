#include <StormByte/util/string.hxx>

#include <cwchar>
#include <cstdlib>
#include <format> // For std::format
#include <ranges>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
#include <regex>

#ifdef WINDOWS
#include <windows.h> // For MAX_PATH
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

StormByte::Expected<std::pair<int, int>, StormByte::Exception> String::SplitFraction(const std::string& fraction) {
	std::size_t slashPos = fraction.find('/');
	if (slashPos == std::string::npos)
		return StormByte::Unexpected<Exception>("Invalid fraction format: '/' not found.");
	
	const std::string numeratorStr = fraction.substr(0, slashPos);
	const std::string denominatorStr = fraction.substr(slashPos + 1);

	if (!IsNumeric(numeratorStr) || !IsNumeric(denominatorStr))
		return StormByte::Unexpected<Exception>("Invalid fraction format: numerator and denominator must be numeric.");

	if (denominatorStr == "0")
		return StormByte::Unexpected<Exception>("Invalid fraction format: denominator cannot be zero.");

	return std::make_pair<int, int>(std::stoi(numeratorStr), std::stoi(denominatorStr));
}

StormByte::Expected<std::pair<int, int>, StormByte::Exception> String::SplitFraction(const std::string& fraction, const int& desired_denominator) {
	auto expected_fraction = SplitFraction(fraction);
	if (!expected_fraction)
		return StormByte::Unexpected(expected_fraction.error());

	auto [numerator, denominator] = expected_fraction.value();
	if (denominator == desired_denominator)
		return std::make_pair(numerator, denominator);
	else if (desired_denominator == 0)
		return StormByte::Unexpected<Exception>("Invalid desired denominator: cannot be zero.");
	else {
		const double factor = static_cast<double>(desired_denominator) / static_cast<double>(denominator);
		return std::make_pair(static_cast<int>(numerator * factor), desired_denominator);
	}
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

std::string String::UTF8Encode(const std::wstring& wstr) {
	if (wstr.empty()) return {};

	// State object to keep track of the conversion process
	std::mbstate_t state = std::mbstate_t();
	const wchar_t* src = wstr.data();
	size_t len = std::wcsrtombs(nullptr, &src, 0, &state); // Get required length

	if (len == static_cast<size_t>(-1)) {
		throw std::runtime_error("Wide to multibyte conversion failed"); // Handle errors
	}

	std::string result(len, '\0'); // Allocate space for the output
	std::wcsrtombs(result.data(), &src, len, &state); // Perform the conversion
	return result;
}

std::wstring String::UTF8Decode(const std::string& str) {
	if (str.empty()) return {};

	// State object to keep track of the conversion process
	std::mbstate_t state = std::mbstate_t();
	const char* src = str.data();
	size_t len = std::mbsrtowcs(nullptr, &src, 0, &state); // Get required length

	if (len == static_cast<size_t>(-1)) {
		throw std::runtime_error("Multibyte to wide conversion failed"); // Handle errors
	}

	std::wstring result(len, L'\0'); // Allocate space for the output
	std::mbsrtowcs(result.data(), &src, len, &state); // Perform the conversion
	return result;
}

std::string String::SanitizeNewlines(const std::string& str) noexcept {
	std::string result = str;
	return std::regex_replace(str, std::regex("\r\n"), "\n");
}

namespace StormByte::Util {
	// Explicit instantiations of HumanReadable for integral types
	template std::string String::HumanReadable<int>(const int&, const Format&, const std::string&) noexcept;
	template std::string String::HumanReadable<unsigned int>(const unsigned int&, const Format&, const std::string&) noexcept;
	template std::string String::HumanReadable<long>(const long&, const Format&, const std::string&) noexcept;
	template std::string String::HumanReadable<unsigned long>(const unsigned long&, const Format&, const std::string&) noexcept;
	template std::string String::HumanReadable<long long>(const long long&, const Format&, const std::string&) noexcept;
	template std::string String::HumanReadable<unsigned long long>(const unsigned long long&, const Format&, const std::string&) noexcept;
	template std::string String::HumanReadable<short>(const short&, const Format&, const std::string&) noexcept;
	template std::string String::HumanReadable<unsigned short>(const unsigned short&, const Format&, const std::string&) noexcept;

	// Explicit instantiations of HumanReadable for floating-point types
	template std::string String::HumanReadable<float>(const float&, const Format&, const std::string&) noexcept;
	template std::string String::HumanReadable<double>(const double&, const Format&, const std::string&) noexcept;
	template std::string String::HumanReadable<long double>(const long double&, const Format&, const std::string&) noexcept;

} // namespace StormByte::Util
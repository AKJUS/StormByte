#include <StormByte/util/string/string.hxx>

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
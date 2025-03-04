#include <exception.hxx>
#include <util/string.hxx>

#include <iostream>

#include "test_handlers.h"

int test_simple_explode() {
	int result = 0;
	try {
		std::string str = "Hello, World!";
		std::queue<std::string> parts = StormByte::Util::String::Explode(str, ',');
		ASSERT_EQUAL("test_simple_explode", 2, parts.size());
		ASSERT_EQUAL("test_simple_explode", "Hello", parts.front());
		parts.pop();
		ASSERT_EQUAL("test_simple_explode", " World!", parts.front());
		parts.pop();
		ASSERT_EQUAL("test_simple_explode", true, parts.empty());
	} catch (const StormByte::Exception& ex) {
		std::cerr << ex.what() << std::endl;
		result++;
	}
	RETURN_TEST("test_simple_explode", result);
}

int main() {
    int result = 0;
    try {
		
    } catch (const StormByte::Exception& ex) {
        std::cerr << ex.what() << std::endl;
        result++;
    }
    if (result == 0) {
        std::cout << "All tests passed!" << std::endl;
    } else {
        std::cout << result << " tests failed." << std::endl;
    }
    return result;
}

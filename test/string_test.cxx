#include <StormByte/exception.hxx>
#include <StormByte/util/string.hxx>

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

int test_path_explode() {
	int result = 0;
	try {
		std::string str = "path/to/items";
		std::queue<std::string> parts = StormByte::Util::String::Explode(str, '/');
		ASSERT_EQUAL("test_path_explode", 3, parts.size());
		ASSERT_EQUAL("test_path_explode", "path", parts.front());
		parts.pop();
		ASSERT_EQUAL("test_path_explode", "to", parts.front());
		parts.pop();
		ASSERT_EQUAL("test_path_explode", "items", parts.front());
		parts.pop();
		ASSERT_EQUAL("test_path_explode", true, parts.empty());
	} catch (const StormByte::Exception& ex) {
		std::cerr << ex.what() << std::endl;
		result++;
	}
	RETURN_TEST("test_path_explode", result);
}

int test_explode_one_item() {
	int result = 0;
	try {
		std::string str = "Hello";
		std::queue<std::string> parts = StormByte::Util::String::Explode(str, '/');
		ASSERT_EQUAL("test_explode_one_item", 1, parts.size());
		ASSERT_EQUAL("test_explode_one_item", "Hello", parts.front());
		parts.pop();
		ASSERT_EQUAL("test_explode_one_item", true, parts.empty());
	} catch (const StormByte::Exception& ex) {
		std::cerr << ex.what() << std::endl;
		result++;
	}
	RETURN_TEST("test_explode_one_item", result);
}

int main() {
    int result = 0;
    try {
		result += test_simple_explode();
		result += test_path_explode();
		result += test_explode_one_item();
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

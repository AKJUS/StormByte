#include <StormByte/util/buffer.hxx>
#include <StormByte/test_handlers.h>
#include <iostream>

using namespace StormByte::Util;

// Test comparisons
int test_simple_buffer() {
    Buffer buffer;
	buffer.Append("Hello", 5);
	std::string original("Hello"), expected(buffer.Data(), buffer.Length());
	ASSERT_EQUAL("test_simple_buffer", original, expected);
    RETURN_TEST("test_simple_buffer", 0);
}

int test_buffer_copy() {
	Buffer buffer;
	buffer.Append("Hello", 5);
	Buffer buffer2(buffer);
	std::string original("Hello"), expected(buffer2.Data(), buffer2.Length());
	ASSERT_EQUAL("test_buffer_copy", original, expected);
	RETURN_TEST("test_buffer_copy", 0);
}

int test_buffer_move() {
	Buffer buffer;
	buffer.Append("Hello", 5);
	Buffer buffer2(std::move(buffer));
	std::string original("Hello"), expected(buffer2.Data(), buffer2.Length());
	ASSERT_EQUAL("test_buffer_move", original, expected);
	ASSERT_EQUAL("test_buffer_move", 0, buffer.Length());
	RETURN_TEST("test_buffer_move", 0);
}

int test_buffer_append() {
	Buffer buffer;
	buffer.Append("Hello", 5);
	buffer.Append(" World!", 7);
	std::string original("Hello World!"), expected(buffer.Data(), buffer.Length());
	ASSERT_EQUAL("test_buffer_append", original, expected);
	RETURN_TEST("test_buffer_append", 0);
}

int main() {
    int result = 0;
	result += test_simple_buffer();
	result += test_buffer_copy();
	result += test_buffer_move();
	result += test_buffer_append();

    if (result == 0) {
        std::cout << "All tests passed!" << std::endl;
    } else {
        std::cout << result << " tests failed." << std::endl;
    }
    return result;
}
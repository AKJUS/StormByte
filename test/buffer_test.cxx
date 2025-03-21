#include <StormByte/util/buffer.hxx>
#include <StormByte/test_handlers.h>
#include <iostream>

using namespace StormByte::Util;

// Test comparisons
int test_simple_buffer() {
    Buffer buffer { "Hello", 5 };
	auto buffer_data = buffer.Data();
	std::string original("Hello"), expected(reinterpret_cast<const char*>(buffer.Data().data()), buffer.Size());
	ASSERT_EQUAL("test_simple_buffer", original, expected);
    RETURN_TEST("test_simple_buffer", 0);
}

int test_buffer_copy() {
	Buffer buffer1 { "Hello", 5 };
	Buffer buffer2(buffer1);
	std::string original(reinterpret_cast<const char*>(buffer1.Data().data()), buffer1.Size()), copy(reinterpret_cast<const char*>(buffer2.Data().data()), buffer2.Size());
	ASSERT_EQUAL("test_buffer_copy", original, copy);
	RETURN_TEST("test_buffer_copy", 0);
}

int test_buffer_move() {
	Buffer buffer1 { "Hello", 5 };
	Buffer buffer2(std::move(buffer1));
	std::string original("Hello"), expected(reinterpret_cast<const char*>(buffer2.Data().data()), buffer2.Size());
	ASSERT_EQUAL("test_buffer_move", original, expected);
	ASSERT_EQUAL("test_buffer_move", 0, buffer1.Size());
	RETURN_TEST("test_buffer_move", 0);
}

int test_buffer_append() {
	Buffer buffer;
	buffer << "Hello" << " " << "World!";
	std::string original("Hello World!"), expected(reinterpret_cast<const char*>(buffer.Data().data()), buffer.Size());
	ASSERT_EQUAL("test_buffer_append", original, expected);
	RETURN_TEST("test_buffer_append", 0);
}

int test_append_string() {
	Buffer buffer;
	std::string str = "Hello, World!";
	buffer << str;
	std::string expected(reinterpret_cast<const char*>(buffer.Data().data()), buffer.Size());
	ASSERT_EQUAL("test_append_string", str, expected);
	RETURN_TEST("test_append_string", 0);
}

int main() {
    int result = 0;
	result += test_simple_buffer();
	result += test_buffer_copy();
	result += test_buffer_move();
	result += test_buffer_append();
	result += test_append_string();

    if (result == 0) {
        std::cout << "All tests passed!" << std::endl;
    } else {
        std::cout << result << " tests failed." << std::endl;
    }
    return result;
}
#include <StormByte/buffers/simple.hxx>
#include <StormByte/test_handlers.h>

#include <iostream>
#include <thread>

using namespace StormByte;

// Test comparisons
int test_simple_buffer() {
	Buffers::Simple buffer { "Hello", 5 };
	auto buffer_data = buffer.Data();
	std::string original("Hello"), expected(reinterpret_cast<const char*>(buffer.Data().data()), buffer.Size());
	ASSERT_EQUAL("test_simple_buffer", original, expected);
	RETURN_TEST("test_simple_buffer", 0);
}

int test_buffer_copy() {
	Buffers::Simple buffer1 { "Hello", 5 };
	Buffers::Simple buffer2(buffer1);
	std::string original(reinterpret_cast<const char*>(buffer1.Data().data()), buffer1.Size()), copy(reinterpret_cast<const char*>(buffer2.Data().data()), buffer2.Size());
	ASSERT_EQUAL("test_buffer_copy", original, copy);
	RETURN_TEST("test_buffer_copy", 0);
}

int test_buffer_move() {
	Buffers::Simple buffer1 { "Hello", 5 };
	Buffers::Simple buffer2(std::move(buffer1));
	std::string original("Hello"), expected(reinterpret_cast<const char*>(buffer2.Data().data()), buffer2.Size());
	ASSERT_EQUAL("test_buffer_move", original, expected);
	ASSERT_EQUAL("test_buffer_move", 0, buffer1.Size());
	RETURN_TEST("test_buffer_move", 0);
}

int test_buffer_append() {
	Buffers::Simple buffer;
	buffer << "Hello" << " " << "World!";
	std::string original("Hello World!"), expected(reinterpret_cast<const char*>(buffer.Data().data()), buffer.Size());
	ASSERT_EQUAL("test_buffer_append", original, expected);
	RETURN_TEST("test_buffer_append", 0);
}

int test_append_string() {
	Buffers::Simple buffer;
	std::string str = "Hello, World!";
	buffer << str;
	std::string expected(reinterpret_cast<const char*>(buffer.Data().data()), buffer.Size());
	ASSERT_EQUAL("test_append_string", str, expected);
	RETURN_TEST("test_append_string", 0);
}

int test_peek_function() {
	Buffers::Simple buffer { "Hello", 5 };
	auto peeked = buffer.Peek();
	ASSERT_TRUE("test_peek_function", peeked.has_value());
	ASSERT_EQUAL("test_peek_function", static_cast<char>(peeked.value()), 'H');

	auto read = buffer.Read(1);
	ASSERT_TRUE("test_peek_function", read.has_value());
	ASSERT_EQUAL("test_peek_function", static_cast<char>(read.value()[0]), 'H');

	RETURN_TEST("test_peek_function", 0);
}

int main() {
	int result = 0;
	result += test_simple_buffer();
	result += test_buffer_copy();
	result += test_buffer_move();
	result += test_buffer_append();
	result += test_append_string();
	result += test_peek_function();

	if (result == 0) {
		std::cout << "All tests passed!" << std::endl;
	} else {
		std::cout << result << " tests failed." << std::endl;
	}
	return result;
}
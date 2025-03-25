#include <StormByte/buffer.hxx>
#include <StormByte/test_handlers.h>

#include <iostream>
#include <thread>

using namespace StormByte;

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

int test_concurrent_writes() {
	Buffer buffer;

	auto writer = [&buffer](const std::string& data) {
		for (int i = 0; i < 100; ++i) {
			buffer << data; // Append data multiple times
		}
	};

	std::thread t1(writer, "Thread1_");
	std::thread t2(writer, "Thread2_");

	t1.join();
	t2.join();

	std::string result(reinterpret_cast<const char*>(buffer.Data().data()), buffer.Size());

	// Verify total buffer size
	size_t expected_size = 100 * 8 * 2; // Each thread writes 100 * 8 characters ("Thread1_" or "Thread2_")
	ASSERT_EQUAL("test_concurrent_writes", expected_size, result.size());

	// Verify the number of occurrences of each word
	size_t thread1_count = 0;
	size_t thread2_count = 0;
	size_t pos = 0;

	// Count occurrences of "Thread1_"
	while ((pos = result.find("Thread1_", pos)) != std::string::npos) {
		++thread1_count;
		pos += 8; // Move past the word
	}

	pos = 0;

	// Count occurrences of "Thread2_"
	while ((pos = result.find("Thread2_", pos)) != std::string::npos) {
		++thread2_count;
		pos += 8; // Move past the word
	}

	ASSERT_EQUAL("test_concurrent_writes", 100, thread1_count); // Expect 100 occurrences of "Thread1_"
	ASSERT_EQUAL("test_concurrent_writes", 100, thread2_count); // Expect 100 occurrences of "Thread2_"

	RETURN_TEST("test_concurrent_writes", 0);
}

int test_concurrent_reads_and_writes() {
	Buffer buffer;

	auto writer = [&buffer]() {
		for (int i = 0; i < 100; ++i) {
			buffer << "Data";
		}
	};

	auto reader = [&buffer]() {
		for (int i = 0; i < 50; ++i) {
			auto data = buffer.Data();
			std::string dummy(reinterpret_cast<const char*>(data.data()), data.size()); // Simulate read
		}
	};

	std::thread t1(writer);
	std::thread t2(reader);
	std::thread t3(writer);
	std::thread t4(reader);

	t1.join();
	t2.join();
	t3.join();
	t4.join();

	// Final verification
	std::string result(reinterpret_cast<const char*>(buffer.Data().data()), buffer.Size());
	size_t data_count = std::count(result.begin(), result.end(), 'D'); // Count occurrences of 'D' (start of "Data")
	ASSERT_EQUAL("test_concurrent_reads_and_writes", 200, data_count); // 100 writes from each of 2 writer threads
	RETURN_TEST("test_concurrent_reads_and_writes", 0);
}

int main() {
	int result = 0;
	result += test_simple_buffer();
	result += test_buffer_copy();
	result += test_buffer_move();
	result += test_buffer_append();
	result += test_append_string();
	result += test_concurrent_writes();
	result += test_concurrent_reads_and_writes();

	if (result == 0) {
		std::cout << "All tests passed!" << std::endl;
	} else {
		std::cout << result << " tests failed." << std::endl;
	}
	return result;
}
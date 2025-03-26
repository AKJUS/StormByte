#include <StormByte/buffers/shared.hxx>
#include <StormByte/test_handlers.h>

#include <thread>

using namespace StormByte;

int test_concurrent_writes() {
	Buffers::Shared buffer;

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
	Buffers::Shared buffer;

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
	result += test_concurrent_writes();
	result += test_concurrent_reads_and_writes();

	if (result == 0) {
		std::cout << "All tests passed successfully" << std::endl;
	} else {
		std::cerr << "Some tests failed" << std::endl;
	}

	return result;
}
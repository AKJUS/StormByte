#include <StormByte/buffers/shared.hxx>
#include <StormByte/test_handlers.h>

#include <set>
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

int test_manual_locking() {
	Buffers::Shared buffer;

	// Pre-fill the buffer with some data
	buffer << "InitialData";

	auto writer = [&buffer]() {
		for (int i = 0; i < 100; ++i) {
			buffer << "WriterData";
		}
	};

	// Lock the buffer manually
	buffer.Lock();

	// Get a span view of the data while the buffer is locked
	auto span_data = buffer.Span();
	std::string span_result(reinterpret_cast<const char*>(span_data.data()), span_data.size());

	// Start a writer thread that tries to modify the buffer
	std::thread t1(writer);

	// Simulate some processing while the buffer is locked
	std::this_thread::sleep_for(std::chrono::milliseconds(100));

	// Verify that the buffer content has not changed
	ASSERT_EQUAL("test_manual_locking", "InitialData", span_result);

	// Unlock the buffer
	buffer.Unlock();

	// Wait for the writer thread to finish
	t1.join();

	// Verify that the buffer was correctly unlocked and modified
	std::string final_result(reinterpret_cast<const char*>(buffer.Data().data()), buffer.Size());
	size_t writer_data_count = std::count(final_result.begin(), final_result.end(), 'W'); // Count occurrences of 'W' (start of "WriterData")
	ASSERT_EQUAL("test_manual_locking", 100, writer_data_count); // Expect 100 occurrences of "WriterData"

	RETURN_TEST("test_manual_locking", 0);
}

int test_write_after_eof() {
	Buffers::Shared buffer;

	// Set the buffer to EOF
	buffer << Buffers::Status::EoF;

	// Attempt to write to the buffer
	Buffers::Write::Status status1 = buffer.Write("TestData1");
	Buffers::Write::Status status2 = buffer.Write("TestData2");

	// Verify that the writes return an error
	ASSERT_TRUE("test_write_after_eof", Buffers::Write::Status::Error == status1);
	ASSERT_TRUE("test_write_after_eof", Buffers::Write::Status::Error == status2);

	// Verify that the buffer content remains unchanged
	std::string result(reinterpret_cast<const char*>(buffer.Data().data()), buffer.Size());
	ASSERT_EQUAL("test_write_after_eof", "", result); // Buffer should remain empty

	RETURN_TEST("test_write_after_eof", 0);
}

int test_process_shared_buffer_multithreaded() {
	Buffers::Shared input_buffer;
	Buffers::Shared output_buffer;

	// Add data to the input buffer
	std::string initial_data = "Hello, World!";
	for (int i = 0; i < 10; ++i) {
		input_buffer << initial_data; // Add the same data multiple times
	}

	// Define a processing function that converts all characters to uppercase
	auto to_uppercase = [](const Buffers::Simple& buffer) -> std::shared_ptr<Buffers::Simple> {
		auto data = buffer.Data();
		auto result = std::make_shared<Buffers::Simple>();
		for (auto byte : data) {
			char c = static_cast<char>(byte);
			result->Write(std::string(1, std::toupper(c)));
		}
		return result;
	};

	// Define a thread-safe processing task
	auto process_task = [&input_buffer, &output_buffer, &to_uppercase]() {
		while (!input_buffer.Empty()) {
			input_buffer.Process(13, to_uppercase, output_buffer); // Process 13 bytes (length of "Hello, World!")
		}
	};

	// Launch multiple threads to process the data concurrently
	std::thread t1(process_task);
	std::thread t2(process_task);

	t1.join();
	t2.join();

	// Verify the output buffer content
	std::string expected_output;
	for (int i = 0; i < 10; ++i) {
		expected_output += "HELLO, WORLD!";
	}

	std::string actual_output(reinterpret_cast<const char*>(output_buffer.Data().data()), output_buffer.Size());
	ASSERT_EQUAL("test_process_shared_buffer_multithreaded", expected_output, actual_output);

	// Verify that the input buffer is empty after processing
	ASSERT_TRUE("test_process_shared_buffer_multithreaded", input_buffer.Empty());

	RETURN_TEST("test_process_shared_buffer_multithreaded", 0);
}

int test_extract_into_multithreaded() {
	Buffers::Shared source_buffer;

	// Add data to the source buffer
	std::string initial_data = "Hello, World!";
	for (int i = 0; i < 100; ++i) {
		source_buffer << initial_data; // Add the same data multiple times
	}

	Buffers::Shared target_buffer1;
	Buffers::Shared target_buffer2;

	// Define a task to extract data into a target buffer
	auto extract_task = [&source_buffer](Buffers::Shared& target_buffer, int iterations, std::size_t length) {
		for (int i = 0; i < iterations; ++i) {
			source_buffer.ExtractInto(length, target_buffer);
		}
	};

	// Launch multiple threads to extract data concurrently
	std::thread t1(extract_task, std::ref(target_buffer1), 50, initial_data.size()); // Extract 50 times into target_buffer1
	std::thread t2(extract_task, std::ref(target_buffer2), 50, initial_data.size()); // Extract 50 times into target_buffer2

	t1.join();
	t2.join();

	// Verify that the source buffer is empty after all extractions
	ASSERT_TRUE("test_extract_into_multithreaded", source_buffer.Empty());

	// Verify the content of the target buffers
	std::string expected_output;
	for (int i = 0; i < 50; ++i) {
		expected_output += initial_data;
	}

	std::string actual_output1(reinterpret_cast<const char*>(target_buffer1.Data().data()), target_buffer1.Size());
	std::string actual_output2(reinterpret_cast<const char*>(target_buffer2.Data().data()), target_buffer2.Size());

	ASSERT_EQUAL("test_extract_into_multithreaded (target_buffer1)", expected_output, actual_output1);
	ASSERT_EQUAL("test_extract_into_multithreaded (target_buffer2)", expected_output, actual_output2);

	RETURN_TEST("test_extract_into_multithreaded", 0);
}

int test_shared_available_bytes() {
    Buffers::Shared buffer;

    // Add data to the buffer
    std::string data = "Hello, World!";
    buffer << data;

    // Verify the initial available bytes
    ASSERT_EQUAL("test_shared_available_bytes (initial)", data.size(), buffer.AvailableBytes());

    // Read 7 bytes ("Hello, ")
    buffer.Read(7);

    // Verify the available bytes after reading
    ASSERT_EQUAL("test_shared_available_bytes (after read)", data.size() - 7, buffer.AvailableBytes());

    // Read the remaining bytes
    buffer.Read(buffer.AvailableBytes());

    // Verify the available bytes after reading everything
    ASSERT_EQUAL("test_shared_available_bytes (after full read)", 0, buffer.AvailableBytes());

    RETURN_TEST("test_shared_available_bytes", 0);
}

int main() {
	int result = 0;
	result += test_concurrent_writes();
	result += test_concurrent_reads_and_writes();
	result += test_manual_locking();
	result += test_write_after_eof();
	result += test_process_shared_buffer_multithreaded();
	result += test_extract_into_multithreaded();
	result += test_shared_available_bytes();

	if (result == 0) {
		std::cout << "All tests passed!" << std::endl;
	} else {
		std::cout << result << " tests failed." << std::endl;
	}

	return result;
}
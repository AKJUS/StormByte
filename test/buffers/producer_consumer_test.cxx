#include <StormByte/buffers/producer.hxx>
#include <StormByte/buffers/consumer.hxx>
#include <StormByte/test_handlers.h>

#include <condition_variable>
#include <thread>
#include <string>

using namespace StormByte;

int test_producer_to_consumer() {
	// Create an Buffers::Async and link ProducerBuffer and ConsumerBuffer
	auto async_buffer = std::make_shared<Buffers::Async>();
	auto producer = async_buffer->Producer();
	auto consumer = async_buffer->Consumer();

	// Producer writes data
	*producer << "Hello, " << "World!";

	// Consumer reads data
	auto extracted_data = consumer->Extract(13); // Length of "Hello, World!"
	ASSERT_TRUE("test_producer_to_consumer", extracted_data.has_value());

	std::string expected = "Hello, World!";
	std::string result(reinterpret_cast<const char*>(extracted_data->data()), extracted_data->size());
	ASSERT_EQUAL("test_producer_to_consumer", expected, result);

	RETURN_TEST("test_producer_to_consumer", 0);
}

int test_producer_reserve() {
	// Create an Buffers::Async and link ProducerBuffer
	auto async_buffer = std::make_shared<Buffers::Async>();
	auto producer = async_buffer->Producer();

	// Reserve space in the buffer
	producer->Reserve(1024);

	// Verify that reserving does not increase the buffer size but allows writing
	ASSERT_EQUAL("test_producer_reserve", 0, async_buffer->Size());

	// Write data to ensure the reserved space is usable
	std::string test_data(1024, 'A'); // Create a string of 1024 'A's
	*producer << test_data;

	// Verify that the buffer contains the data
	ASSERT_EQUAL("test_producer_reserve", 1024, async_buffer->Size());

	RETURN_TEST("test_producer_reserve", 0);
}

int test_producer_consumer_concurrent() {
	// Create an Buffers::Async and link ProducerBuffer and ConsumerBuffer
	auto async_buffer = std::make_shared<Buffers::Async>();
	auto producer = async_buffer->Producer();
	auto consumer = async_buffer->Consumer();

	// Producer writes data in a separate thread
	auto producer_thread = [producer]() {
		for (int i = 0; i < 100; ++i) {
			*producer << "Data" << std::to_string(i) << " "; // Convert int to std::string
		}
		*producer << Buffers::Status::EoF; // Signal EOF
	};

	// Consumer reads data in a separate thread
	auto consumer_thread = [consumer]() {
		while (true) {
			auto data = consumer->Extract(10); // Read chunks of 10 bytes
			if (!data) break; // Stop if no more data is available
		}
	};

	std::thread t1(producer_thread);
	std::thread t2(consumer_thread);

	t1.join();
	t2.join();

	// Verify that the buffer is empty after all reads
	ASSERT_EQUAL("test_producer_consumer_concurrent", 0, async_buffer->Size());

	RETURN_TEST("test_producer_consumer_concurrent", 0);
}

int test_consumer_wait_for_data() {
	// Create an Buffers::Async and link ProducerBuffer and ConsumerBuffer
	auto async_buffer = std::make_shared<Buffers::Async>();
	auto producer = async_buffer->Producer();
	auto consumer = async_buffer->Consumer();

	// Consumer waits for data in a separate thread
	bool data_received = false;
	std::mutex mtx;
	std::condition_variable cv;
	bool consumer_ready = false;

	auto consumer_thread = [&consumer, &data_received, &mtx, &cv, &consumer_ready]() {
		{
			std::lock_guard<std::mutex> lock(mtx);
			consumer_ready = true;
			cv.notify_one();
		}
		auto data = consumer->Extract(5); // Wait for 5 bytes
		if (data) {
			data_received = true;
			std::string result(reinterpret_cast<const char*>(data->data()), data->size());
			ASSERT_EQUAL("test_consumer_wait_for_data", "Hello", result);
			RETURN_TEST("test_consumer_wait_for_data", 0);
		} else {
			RETURN_TEST("test_consumer_wait_for_data", 1);
		}
	};

	std::thread t(consumer_thread);

	// Wait for the consumer to be ready
	{
		std::unique_lock<std::mutex> lock(mtx);
		cv.wait(lock, [&consumer_ready]() { return consumer_ready; });
	}

	// Producer writes data after a delay
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	*producer << "Hello";

	t.join();

	// Verify that the consumer received the data
	ASSERT_TRUE("test_consumer_wait_for_data", data_received);

	RETURN_TEST("test_consumer_wait_for_data", 0);
}

int test_producer_to_consumer_eof() {
	// Create an Buffers::Async and link ProducerBuffer and ConsumerBuffer
	auto async_buffer = std::make_shared<Buffers::Async>();
	auto producer = async_buffer->Producer();
	auto consumer = async_buffer->Consumer();

	// Producer writes data and sets EOF
	*producer << "End of Data";
	*async_buffer << Buffers::Status::EoF; // Use operator<< for smart pointer

	// Consumer reads data
	auto extracted_data = consumer->Extract(11); // Length of "End of Data"
	ASSERT_TRUE("test_producer_to_consumer_eof", extracted_data.has_value());

	std::string expected = "End of Data";
	std::string result(reinterpret_cast<const char*>(extracted_data->data()), extracted_data->size());
	ASSERT_EQUAL("test_producer_to_consumer_eof", expected, result);

	// Verify that the buffer status is EOF
	ASSERT_TRUE("test_producer_to_consumer_eof", (Buffers::Status::EoF == async_buffer->Status()));

	RETURN_TEST("test_producer_to_consumer_eof", 0);
}

int test_consumer_skip() {
	// Create an Buffers::Async and link ProducerBuffer and ConsumerBuffer
	auto async_buffer = std::make_shared<Buffers::Async>();
	auto producer = async_buffer->Producer();
	auto consumer = async_buffer->Consumer();

	// Producer thread: Writes "Hello World!" to the buffer and sets EOF
	auto producer_thread = [producer]() -> void {
		*producer << "Hello World!";
		*producer << Buffers::Status::EoF; // Signal EOF
	};

	// Consumer thread: Skips "Hello " and verifies the remaining part is "World!"
	auto consumer_thread = [consumer]() -> int {
		// Skip the first 6 characters ("Hello ")
		auto skip_result = consumer->Skip(6);
		ASSERT_TRUE("test_consumer_skip", skip_result.has_value());

		// Extract the remaining part ("World!")
		auto extracted_data = consumer->Extract(6); // Length of "World!"
		ASSERT_TRUE("test_consumer_skip", extracted_data.has_value());

		std::string expected = "World!";
		std::string result(reinterpret_cast<const char*>(extracted_data->data()), extracted_data->size());
		ASSERT_EQUAL("test_consumer_skip", expected, result);

		RETURN_TEST("test_consumer_skip", 0);
	};

	// Start the producer and consumer threads
	std::thread t1(producer_thread);
	int consumer_result = 0;
	std::thread t2([&consumer_thread, &consumer_result]() {
		consumer_result = consumer_thread();
	});

	// Wait for both threads to finish
	t1.join();
	t2.join();

	// Verify that the buffer is empty after all reads
	ASSERT_EQUAL("test_consumer_skip", 0, async_buffer->Size());

	RETURN_TEST("test_consumer_skip", consumer_result);
}

int test_consumer_partial_read() {
	// Create an Buffers::Async and link ProducerBuffer and ConsumerBuffer
	auto async_buffer = std::make_shared<Buffers::Async>();
	auto producer = async_buffer->Producer();
	auto consumer = async_buffer->Consumer();

	// Producer writes a small amount of data
	*producer << "Hi";

	// Consumer attempts to read more data than available using Mode::Partial
	auto extracted_data = consumer->Extract(5, Buffers::Read::Mode::Partial); // Request 5 bytes
	ASSERT_TRUE("test_consumer_partial_read", extracted_data.has_value());

	// Verify that only the available data is returned
	std::string expected = "Hi";
	std::string result(reinterpret_cast<const char*>(extracted_data->data()), extracted_data->size());
	ASSERT_EQUAL("test_consumer_partial_read", expected, result);

	RETURN_TEST("test_consumer_partial_read", 0);
}

int test_consumer_wait_for_producer() {
    // Create an Async buffer and link ProducerBuffer and ConsumerBuffer
    auto async_buffer = std::make_shared<Buffers::Async>();
    auto producer = async_buffer->Producer();
    auto consumer = async_buffer->Consumer();

    // Consumer thread: Waits for the producer to finish processing
    bool consumer_finished = false;
    int consumer_result = 0;
    std::thread consumer_thread([&consumer, &consumer_finished, &consumer_result]() -> int {
        // Wait for the producer to mark the buffer as EoF or Error
        consumer->Wait();

        // Verify that the buffer status is EoF
        ASSERT_TRUE("test_consumer_wait_for_producer", consumer->Status() == Buffers::Status::EoF);

        // Mark the consumer as finished
        consumer_finished = true;
        return 0; // Return success
    });

    // Producer thread: Writes data and marks the buffer as EoF
    std::thread producer_thread([&producer]() -> int {
        // Simulate some processing
        std::this_thread::sleep_for(std::chrono::milliseconds(200));

        // Write data to the buffer
        *producer << "Producer Data";

        // Mark the buffer as EoF
        *producer << Buffers::Status::EoF;

        return 0; // Return success
    });

    // Wait for both threads to finish
    producer_thread.join();
    consumer_thread.join();

    // Verify that the consumer finished successfully
    ASSERT_TRUE("test_consumer_wait_for_producer", consumer_finished);

    RETURN_TEST("test_consumer_wait_for_producer", consumer_result);
}

int main() {
	int result = 0;
	result += test_producer_to_consumer();
	result += test_producer_reserve();
	result += test_producer_consumer_concurrent();
	result += test_consumer_wait_for_data();
	result += test_producer_to_consumer_eof();
	result += test_consumer_skip();
	result += test_consumer_partial_read(); // Add the new test here
	result += test_consumer_wait_for_producer(); // Add the new test here

	if (result == 0) {
		std::cout << "All tests passed!" << std::endl;
	} else {
		std::cout << result << " tests failed." << std::endl;
	}
	return result;
}
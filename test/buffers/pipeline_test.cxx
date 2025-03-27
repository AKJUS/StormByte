#include <StormByte/buffers/pipeline.hxx>
#include <StormByte/buffers/consumer.hxx>
#include <StormByte/buffers/producer.hxx>
#include <StormByte/test_handlers.h>

#include <thread>
#include <string>
#include <vector>
#include <memory>

using namespace StormByte::Buffers;

int test_pipeline_success() {
    // Create a Pipeline
    Pipeline pipeline;

    // Add two functions to the pipeline
    pipeline.AddPipe([](ConsumerPtr input, ProducerPtr output) -> bool {
        auto data = input->Extract(5); // Extract 5 bytes
        if (!data) return false;
        // Convert std::byte data to std::string
        std::string str(reinterpret_cast<const char*>(data->data()), data->size());
        *output << str + " World";
        return true;
    });

    pipeline.AddPipe([](ConsumerPtr input, ProducerPtr output) -> bool {
        auto data = input->Extract(11); // Extract 11 bytes
        if (!data) return false;
        // Convert std::byte data to std::string
        std::string str(reinterpret_cast<const char*>(data->data()), data->size());
        *output << str + "!";
        return true;
    });

    // Create an Async buffer and write initial data
    auto async_buffer = std::make_shared<Async>();
    *async_buffer->Producer() << "Hello";

    // Process the pipeline
    auto result = pipeline.Process(async_buffer->Consumer());

    // Wait for the pipeline to complete
    while (result->Status() == Status::Ready) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    // Verify the final output
    auto final_data = result->Extract(12); // Length of "Hello World!"
    ASSERT_TRUE("test_pipeline_success", final_data.has_value());

    std::string expected = "Hello World!";
    std::string actual(reinterpret_cast<const char*>(final_data->data()), final_data->size());
    ASSERT_EQUAL("test_pipeline_success", expected, actual);

    RETURN_TEST("test_pipeline_success", 0);
}

int test_pipeline_failure() {
    // Create a Pipeline
    Pipeline pipeline;

    // Add two functions to the pipeline
    pipeline.AddPipe([](ConsumerPtr input, ProducerPtr output) -> bool {
        auto data = input->Extract(5); // Extract 5 bytes
        if (!data) return false;
        // Convert std::byte data to std::string
        std::string str(reinterpret_cast<const char*>(data->data()), data->size());
        *output << str + " World";
        return true;
    });

    pipeline.AddPipe([](ConsumerPtr input, ProducerPtr output) -> bool {
        // Simulate a failure
        return false;
    });

    // Create an Async buffer and write initial data
    auto async_buffer = std::make_shared<Async>();
    *async_buffer->Producer() << "Hello";

    // Process the pipeline
    auto result = pipeline.Process(async_buffer->Consumer());

    // Wait for the pipeline to complete
    while (result->Status() == Status::Ready) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    // Verify the final buffer status is Error
    ASSERT_TRUE("test_pipeline_failure", result->Status() == Status::Error);

    RETURN_TEST("test_pipeline_failure", 0);
}

int test_pipeline_partial_processing() {
    // Create a Pipeline
    Pipeline pipeline;

    // Add three functions to the pipeline
    pipeline.AddPipe([](ConsumerPtr input, ProducerPtr output) -> bool {
        auto data = input->Extract(5); // Extract 5 bytes
        if (!data) return false;
        // Convert std::byte data to std::string
        std::string str(reinterpret_cast<const char*>(data->data()), data->size());
        *output << str + " World";
        return true;
    });

    pipeline.AddPipe([](ConsumerPtr input, ProducerPtr output) -> bool {
        auto data = input->Extract(11); // Extract 11 bytes
        if (!data) return false;
        // Convert std::byte data to std::string
        std::string str(reinterpret_cast<const char*>(data->data()), data->size());
        *output << str + "!";
        return true;
    });

    pipeline.AddPipe([](ConsumerPtr input, ProducerPtr output) -> bool {
        auto data = input->Extract(12); // Extract 12 bytes
        if (!data) return false;
        // Convert std::byte data to std::string
        std::string str(reinterpret_cast<const char*>(data->data()), data->size());
        *output << str + " Done";
        return true;
    });

    // Create an Async buffer and write initial data
    auto async_buffer = std::make_shared<Async>();
    *async_buffer->Producer() << "Hello";

    // Process the pipeline
    auto result = pipeline.Process(async_buffer->Consumer());

    // Wait for the pipeline to complete
    while (result->Status() == Status::Ready) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    // Verify the final output
    auto final_data = result->Extract(17); // Length of "Hello World! Done"
    ASSERT_TRUE("test_pipeline_partial_processing", final_data.has_value());

    std::string expected = "Hello World! Done";
    std::string actual(reinterpret_cast<const char*>(final_data->data()), final_data->size());
    ASSERT_EQUAL("test_pipeline_partial_processing", expected, actual);

    RETURN_TEST("test_pipeline_partial_processing", 0);
}

int test_pipeline_all_threads_finished() {
    // Create a Pipeline
    Pipeline pipeline;

    // Shared atomic variables to track thread completion
    std::atomic<bool> thread1_finished = false;
    std::atomic<bool> thread2_finished = false;
    std::atomic<bool> thread3_finished = false;

    // Add three functions to the pipeline
    pipeline.AddPipe([&thread1_finished](ConsumerPtr input, ProducerPtr output) -> bool {
        auto data = input->Extract(5); // Extract 5 bytes
        if (!data) return false;
        // Convert std::byte data to std::string
        std::string str(reinterpret_cast<const char*>(data->data()), data->size());
        *output << str + " World";
        thread1_finished = true; // Mark thread as finished
        return true;
    });

    pipeline.AddPipe([&thread2_finished](ConsumerPtr input, ProducerPtr output) -> bool {
        auto data = input->Extract(11); // Extract 11 bytes
        if (!data) return false;
        // Convert std::byte data to std::string
        std::string str(reinterpret_cast<const char*>(data->data()), data->size());
        *output << str + "!";
        thread2_finished = true; // Mark thread as finished
        return true;
    });

    pipeline.AddPipe([&thread3_finished](ConsumerPtr input, ProducerPtr output) -> bool {
        auto data = input->Extract(12); // Extract 12 bytes
        if (!data) return false;
        // Convert std::byte data to std::string
        std::string str(reinterpret_cast<const char*>(data->data()), data->size());
        *output << str + " Done";
        thread3_finished = true; // Mark thread as finished
        return true;
    });

    // Create an Async buffer and write initial data
    auto async_buffer = std::make_shared<Async>();
    *async_buffer->Producer() << "Hello";

    // Process the pipeline
    auto result = pipeline.Process(async_buffer->Consumer());

    // Wait for the pipeline to complete
    while (result->Status() == Status::Ready) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    // Verify the final buffer status is EoF
    ASSERT_TRUE("test_pipeline_all_threads_finished", result->Status() == Status::EoF);

    // Verify that all threads have finished
    ASSERT_TRUE("test_pipeline_all_threads_finished", thread1_finished.load());
    ASSERT_TRUE("test_pipeline_all_threads_finished", thread2_finished.load());
    ASSERT_TRUE("test_pipeline_all_threads_finished", thread3_finished.load());

    RETURN_TEST("test_pipeline_all_threads_finished", 0);
}

int test_pipeline_integer_processing() {
    // Create a Pipeline
    Pipeline pipeline;

    // Input array of integers
    std::vector<int> input_integers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::cout << "Input integers: ";
    for (const auto& num : input_integers) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    // Convert the input array of integers to a std::vector<std::byte>
    std::vector<std::byte> input_bytes(reinterpret_cast<std::byte*>(input_integers.data()),
                                       reinterpret_cast<std::byte*>(input_integers.data() + input_integers.size()));

    // Add four functions to the pipeline
    pipeline.AddPipe([](ConsumerPtr input, ProducerPtr output) -> bool {
        std::cout << "[Pipe 1] Started" << std::endl;
        while (true) {
            auto data = input->Extract(sizeof(int)); // Extract one integer (sizeof(int) bytes)
            if (!data) break;

            int num = *reinterpret_cast<int*>(data->data()); // Convert std::byte data back to an integer
            std::cout << "[Pipe 1] Received integer: " << num << std::endl;
            num *= 2; // Multiply the integer by 2
            std::cout << "[Pipe 1] Processed integer (multiplied by 2): " << num << std::endl;

            std::vector<std::byte> output_bytes(reinterpret_cast<std::byte*>(&num),
                                                reinterpret_cast<std::byte*>(&num) + sizeof(int));
            *output << output_bytes; // Write to output
            std::cout << "[Pipe 1] Wrote to output buffer" << std::endl;
        }
        std::cout << "[Pipe 1] End" << std::endl;
        return true;
    });

    pipeline.AddPipe([](ConsumerPtr input, ProducerPtr output) -> bool {
        std::cout << "[Pipe 2] Started" << std::endl;
        while (true) {
            auto data = input->Extract(sizeof(int)); // Extract one integer (sizeof(int) bytes)
            if (!data) break;

            int num = *reinterpret_cast<int*>(data->data()); // Convert std::byte data back to an integer
            std::cout << "[Pipe 2] Received integer: " << num << std::endl;
            num += 5; // Add 5 to the integer
            std::cout << "[Pipe 2] Processed integer (added 5): " << num << std::endl;

            std::vector<std::byte> output_bytes(reinterpret_cast<std::byte*>(&num),
                                                reinterpret_cast<std::byte*>(&num) + sizeof(int));
            *output << output_bytes; // Write to output
            std::cout << "[Pipe 2] Wrote to output buffer" << std::endl;
        }
        std::cout << "[Pipe 2] End" << std::endl;
        return true;
    });

    pipeline.AddPipe([](ConsumerPtr input, ProducerPtr output) -> bool {
        std::cout << "[Pipe 3] Started" << std::endl;
        while (true) {
            auto data = input->Extract(sizeof(int)); // Extract one integer (sizeof(int) bytes)
            if (!data) break;

            int num = *reinterpret_cast<int*>(data->data()); // Convert std::byte data back to an integer
            std::cout << "[Pipe 3] Received integer: " << num << std::endl;
            num -= 5; // Subtract 5 from the integer
            std::cout << "[Pipe 3] Processed integer (subtracted 5): " << num << std::endl;

            std::vector<std::byte> output_bytes(reinterpret_cast<std::byte*>(&num),
                                                reinterpret_cast<std::byte*>(&num) + sizeof(int));
            *output << output_bytes; // Write to output
            std::cout << "[Pipe 3] Wrote to output buffer" << std::endl;
        }
        std::cout << "[Pipe 3] End" << std::endl;
        return true;
    });

    pipeline.AddPipe([](ConsumerPtr input, ProducerPtr output) -> bool {
        std::cout << "[Pipe 4] Started" << std::endl;
        while (true) {
            auto data = input->Extract(sizeof(int)); // Extract one integer (sizeof(int) bytes)
            if (!data) break;

            int num = *reinterpret_cast<int*>(data->data()); // Convert std::byte data back to an integer
            std::cout << "[Pipe 4] Received integer: " << num << std::endl;
            num /= 2; // Divide the integer by 2
            std::cout << "[Pipe 4] Processed integer (divided by 2): " << num << std::endl;

            std::vector<std::byte> output_bytes(reinterpret_cast<std::byte*>(&num),
                                                reinterpret_cast<std::byte*>(&num) + sizeof(int));
            *output << output_bytes; // Write to output
            std::cout << "[Pipe 4] Wrote to output buffer" << std::endl;
        }
        std::cout << "[Pipe 4] End" << std::endl;
        return true;
    });

    // Create an Async buffer and write the initial data and mark it as EoF
    auto start_buffer = std::make_shared<Async>();
    *start_buffer->Producer() << input_bytes << StormByte::Buffers::Status::EoF;

    // Process the pipeline
    auto result = pipeline.Process(start_buffer->Consumer());

    // Verify the final buffer status is EoF
    while (result->Status() == Status::Ready) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    ASSERT_TRUE("test_pipeline_integer_processing", result->Status() == Status::EoF);

    // Extract and verify integers one at a time
    for (size_t i = 0; i < input_integers.size(); ++i) {
        auto data = result->Extract(sizeof(int)); // Extract one integer (sizeof(int) bytes)
        ASSERT_TRUE("test_pipeline_integer_processing", data.has_value());

        int output_integer = *reinterpret_cast<int*>(data->data()); // Convert std::byte data back to an integer
        std::cout << "[Verification] Input integer: " << input_integers[i]
                  << ", Output integer: " << output_integer << std::endl;
        ASSERT_EQUAL("test_pipeline_integer_processing", input_integers[i], output_integer);
    }

    RETURN_TEST("test_pipeline_integer_processing", 0);
}

int main() {
    int result = 0;
    result += test_pipeline_success();
    result += test_pipeline_failure();
    result += test_pipeline_partial_processing();
    result += test_pipeline_all_threads_finished();
    result += test_pipeline_integer_processing(); // Add the new test here

    if (result == 0) {
        std::cout << "All tests passed!" << std::endl;
    } else {
        std::cout << result << " tests failed." << std::endl;
    }
    return result;
}
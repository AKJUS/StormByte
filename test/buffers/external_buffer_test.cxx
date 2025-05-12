#include <StormByte/buffer/external.hxx>
#include <StormByte/test_handlers.h>

#include <thread>
#include <iostream>
#include <atomic>

using namespace StormByte;

int test_external_buffer_with_reader_function() {
    std::atomic<int> counter = 0;

    // Define an external reader function that generates data
    Buffer::ExternalReaderFunction reader = [&counter]() -> Buffer::ExpectedData<Buffer::Exception> {
        std::cout << "[DEBUG] Reader function called. Counter: " << counter << std::endl;
        if (counter < 5) {
            ++counter;
            std::string data = "Data" + std::to_string(counter);
            std::cout << "[DEBUG] Reader generated data: " << data << std::endl;
            return Buffer::Data(reinterpret_cast<const std::byte*>(data.data()),
                                reinterpret_cast<const std::byte*>(data.data() + data.size()));
        }
        std::cout << "[DEBUG] Reader returning EOF exception." << std::endl;
        return Unexpected<Buffer::Exception>("End of data");
    };

    Buffer::External buffer(reader);

    // Verify that the buffer reads data from the external reader
    for (int i = 1; i <= 5; ++i) {
        std::cout << "[DEBUG] Iteration " << i << ": Checking AvailableBytes()." << std::endl;
        auto available = buffer.AvailableBytes();
        std::cout << "[DEBUG] AvailableBytes(): " << available << std::endl;

        if (available == 0 && buffer.IsEoF()) {
            std::cout << "[DEBUG] End of data stream detected." << std::endl;
            break;
        }

        ASSERT_TRUE("test_external_buffer_with_reader_function", available >= 5);

        std::cout << "[DEBUG] Iteration " << i << ": Reading data from buffer." << std::endl;
        auto data = buffer.Read(5); // Read 5 bytes ("Data1", "Data2", etc.)
        ASSERT_TRUE("test_external_buffer_with_reader_function", data.has_value());
        std::string result(reinterpret_cast<const char*>(data->data()), data->size());
        std::cout << "[DEBUG] Iteration " << i << ": Read data: " << result << std::endl;
        ASSERT_EQUAL("test_external_buffer_with_reader_function", "Data" + std::to_string(i), result);
    }

    // Verify that no more data is available
    std::cout << "[DEBUG] Checking if buffer is empty." << std::endl;
    auto available = buffer.AvailableBytes();
    std::cout << "[DEBUG] AvailableBytes(): " << available << std::endl;
    ASSERT_TRUE("test_external_buffer_with_reader_function (no more data)", available == 0);
    ASSERT_TRUE("test_external_buffer_with_reader_function (EOF)", buffer.IsEoF());
    auto data = buffer.Read(5);
    ASSERT_FALSE("test_external_buffer_with_reader_function (no more data)", data.has_value());

    RETURN_TEST("test_external_buffer_with_reader_function", 0);
}

int main() {
    int result = 0;
    result += test_external_buffer_with_reader_function();

    if (result == 0) {
        std::cout << "All tests passed!" << std::endl;
    } else {
        std::cout << result << " tests failed." << std::endl;
    }

    return result;
}
#include <StormByte/buffer/external.hxx>
#include <StormByte/test_handlers.h>

#include <thread>
#include <iostream>
#include <atomic>

using namespace StormByte;

class ReaderSimulator: public Buffer::Reader {
	public:
		ReaderSimulator() noexcept: Reader(), m_counter(0) {}
		ReaderSimulator(const ReaderSimulator& other) noexcept = default;
		ReaderSimulator(ReaderSimulator&& other) noexcept = default;
		~ReaderSimulator() noexcept override = default;
		ReaderSimulator& operator=(const ReaderSimulator& other) noexcept = default;
		ReaderSimulator& operator=(ReaderSimulator&& other) noexcept = default;
		std::shared_ptr<Reader> Clone() const noexcept override {
			return std::make_shared<ReaderSimulator>(*this);
		}
		std::shared_ptr<Reader> Move() noexcept override {
			return std::make_shared<ReaderSimulator>(std::move(*this));
		}
		// Simulate reading data from an external source
		Expected<Buffer::Simple, Buffer::Exception> Read(const size_t& min = 0) const noexcept override {
			if (m_counter < 5) {
				++m_counter;
				std::string data = "Data" + std::to_string(m_counter);
				return Buffer::Simple(data);
			}
			return Unexpected<Buffer::Exception>("End of data");
		}

	private:
		mutable uint8_t m_counter;
};

int test_external_buffer_with_reader_function() {
	ReaderSimulator reader;
    Buffer::External buffer(std::move(reader));

    // Verify that the buffer reads data from the external reader
    for (int i = 1; i <= 5; ++i) {
        std::cout << "[DEBUG] Iteration " << i << ": Checking HasEnoughData(5)." << std::endl;
        auto available = buffer.HasEnoughData(5);
        std::cout << "[DEBUG] HasEnoughData(): " << std::boolalpha << available << std::noboolalpha << std::endl;

        if (!available && buffer.IsEoF()) {
            std::cout << "[DEBUG] End of data stream detected." << std::endl;
            break;
        }

        ASSERT_TRUE("test_external_buffer_with_reader_function", available);

        std::cout << "[DEBUG] Iteration " << i << ": Reading data from buffer." << std::endl;
        auto data = buffer.Read(5); // Read 5 bytes ("Data1", "Data2", etc.)
        ASSERT_TRUE("test_external_buffer_with_reader_function", data.has_value());
        std::string result(reinterpret_cast<const char*>(data->data()), data->size());
        std::cout << "[DEBUG] Iteration " << i << ": Read data: " << result << std::endl;
        ASSERT_EQUAL("test_external_buffer_with_reader_function", "Data" + std::to_string(i), result);
    }

    // Verify that no more data is available
    std::cout << "[DEBUG] Checking if buffer is empty." << std::endl;
    auto available = buffer.HasEnoughData(5);
    std::cout << "[DEBUG] HasEnoughData(5): " << std::boolalpha << available << std::noboolalpha << std::endl;
    ASSERT_FALSE("test_external_buffer_with_reader_function (no more data)", available);
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
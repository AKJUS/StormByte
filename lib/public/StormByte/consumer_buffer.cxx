#include <StormByte/consumer_buffer.hxx>
#include <StormByte/system.hxx>

using namespace StormByte;

AsyncBuffer::ExpectedDataType ConsumerBuffer::Extract(const size_t& length) {
    // Check the buffer status immediately
    auto current_status = m_internal_buffer->status.load();
    if (current_status == AsyncBuffer::Status::Error) {
        return StormByte::Unexpected<BufferNotReady>(
            "Buffer is in an error state (status: {})", StatusString());
    }

    // Wait until enough data is available or the buffer status changes
    while (!m_internal_buffer->buffer->HasEnoughData(length)) {
        current_status = m_internal_buffer->status.load();

        // Handle Error status
        if (current_status == AsyncBuffer::Status::Error) {
            return StormByte::Unexpected<BufferNotReady>(
                "Buffer is in an error state while waiting (status: {})", StatusString());
        }

        // Handle EoF status
        if (current_status == AsyncBuffer::Status::EoF) {
            if (!m_internal_buffer->buffer->HasEnoughData(length)) {
                return StormByte::Unexpected<BufferException>(
                    "Buffer has reached EOF and does not have enough data (status: {})", StatusString());
            }
            break; // Proceed to extract remaining data
        }

        // Handle other statuses (e.g., Ready)
        if (current_status != AsyncBuffer::Status::Ready) {
            return StormByte::Unexpected<BufferNotReady>(
                "Buffer is not ready while waiting (status: {})", StatusString());
        }

        // Block the thread until the condition changes
        System::Sleep(std::chrono::milliseconds(100)); // Small sleep to avoid busy-waiting
    }

    // Extract the data from the buffer
    auto expected_data = m_internal_buffer->buffer->Extract(length);
    if (!expected_data) {
        return StormByte::Unexpected(expected_data.error());
    }

    // Return the extracted data
    return expected_data;
}

AsyncBuffer::ExpectedDataType ConsumerBuffer::Extract() {
    // Check the buffer status immediately
    if (m_internal_buffer->status.load() != AsyncBuffer::Status::Ready) {
        return StormByte::Unexpected<BufferNotReady>(
            "Buffer is not ready (status: {})", StatusString());
    }

    // Check if there is any data in the buffer
    if (m_internal_buffer->buffer->Size() == 0) {
        // Return an empty DataType (std::vector<std::byte>)
        return Buffer::DataType{};
    }

    // Extract all data from the buffer
    auto expected_data = m_internal_buffer->buffer->Extract(m_internal_buffer->buffer->Size());
    if (!expected_data)
        return StormByte::Unexpected(expected_data.error());

    // Return the extracted data
    return expected_data;
}

StormByte::Expected<void, BufferNotReady> ConsumerBuffer::Skip(const std::size_t& length) {
    // Call Extract to remove the specified number of characters from the buffer
    auto extract_result = Extract(length);

    // Propagate the error if Extract fails
    if (!extract_result) {
        return StormByte::Unexpected<BufferNotReady>(extract_result.error()->what());
    }

    // Successfully skipped the data
    return {};
}

ConsumerBuffer& ConsumerBuffer::operator<<(const enum Status& status) {
    AsyncBuffer::operator<<(status);
    return *this;
}
#include <StormByte/buffers/consumer.hxx>
#include <StormByte/system.hxx>

using namespace StormByte::Buffers;

Consumer::Consumer(const Async& async) noexcept: Async(async) {}

ExpectedData<StormByte::Buffers::Exception> Consumer::Extract(const size_t& length) {
	// Check the buffer status immediately
	auto current_status = Status();
	if (current_status == Status::Error) {
		return StormByte::Unexpected<BufferNotReady>(
			"Buffer is in an error state"
		);
	}

	// Wait until enough data is available or the buffer status changes
	while (!m_buffer->second.HasEnoughData(length)) {
		current_status = Status();

		// Handle Error status
		if (current_status == Status::Error) {
			return StormByte::Unexpected<BufferNotReady>(
				"Buffer is in an error state while waiting"
			);
		}

		// Handle EoF status
		if (current_status == Status::EoF) {
			if (!m_buffer->second.HasEnoughData(length)) {
				return StormByte::Unexpected<StormByte::Buffers::Exception>(
					"Buffer has reached EOF and does not have enough data"
				);
			}
			break; // Proceed to extract remaining data
		}

		// Handle other statuses (e.g., Ready)
		if (current_status != Status::Ready) {
			return StormByte::Unexpected<BufferNotReady>(
				"Buffer is not ready while waiting"
			);
		}

		// Block the thread until the condition changes
		System::Sleep(std::chrono::milliseconds(100)); // Small sleep to avoid busy-waiting
	}

	// Extract the data from the buffer
	auto expected_data = m_buffer->second.Extract(length);
	if (!expected_data) {
		return StormByte::Unexpected(expected_data.error());
	}

	// Return the extracted data
	return expected_data;
}

ExpectedData<StormByte::Buffers::Exception> Consumer::Extract() {
	// Check the buffer status immediately
	if (Status() != Status::Ready) {
		return StormByte::Unexpected<BufferNotReady>(
			"Buffer is not ready"
		);
	}

	// Check if there is any data in the buffer
	if (m_buffer->second.Size() == 0) {
		// Return an empty DataType (std::vector<std::byte>)
		return Buffers::Data {};
	}

	// Extract all data from the buffer
	auto expected_data = m_buffer->second.Extract(m_buffer->second.Size());
	if (!expected_data)
		return StormByte::Unexpected(expected_data.error());

	// Return the extracted data
	return expected_data;
}

StormByte::Expected<void, BufferNotReady> Consumer::Skip(const std::size_t& length) {
	// Call Extract to remove the specified number of characters from the buffer
	auto extract_result = Extract(length);

	// Propagate the error if Extract fails
	if (!extract_result) {
		return StormByte::Unexpected<BufferNotReady>(extract_result.error()->what());
	}

	// Successfully skipped the data
	return {};
}

Consumer& Consumer::operator<<(const enum Status& status) {
	Async::operator<<(status);
	return *this;
}
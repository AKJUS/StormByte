#include <StormByte/buffer/consumer.hxx>
#include <StormByte/buffer/producer.hxx>

using namespace StormByte::Buffer;

Consumer::Consumer() noexcept {
	m_shared = std::make_shared<Shared>();
}

// Constructor that initializes the consumer with a shared buffer
Consumer::Consumer(std::shared_ptr<Shared> shared) noexcept
:m_shared(std::move(shared)) {}

size_t Consumer::AvailableBytes() const noexcept {
	return m_shared->AvailableBytes();
}

// Retrieves a copy of the buffer data
Data Consumer::Data() const noexcept {
	return m_shared->Data();
}

// Discards data from the buffer
void Consumer::Discard(const std::size_t& length, const Read::Position& mode) noexcept {
	m_shared->Discard(length, mode);
}

// Checks if the read position is at the end
bool Consumer::End() const noexcept {
	return m_shared->End();
}

// Checks if the shared buffer is empty
bool Consumer::Empty() const noexcept {
	return m_shared->Empty();
}

// Extracts a specific size of data, taking ownership of the read data and removing it from the shared buffer
ExpectedData<BufferOverflow> Consumer::Extract(const size_t& length) {
	return m_shared->Extract(length);
}

// Extracts a specific size of data and moves it directly into the provided buffer
Read::Status Consumer::ExtractInto(const size_t& length, Shared& output) noexcept {
	return m_shared->ExtractInto(length, output);
}

// Checks if the shared buffer has enough data starting from the current read position
bool Consumer::HasEnoughData(const std::size_t& length) const {
	return m_shared->HasEnoughData(length);
}

// Retrieves the stored value as a hexadecimal string
std::string Consumer::HexData(const std::size_t& column_size) const {
	return m_shared->HexData(column_size);
}

// Checks if the buffer is at the end of the file/data
bool Consumer::IsEoF() const noexcept {
	return m_shared->IsEoF();
}

// Checks if the buffer is readable
bool Consumer::IsReadable() const noexcept {
	return m_shared->IsReadable();
}

// Peeks at the next byte in the shared buffer without advancing the read position
ExpectedByte<BufferOverflow> Consumer::Peek() const {
	return m_shared->Peek();
}

// Retrieves the read position
std::size_t Consumer::Position() const noexcept {
	return m_shared->Position();
}

// Reads a specific size of data starting from the current read position
ExpectedData<BufferOverflow> Consumer::Read(const size_t& length) const {
	return m_shared->Read(length);
}

// Moves the read pointer within the shared buffer based on the specified position and mode
void Consumer::Seek(const std::ptrdiff_t& position, const Read::Position& mode) const {
	m_shared->Seek(position, mode);
}

// Retrieves the length of the shared buffer
std::size_t Consumer::Size() const noexcept {
	return m_shared->Size();
}

// Gets the buffer status
enum Status Consumer::Status() const noexcept {
	return m_shared->Status();
}

class Producer Consumer::Producer() const noexcept {
	class Producer prod(*this);
	return prod;
}
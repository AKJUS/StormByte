#include <StormByte/buffers/producer.hxx>
#include <StormByte/buffers/consumer.hxx>

using namespace StormByte::Buffers;

// Default constructor
Producer::Producer() noexcept: m_shared(std::make_shared<Shared>()) {}

// Sets the buffer status
Producer& Producer::operator<<(const Status& status) {
	*m_shared << status;
	return *this;
}

// Appends a buffer to the current buffer
Producer& Producer::operator<<(const Simple& buffer) {
	*m_shared << buffer;
	return *this;
}

// Moves a buffer and appends it to the current buffer
Producer& Producer::operator<<(Simple&& buffer) {
	*m_shared << std::move(buffer);
	return *this;
}

// Appends a string to the current shared buffer
Producer& Producer::operator<<(const std::string& data) {
	*m_shared << data;
	return *this;
}

// Appends a byte vector to the current shared buffer
Producer& Producer::operator<<(const Buffers::Data& data) {
	*m_shared << data;
	return *this;
}

// Moves a byte vector and appends it to the current shared buffer
Producer& Producer::operator<<(Buffers::Data&& data) {
	*m_shared << std::move(data);
	return *this;
}

// Gets the consumer interface to consume this buffer
Consumer Producer::Consumer() const {
	return Buffers::Consumer(m_shared);
}

// Locks the shared buffer for exclusive access
void Producer::Lock() {
	m_shared->Lock();
}

// Reserves shared buffer size
void Producer::Reserve(const std::size_t& size) {
	m_shared->Reserve(size);
}

// Unlocks the shared buffer, releasing exclusive access
void Producer::Unlock() {
	m_shared->Unlock();
}

// Writes a simple buffer to the current shared buffer
Write::Status Producer::Write(const Simple& buffer) {
	return m_shared->Write(buffer);
}

// Moves a simple buffer and writes it to the current shared buffer
Write::Status Producer::Write(Simple&& buffer) {
	return m_shared->Write(std::move(buffer));
}

// Writes a string to the current shared buffer
Write::Status Producer::Write(const std::string& data) {
	return m_shared->Write(data);
}

// Writes a byte vector to the current shared buffer
Write::Status Producer::Write(const Buffers::Data& data) {
	return m_shared->Write(data);
}

// Moves a byte vector and writes it to the current shared buffer
Write::Status Producer::Write(Buffers::Data&& data) {
	return m_shared->Write(std::move(data));
}


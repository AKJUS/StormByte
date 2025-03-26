#include <StormByte/buffers/producer.hxx>
#include <StormByte/buffers/consumer.hxx>

using namespace StormByte::Buffers;

Producer::Producer(const Async& async) noexcept: Async(async) {}

Producer& Producer::operator<<(const Simple& buffer) {
	m_buffer->second << buffer;
	return *this;
}

Producer& Producer::operator<<(Simple&& buffer) {
	m_buffer->second << std::move(buffer);
	return *this;
}

Producer& Producer::operator<<(const std::string& data) {
	// Convert the string to bytes and append it to the buffer
	m_buffer->second << data;
	return *this;
}

Producer& Producer::operator<<(const Buffers::Data& data) {
	// Append the byte vector to the buffer
	m_buffer->second << data;
	return *this;
}

Producer& Producer::operator<<(Buffers::Data&& data) {
	// Move the byte vector into the buffer to avoid copying
	m_buffer->second << std::move(data);
	return *this;
}

Producer& Producer::operator<<(const Buffers::Status& status) {
	Async::operator<<(status);
	return *this;
}

void Producer::Reserve(const std::size_t& size) {
	// Reserve space in the buffer
	m_buffer->second.Reserve(size);
}
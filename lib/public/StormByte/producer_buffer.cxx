#include <StormByte/producer_buffer.hxx>
#include <StormByte/consumer_buffer.hxx>

using namespace StormByte;

ProducerBuffer& ProducerBuffer::operator<<(ConsumerBuffer& buffer) {
    // Extract data from the ConsumerBuffer and append it to this ProducerBuffer
    auto extracted_data = buffer.Extract();
    if (extracted_data) {
        m_internal_buffer->buffer << std::move(extracted_data.value());
    }
    return *this;
}

ProducerBuffer& ProducerBuffer::operator<<(const std::string& data) {
    // Convert the string to bytes and append it to the buffer
    m_internal_buffer->buffer << data;
    return *this;
}

ProducerBuffer& ProducerBuffer::operator<<(const Buffer::DataType& data) {
    // Append the byte vector to the buffer
    m_internal_buffer->buffer << data;
    return *this;
}

ProducerBuffer& ProducerBuffer::operator<<(Buffer::DataType&& data) {
    // Move the byte vector into the buffer to avoid copying
    m_internal_buffer->buffer << std::move(data);
    return *this;
}

ProducerBuffer& ProducerBuffer::operator<<(const enum Status& status) {
	AsyncBuffer::operator<<(status);
	return *this;
}

void ProducerBuffer::Reserve(const std::size_t& size) {
    // Reserve space in the buffer
    m_internal_buffer->buffer->Reserve(size);
}
#include <StormByte/consumer_buffer.hxx>
#include <StormByte/producer_buffer.hxx>

using namespace StormByte;

AsyncBuffer::AsyncBuffer() noexcept: m_internal_buffer(std::make_shared<InternalBuffer>()) {
	m_internal_buffer->buffer = std::make_shared<Buffer>();
	m_internal_buffer->status = AsyncBuffer::Status::Ready;
}

enum AsyncBuffer::Status AsyncBuffer::Status() const noexcept {
	return m_internal_buffer->status.load();
}

AsyncBuffer& AsyncBuffer::operator<<(const enum AsyncBuffer::Status& status) {
	m_internal_buffer->status.store(status);
	return *this;
}

std::shared_ptr<ConsumerBuffer> AsyncBuffer::Consumer() const noexcept {
	ConsumerBuffer consumer;
	consumer.m_internal_buffer = m_internal_buffer;
	return std::make_shared<ConsumerBuffer>(std::move(consumer));
}

std::shared_ptr<ProducerBuffer> AsyncBuffer::Producer() const noexcept {
	ProducerBuffer producer;
	producer.m_internal_buffer = m_internal_buffer;
	return std::make_shared<ProducerBuffer>(std::move(producer));
}

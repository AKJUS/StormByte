#include <StormByte/buffers/async.hxx>
#include <StormByte/buffers/consumer.hxx>
#include <StormByte/buffers/producer.hxx>

using namespace StormByte::Buffers;

Async::Async() noexcept: m_buffer(std::make_shared<BufferWithStatus>(Status::Ready, Shared())) {}

std::size_t Async::Size() const noexcept {
	return m_buffer->second.Size();
}

StormByte::Buffers::Status Async::Status() const noexcept {
	return m_buffer->first.load();
}

std::string Async::StatusToString() const noexcept {
	switch(Status()) {
		case Buffers::Status::Ready:	return "Ready";
		case Buffers::Status::EoF: 		return "EoF";
		case Buffers::Status::Error:
		default:
										return "Error";
	}
}

Async& Async::operator<<(const StormByte::Buffers::Status& status) {
	m_buffer->first.store(status);
	return *this;
}

std::shared_ptr<Consumer> Async::Consumer() const noexcept {
	return std::make_shared<class Consumer>(*this);
}

std::shared_ptr<Producer> Async::Producer() const noexcept {
	return std::make_shared<class Producer>(*this);
}

#include <StormByte/buffers/consumer.hxx>
#include <StormByte/buffers/pipeline.hxx>
#include <StormByte/buffers/producer.hxx>
#include <thread>

using namespace StormByte::Buffers;

void Pipeline::AddPipe(const PipeFunction& pipe) {
	m_pipes.push_back(pipe);
}

void Pipeline::AddPipe(PipeFunction&& pipe) {
	m_pipes.push_back(std::move(pipe));
}

Consumer Pipeline::Process(Shared&& buffer) const noexcept {
	Consumer buffer_chain = Producer(std::move(buffer)).Consumer();

	for (const auto& pipe: m_pipes) {
		Producer result;

		// Launch a detached thread for each pipe function
		std::thread([pipe, buffer_chain, result]() {
			pipe(buffer_chain, result);
		}).detach();

		// Update the buffer chain to the result's consumer
		buffer_chain = result.Consumer();
	}

	return buffer_chain;
}
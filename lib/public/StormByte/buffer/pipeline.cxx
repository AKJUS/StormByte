#include <StormByte/buffer/consumer.hxx>
#include <StormByte/buffer/pipeline.hxx>
#include <StormByte/buffer/producer.hxx>
#include <thread>

using namespace StormByte::Buffer;

void Pipeline::AddPipe(const PipeFunction& pipe) {
	m_pipes.push_back(pipe);
}

void Pipeline::AddPipe(PipeFunction&& pipe) {
	m_pipes.push_back(std::move(pipe));
}

Consumer Pipeline::Process(Consumer buffer) const noexcept {
	Consumer last_result = buffer;

	for (const auto& pipe: m_pipes) {
		Producer current_result;

		// Launch a detached thread for each pipe function
		std::thread([pipe, current_result, last_result]() {
			pipe(last_result, current_result);
		}).detach();

		// Update the buffer chain to the result's consumer
		last_result = current_result.Consumer();
	}

	return last_result;
}
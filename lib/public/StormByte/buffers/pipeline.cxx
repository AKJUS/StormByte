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

StormByte::Buffers::ConsumerPtr Pipeline::Process(Buffers::ConsumerPtr buffer) const noexcept {
    ConsumerPtr buffer_chain = buffer;

    for (const auto& pipe : m_pipes) {
        ProducerPtr result = std::make_shared<Producer>();

        // Launch a detached thread for each pipe function
        std::thread([pipe, buffer_chain, result]() {
            try {
                // Execute the pipe function and update the buffer status
                if (pipe(buffer_chain, result)) {
                    result << Buffers::Status::EoF;
                } else {
                    result << Buffers::Status::Error;
                }
            } catch (...) {
                // Handle unexpected exceptions and mark the buffer as Error
                result << Buffers::Status::Error;
            }
        }).detach();

        // Update the buffer chain to the result's consumer
        buffer_chain = result->Consumer();
    }

    return buffer_chain;
}
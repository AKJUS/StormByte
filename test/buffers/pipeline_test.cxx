#include <StormByte/buffers/pipeline.hxx>
#include <StormByte/test_handlers.h>
#include <iostream>
#include <vector>
#include <thread>
#include <chrono> // For std::this_thread::sleep_for

using namespace StormByte;

int test_pipeline_integer_operations() {
    Buffers::Shared input_buffer;

    // Prepare the input buffer with integer data
    std::vector<int> input_data;
    for (int i = 1; i <= 24; i++) { // Populate the vector with values from 1 to 24
        input_data.push_back(i);
    }

    // Convert the input data to Buffers::Data and write it to the buffer
    Buffers::Data data(reinterpret_cast<const std::byte*>(input_data.data()),
                       reinterpret_cast<const std::byte*>(input_data.data()) + input_data.size() * sizeof(int));
    input_buffer.Write(std::move(data)); // Use std::move to avoid unnecessary copy
    input_buffer << Buffers::Status::EoF; // Mark as finished

    // Define the pipeline
    Buffers::Pipeline pipeline;

    // Pipe 1: Multiply by 2
    pipeline.AddPipe([](Buffers::Consumer input, Buffers::Producer output) {
        while (!input.IsEoF() || input.HasEnoughData(sizeof(int))) {
            auto data = input.Read(sizeof(int));
            if (!data) {
                std::cout << "[Pipe 1]: Unexpected error: " << data.error()->what() << std::endl;
                continue;
            }

            int value = *reinterpret_cast<const int*>(data->data());
            std::cout << "[Pipe 1]: Read value " << value << std::endl;
            value *= 2;

            Buffers::Data output_data(reinterpret_cast<const std::byte*>(&value),
                                      reinterpret_cast<const std::byte*>(&value) + sizeof(int));
            std::cout << "[Pipe 1]: Writing value " << value << " to output buffer" << std::endl;
            output.Write(std::move(output_data)); // Use std::move to avoid unnecessary copy
        }
        output << Buffers::Status::EoF; // Mark as finished
    });

    // Pipe 2: Add 5
    pipeline.AddPipe([](Buffers::Consumer input, Buffers::Producer output) {
        while (!input.IsEoF() || input.HasEnoughData(sizeof(int))) {
            auto data = input.Read(sizeof(int));
            if (!data) {
                std::cout << "[Pipe 2]: Unexpected error: " << data.error()->what() << std::endl;
                continue;
            }

            int value = *reinterpret_cast<const int*>(data->data());
            std::cout << "[Pipe 2]: Read value " << value << std::endl;
            value += 5;

            Buffers::Data output_data(reinterpret_cast<const std::byte*>(&value),
                                      reinterpret_cast<const std::byte*>(&value) + sizeof(int));
            std::cout << "[Pipe 2]: Writing value " << value << " to output buffer" << std::endl;
            output.Write(std::move(output_data)); // Use std::move to avoid unnecessary copy
        }
        output << Buffers::Status::EoF; // Mark as finished
    });

    // Pipe 3: Subtract 5
    pipeline.AddPipe([](Buffers::Consumer input, Buffers::Producer output) {
        while (!input.IsEoF() || input.HasEnoughData(sizeof(int))) {
            auto data = input.Read(sizeof(int));
            if (!data) {
                std::cout << "[Pipe 3]: Unexpected error: " << data.error()->what() << std::endl;
                continue;
            }

            int value = *reinterpret_cast<const int*>(data->data());
            std::cout << "[Pipe 3]: Read value " << value << std::endl;
            value -= 5;

            Buffers::Data output_data(reinterpret_cast<const std::byte*>(&value),
                                      reinterpret_cast<const std::byte*>(&value) + sizeof(int));
            std::cout << "[Pipe 3]: Writing value " << value << " to output buffer" << std::endl;
            output.Write(std::move(output_data)); // Use std::move to avoid unnecessary copy
        }
        output << Buffers::Status::EoF; // Mark as finished
    });

    // Pipe 4: Divide by 2
    pipeline.AddPipe([](Buffers::Consumer input, Buffers::Producer output) {
        while (!input.IsEoF() || input.HasEnoughData(sizeof(int))) {
            auto data = input.Read(sizeof(int));
            if (!data) {
                std::cout << "[Pipe 4]: Unexpected error: " << data.error()->what() << std::endl;
                continue;
            }

            int value = *reinterpret_cast<const int*>(data->data());
            std::cout << "[Pipe 4]: Read value " << value << std::endl;
            value /= 2;

            Buffers::Data output_data(reinterpret_cast<const std::byte*>(&value),
                                      reinterpret_cast<const std::byte*>(&value) + sizeof(int));
            std::cout << "[Pipe 4]: Writing value " << value << " to output buffer" << std::endl;
            output.Write(std::move(output_data)); // Use std::move to avoid unnecessary copy
        }
        output << Buffers::Status::EoF; // Mark as finished
    });

	std::cout << "Input buffer:" << std::endl << input_buffer.HexData();
    // Process the pipeline
    Buffers::Consumer final_buffer = pipeline.Process(std::move(input_buffer));

    // Wait for the final buffer to be marked as EoF
    while (!final_buffer.IsEoF()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    // Verify the output buffer matches the input buffer
    std::cout << "[Final Buffer]: HexData: " << final_buffer.HexData() << std::endl;

    ASSERT_EQUAL("test_pipeline_integer_operations", input_data.size(), final_buffer.Size() / sizeof(int));
    for (size_t i = 0; i < input_data.size(); ++i) {
        auto data = final_buffer.Read(sizeof(int));
        if (!data) {
            std::cout << "[Final Buffer]: Unexpected error: " << data.error()->what() << std::endl;
            continue;
        }

        int value = *reinterpret_cast<const int*>(data->data());
        std::cout << "[Final Buffer]: Read value " << value << std::endl;
        ASSERT_EQUAL("test_pipeline_integer_operations", input_data[i], value);
    }

    RETURN_TEST("test_pipeline_integer_operations", 0);
}

int main() {
    int result = 0;
    result += test_pipeline_integer_operations();

    if (result == 0) {
        std::cout << "All tests passed!" << std::endl;
    } else {
        std::cout << result << " tests failed." << std::endl;
    }

    return result;
}
#pragma once

#include <StormByte/buffer/consumer.hxx>
#include <StormByte/buffer/producer.hxx>
#include <StormByte/buffer/typedefs.hxx>

/**
 * @namespace Buffer
 * @brief Namespace for buffer-related components in the StormByte library.
 *
 * The Buffer namespace provides classes and utilities for managing simple, shared, and producer/consumer
 * buffers in both single-threaded and multi-threaded environments. It supports a variety of use cases, including:
 * - **Simple Buffer**: Lightweight, non-thread-safe buffers for single-threaded environments.
 * - **Shared Buffer**: Flexible and efficient storage for byte data with concurrent access support.
 * - **Producer/Consumer Buffer**: Advanced models for managing data flow between producers and consumers
 *   with status tracking (e.g., `Ready`, `EoF`, `Error`).
 * - **Thread Safety**: Shared and producer/consumer buffers are designed to ensure consistent behavior in multi-threaded environments.
 */
namespace StormByte::Buffer {
    /**
     * @class Pipeline
     * @brief A class designed to execute a sequence of functions asynchronously in a pipeline.
     *
     * The `Pipeline` class allows users to define a chain of processing functions, where each function:
     * - Takes two parameters:
     *   1. A `Consumer` buffer to retrieve input data.
     *   2. A `Producer` buffer to store processed output data.
     * - Returns a `bool` indicating the success or failure of the operation.
     *
     * Functions are executed in the order they are added to the pipeline. Each function runs asynchronously in a detached thread, 
     * allowing all functions in the chain to execute concurrently. The output of one function is fed as the input to the next 
     * function in the chain. If any function in the chain returns `false`, the output buffer is marked with a status of `Status::Error`. 
     * It is the responsibility of each subsequent function in the chain to check the input buffer's status and return `false` 
     * if the status is `Status::Error`. This ensures that the error condition is propagated consistently until the end of the pipeline.
     *
     * **Buffer Status Handling:**
     * - There is no need to manually update the buffer status inside the function. The pipeline will handle this automatically:
     *   - A return value of `true` will set the output buffer's status to `Status::EoF`, signaling to the next function that the input has finished.
     *   - A return value of `false` will set the output buffer's status to `Status::Error`, ensuring that subsequent functions return `false` accordingly.
     *
     * **Thread Synchronization:**
     * - The pipeline relies on the `Async` buffer's shared ownership model to manage thread synchronization and lifetime.
     * - Each detached thread processes its assigned function and updates the buffer's status accordingly.
     * - The returned buffer from the `Process` function represents the final output of the pipeline. Its status indicates the state of the processing:
     *   - `Status::Ready`: Data is still being processed in at least one thread.
     *   - `Status::EoF`: All threads have completed successfully.
     *   - `Status::Error`: An error occurred in one of the threads, and processing was terminated.
     *
     * **Important Notes:**
     * - Functions must not throw exceptions.
     * - If any function does not return a `bool` or does not finish execution, the behavior of the pipeline is undefined.
     * - The lifetime of intermediate buffers is managed automatically through `std::shared_ptr`. Buffer are destroyed when no longer needed.
     */
    class STORMBYTE_PUBLIC Pipeline final {
        public:
            /**
             * @brief Default constructor
             * Initializes an empty pipeline buffer.
             */
            Pipeline() noexcept												= default;

            /**
             * @brief Copy constructor
             * Creates a new `Pipeline` that shares the same underlying buffer as the original.
             * @param other Pipeline to copy
             */
            Pipeline(const Pipeline& other) 								= default;

            /**
             * @brief Move constructor
             * Moves the contents of another `Pipeline` into this instance.
             * @param other Pipeline to move
             */
            Pipeline(Pipeline&& other) noexcept 							= default;

            /**
             * @brief Destructor
             */
            ~Pipeline() noexcept 											= default;

            /**
             * @brief Copy assignment operator
             * @param other `Pipeline` instance to copy from
             * @return Reference to the updated `Pipeline` instance
             */
            Pipeline& operator=(const Pipeline& other)						= default;

            /**
             * @brief Move assignment operator
             * @param other `Pipeline` instance to move from
             * @return Reference to the updated `Pipeline` instance
             */
            Pipeline& operator=(Pipeline&& other) noexcept					= default;

            /**
             * @brief Adds a new pipe function to the pipeline.
             * @param pipe Pipe function to add
             */
            void 															AddPipe(const PipeFunction& pipe);

            /**
             * @brief Moves a new pipe function to the pipeline.
             * @param pipe Pipe function to move
             */
            void 															AddPipe(PipeFunction&& pipe);

            /**
             * @brief Processes the pipeline buffer.
             * 
             * The provided buffer is passed through the chain of functions in the pipeline. Each function processes
             * the buffer and passes its output to the next function in the chain. All functions are executed asynchronously 
             * in detached threads, allowing concurrent processing of the pipeline stages.
             *
             * The returned buffer represents the final output of the pipeline. Its status indicates the state of the processing:
             * - `Status::Ready`: Data is still being processed in at least one thread.
             * - `Status::EoF`: All threads have completed successfully.
             * - `Status::Error`: An error occurred in one of the threads, and processing was terminated.
             *
             * **Execution Details:**
             * - Each function in the pipeline runs in a detached thread, and the `Async` buffer ensures proper synchronization and lifetime management.
             * - The pipeline automatically propagates errors by marking the buffer with `Status::Error` if a function fails.
             * - The returned buffer is the last buffer in the chain, ensuring that its status reflects the state of the entire pipeline.
             *
             * @param buffer Buffer to process
             * @return Processed buffer
             */
            Consumer														Process(Consumer buffer) const noexcept;

        private:
            std::vector<PipeFunction> m_pipes;								///< Vector of pipe functions
    };
}
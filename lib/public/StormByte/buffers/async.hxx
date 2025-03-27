#pragma once

#include <StormByte/buffers/shared.hxx>

#include <atomic>
#include <utility>

/**
 * @namespace Buffers
 * @brief Namespace for buffer-related components in the StormByte library.
 *
 * The `StormByte::Buffers` namespace provides classes and utilities for managing simple, shared, and producer/consumer
 * buffers in both single-threaded and multi-threaded environments. It supports a variety of use cases, including:
 * - **Simple Buffers**: Lightweight, non-thread-safe buffers for single-threaded environments.
 * - **Shared Buffers**: Flexible and efficient storage for byte data with concurrent access support.
 * - **Producer/Consumer Buffers**: Advanced models for managing data flow between producers and consumers
 *   with status tracking (e.g., `Ready`, `EoF`, `Error`).
 * - **Thread Safety**: Shared and producer/consumer buffers are designed to ensure consistent behavior in multi-threaded environments.
 */
namespace StormByte::Buffers {
    class Consumer; 																			///< Forward declaration of ConsumerBuffer.
    class Producer; 																			///< Forward declaration of ProducerBuffer.

    /**
     * @class Async
     * @brief Manages byte buffers in a multi-threaded producer/consumer model.
     *
     * The `Async` class enables safe multi-threaded read and write operations on byte buffers.
     * It uses `ConsumerBuffer` for reading data and `ProducerBuffer` for writing data. The `ProducerBuffer`
     * must update the buffer status to `EoF` (End of File) or `Error` to signal the end of operations or issues encountered.
     *
     * Proper handling of the buffer status is critical to avoid undesired behavior, such as infinite wait states
     * in the `ConsumerBuffer` after consuming all available data.
     *
     * Designed for multi-threaded environments, including detached threads, the class ensures safe resource management.
     * It is recommended to use `std::shared_ptr` when interacting with `Async`. The `operator<<` methods allow seamless
     * appending of data using instances of `Buffer` or `Buffer::DataType`.
     *
     * Note: Operations on a buffer with a status of `Error` are ignored, except for updating the buffer's status.
     */
    class STORMBYTE_PUBLIC Async {
        public:
            /**
             * @brief Constructs an empty `Async` instance.
             */
            Async() noexcept;

            /**
             * @brief Copy constructor.
             * @param other `Async` instance to copy from.
             */
            Async(const Async& other)															= default;

            /**
             * @brief Move constructor.
             * @param other `Async` instance to move from.
             */
            Async(Async&& other)																= default;

            /**
             * @brief Destructor.
             */
            virtual ~Async() noexcept 															= default;

            /**
             * @brief Copy assignment operator.
             * @param other `Async` instance to copy from.
             * @return Reference to the updated `Async` instance.
             */
            Async& operator=(const Async& other)												= default;

            /**
             * @brief Move assignment operator.
             * @param other `Async` instance to move from.
             * @return Reference to the updated `Async` instance.
             */
            Async& operator=(Async&& other) noexcept											= default;

            /**
             * @brief Sets the status of the buffer.
             * @param status The new `Status` to assign to the buffer.
             * @return Reference to the updated `Async` instance.
             */
            Async& 																				operator<<(const Buffers::Status& status);

            /**
             * @brief Retrieves a shared pointer to a consumer buffer linked to the current buffer.
             *
             * This method returns a `std::shared_ptr<Consumer>` that shares the same underlying buffer as the current `Async` instance.
             * The `Consumer` can be used to extract data from the buffer in a thread-safe manner.
             *
             * @return Shared pointer to a `Consumer` instance linked to the current buffer.
             */
            ConsumerPtr 																		Consumer() const noexcept;

            /**
             * @brief Retrieves a shared pointer to a producer buffer linked to the current buffer.
             *
             * This method returns a `ProducerPtr` that shares the same underlying buffer as the current `Async` instance.
             * The `Producer` can be used to append data to the buffer in a thread-safe manner.
             *
             * @return Shared pointer to a `Producer` instance linked to the current buffer.
             */
            ProducerPtr 																		Producer() const noexcept;

            /**
             * @brief Retrieves the size of the buffer.
             * @return Size of the buffer.
             */
            size_t 																				Size() const noexcept;

            /**
             * @brief Retrieves the current status of the buffer.
             * @return The current `Status` of the buffer.
             */
            Buffers::Status 																	Status() const noexcept;

            /**
             * @brief Retrieves the string representation of the buffer status.
             * @return String representation of the buffer status.
             */
            std::string 																		StatusToString() const noexcept;

        protected:
            using BufferWithStatus = std::pair<std::atomic<Buffers::Status>, Shared>; 			///< Buffer with status pair.
            using BufferWithStatusPtr = std::shared_ptr<BufferWithStatus>; 						///< Shared pointer to buffer with status.
            BufferWithStatusPtr 																m_buffer; 														///< Internal buffer and its status.
    };
}

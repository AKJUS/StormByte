#pragma once

#include <StormByte/buffers/shared.hxx>

#include <atomic>
#include <utility>

/**
 * @namespace Buffers
 * @brief Namespace for buffer-related components in the StormByte library.
 *
 * The `Buffers` namespace provides classes and utilities for managing simple, shared, and producer/consumer
 * buffers in a multi-threaded or single-threaded environment. It supports a variety of use cases, including
 * thread-safe operations and lightweight non-thread-safe buffers.
 *
 * Key features include:
 * - **Simple Buffers**: Lightweight, non-thread-safe buffers designed for single-threaded environments.
 * - **Shared Buffers**: Flexible and efficient storage for byte data with support for concurrent access.
 * - **Producer/Consumer Buffers**: Advanced models for managing data flow between producers and consumers
 *   with status tracking (e.g., `Ready`, `EoF`, `Error`).
 * - **Thread Safety**: Shared and producer/consumer buffers are designed to be thread-safe, enabling consistent
 *   behavior in multi-threaded environments.
 */
namespace StormByte::Buffers {
	class Consumer; 																			///< Forward declaration of ConsumerBuffer.
	class Producer; 																			///< Forward declaration of ProducerBuffer.

	/**
	 * @class Async
	 * @brief Manages byte buffers in a multi-threaded producer/consumer model.
	 *
	 * The `Async` class enables multiple threads to safely read and write data,
	 * where `ConsumerBuffer` is responsible for reading data and `ProducerBuffer` handles
	 * writing data. The `ProducerBuffer` must also update the buffer status to `EoF` (End of File)
	 * or `Error` to signal the end of operations or any issues encountered during processing.
	 *
	 * If the `ProducerBuffer` fails to update the buffer status, the `ConsumerBuffer` may enter
	 * an infinite wait state after consuming all available data. While this undesired behavior
	 * is expected in such cases, proper handling of the buffer status can prevent it.
	 *
	 * The class is designed to operate in a multi-threaded environment, including detached threads.
	 * To ensure safe resource management, it is recommended to work with `std::shared_ptr` when
	 * interacting with `Async`. The `operator<<` methods transparently accept instances
	 * of `Buffer` or `Buffer::DataType` to append data seamlessly.
	 *
	 * Note: Any operations performed on a buffer with a status of `Error` will be ignored,
	 * except for updating the buffer's status.
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

			/**
			 * @brief Sets the status of the buffer.
			 * @param status The new `Status` to assign to the buffer.
			 * @return Reference to the updated `Async` instance.
			 */
			Async& 																				operator<<(const Buffers::Status& status);

			/**
			 * @brief Creates a producer buffer linked to the current buffer.
			 * @return `ProducerBuffer` instance linked to the current buffer.
			 */
			std::shared_ptr<Producer>															Producer() const noexcept;

			/**
			 * @brief Creates a consumer buffer linked to the current buffer.
			 * @return `ConsumerBuffer` instance linked to the current buffer.
			 */
			std::shared_ptr<Consumer> 															Consumer() const noexcept;

		protected:
			using BufferWithStatus = std::pair<std::atomic<Buffers::Status>, Shared>; 			///< Buffer with status pair.
			using BufferWithStatusPtr = std::shared_ptr<BufferWithStatus>; 						///< Shared pointer to buffer with status.
			BufferWithStatusPtr m_buffer; 														///< Internal buffer and its status.
	};
}

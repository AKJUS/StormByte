#pragma once

#include <StormByte/async_buffer.hxx>

/**
 * @namespace StormByte
 * @brief Main namespace for the StormByte library and components
 */
namespace StormByte {
	/**
	 * @class ProducerBuffer
	 * @brief A thread-safe class for handling producer buffers with write-only operations.
	 *
	 * The `ProducerBuffer` class is designed for use in producer-consumer models where the producer
	 * writes data to a shared buffer. This class provides various `operator<<` overloads to append
	 * data from different sources, such as other buffers, strings, or byte vectors.
	 *
	 * **Key Features:**
	 * - Write-only operations: The `ProducerBuffer` allows appending data but does not support reading.
	 * - Thread-safe: All operations are thread-safe, ensuring consistent behavior in multi-threaded environments.
	 * - Shared buffer: Copies of a `ProducerBuffer` share the same underlying buffer, meaning changes made
	 *   to one instance will be reflected in all copies.
	 *
	 * This class is ideal for scenarios where multiple threads need to produce data into a shared buffer
	 * while maintaining thread safety and avoiding data corruption.
	 */
	class STORMBYTE_PUBLIC ProducerBuffer: public AsyncBuffer {
		friend class AsyncBuffer;
		public:
			/**
			 * @brief Copy constructor
			 * Creates a new `ProducerBuffer` that shares the same underlying buffer as the original.
			 * @param other Buffer to copy
			 * @note The new instance will share the same underlying buffer as `other`.
			 * Changes made to one instance will affect the other.
			 */
			ProducerBuffer(const ProducerBuffer& other)										= default;

			/**
			 * @brief Move constructor
			 * Moves the contents of another `ProducerBuffer` into this instance.
			 * @param other Buffer to move
			 */
			ProducerBuffer(ProducerBuffer&& other) noexcept									= default;

			/**
			 * @brief Destructor
			 * Cleans up the `ProducerBuffer` instance.
			 */
			virtual ~ProducerBuffer() noexcept												= default;

			/**
			 * @brief Copy assignment operator
			 * Copies the state of another `ProducerBuffer` into this instance.
			 * @param other Buffer to copy
			 * @return Reference to this buffer
			 * @note The new instance will share the same underlying buffer as `other`.
			 * Changes made to one instance will affect the other.
			 */
			ProducerBuffer& operator=(const ProducerBuffer& other)							= default;

			/**
			 * @brief Move assignment operator
			 * Moves the contents of another `ProducerBuffer` into this instance.
			 * @param other Buffer to move
			 * @return Reference to this buffer
			 */
			ProducerBuffer& operator=(ProducerBuffer&& other) noexcept 						= default;

			/**
			 * @brief Appends a buffer to the current buffer
			 * Transfers data from the specified `ConsumerBuffer` into this `ProducerBuffer`.
			 * @param buffer Buffer to append
			 * @return Reference to the updated `ProducerBuffer` instance
			 */
			ProducerBuffer& 																operator<<(ConsumerBuffer& buffer);

			/**
			 * @brief Appends a string to the current buffer
			 * Converts the provided string into bytes and appends it to the buffer.
			 * @param data String to append
			 * @return Reference to the updated `ProducerBuffer` instance
			 */
			ProducerBuffer&																	operator<<(const std::string& data);

			/**
			 * @brief Appends a byte vector to the current buffer
			 * Copies the provided byte vector into the buffer.
			 * @param data Byte vector to append
			 * @return Reference to the updated `ProducerBuffer` instance
			 */
			ProducerBuffer&																	operator<<(const Buffer::DataType& data);

			/**
			 * @brief Moves a byte vector and appends to the current buffer
			 * Transfers ownership of the provided byte vector into the buffer, avoiding a copy.
			 * @param data Byte vector to append
			 * @return Reference to the updated `ProducerBuffer` instance
			 */
			ProducerBuffer&																	operator<<(Buffer::DataType&& data);

			/**
			 * @brief Sets the status of the buffer.
			 * @param status The new `Status` to assign to the buffer.
			 * @return Reference to the updated `ProducerBuffer` instance.
			 */
			ProducerBuffer& 																operator<<(const enum Status& status);

			/**
			 * @brief Reserves buffer size
			 * Ensures the buffer has enough capacity for the specified size.
			 * @param size Length of the buffer to reserve
			 */
			void 																			Reserve(const std::size_t& size);

		private:
			/**
			 * @brief Default constructor
			 * Creates an empty `ProducerBuffer` instance.
			 */
			ProducerBuffer() noexcept														= default;
	};
}
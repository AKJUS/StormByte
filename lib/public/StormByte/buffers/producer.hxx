#pragma once

#include <StormByte/buffers/async.hxx>

/**
 * @namespace Buffers
 * @brief Namespace for buffer-related components in the StormByte library.
 *
 * The `StormByte::Buffers` namespace provides classes and utilities for managing various types of buffers,
 * including simple, shared, and producer/consumer buffers. These buffers are designed to support
 * both single-threaded and multi-threaded environments, offering flexibility and efficiency.
 */
namespace StormByte::Buffers {
	class Consumer; 																		///< Forward declaration of Consumer.
	
	/**
	 * @class Producer
	 * @brief A thread-safe class for handling producer buffers with write-only operations.
	 *
	 * The `Producer` class is designed for use in producer-consumer models where the producer
	 * writes data to a shared buffer. This class provides various `operator<<` overloads to append
	 * data from different sources, such as other buffers, strings, or byte vectors.
	 *
	 * **Key Features:**
	 * - Write-only operations: The `Producer` allows appending data but does not support reading.
	 * - Thread-safe: All operations are thread-safe, ensuring consistent behavior in multi-threaded environments.
	 * - Shared buffer: Copies of a `Producer` share the same underlying buffer, meaning changes made
	 *   to one instance will be reflected in all copies.
	 *
	 * This class is ideal for scenarios where multiple threads need to produce data into a shared buffer
	 * while maintaining thread safety and avoiding data corruption.
	 */
	class STORMBYTE_PUBLIC Producer final: public Async {
		public:
			/**
			 * @brief Default constructor
			 * Creates an empty `Producer` instance.
			 * @param async The async buffer to copy from
			 */
			explicit Producer(const Async& async) noexcept;

			/**
			 * @brief Copy constructor
			 * Creates a new `Producer` that shares the same underlying buffer as the original.
			 * @param other Buffer to copy
			 * @note The new instance will share the same underlying buffer as `other`.
			 * Changes made to one instance will affect the other.
			 */
			Producer(const Producer& other)													= default;

			/**
			 * @brief Move constructor
			 * Moves the contents of another `Producer` into this instance.
			 * @param other Buffer to move
			 */
			Producer(Producer&& other) noexcept												= default;

			/**
			 * @brief Destructor
			 * Cleans up the `Producer` instance.
			 */
			~Producer() noexcept override													= default;

			/**
			 * @brief Copy assignment operator
			 * Copies the state of another `Producer` into this instance.
			 * @param other Buffer to copy
			 * @return Reference to this buffer
			 * @note The new instance will share the same underlying buffer as `other`.
			 * Changes made to one instance will affect the other.
			 */
			Producer& operator=(const Producer& other)										= default;

			/**
			 * @brief Move assignment operator
			 * Moves the contents of another `Producer` into this instance.
			 * @param other Buffer to move
			 * @return Reference to this buffer
			 */
			Producer& operator=(Producer&& other) noexcept 									= default;

			/**
			 * @brief Appends a buffer to the current buffer
			 * Transfers data from the specified `Simple` buffer into this `Producer`.
			 * @param buffer Buffer to append
			 * @return Reference to the updated `Producer` instance
			 */
			Producer& 																		operator<<(const Simple& buffer);

			/**
			 * @brief Appends a buffer (moving its contents) to the current buffer
			 * Transfers data from the specified `Simple` buffer into this `Producer`.
			 * @param buffer Buffer to append
			 * @return Reference to the updated `Producer` instance
			 */
			Producer& 																		operator<<(Simple&& buffer);

			/**
			 * @brief Appends a string to the current buffer
			 * Converts the provided string into bytes and appends it to the buffer.
			 * @param data String to append
			 * @return Reference to the updated `Producer` instance
			 */
			Producer&																		operator<<(const std::string& data);

			/**
			 * @brief Appends a byte vector to the current buffer
			 * Copies the provided byte vector into the buffer.
			 * @param data Byte vector to append
			 * @return Reference to the updated `Producer` instance
			 */
			Producer&																		operator<<(const Buffers::Data& data);

			/**
			 * @brief Moves a byte vector and appends to the current buffer
			 * Transfers ownership of the provided byte vector into the buffer, avoiding a copy.
			 * @param data Byte vector to append
			 * @return Reference to the updated `Producer` instance
			 */
			Producer&																		operator<<(Buffers::Data&& data);

			/**
			 * @brief Sets the status of the buffer.
			 * @param status The new `Status` to assign to the buffer.
			 * @return Reference to the updated `Producer` instance.
			 */
			Producer& 																		operator<<(const Buffers::Status& status);

			/**
			 * @brief Reserves buffer size
			 * Ensures the buffer has enough capacity for the specified size.
			 * @param size Length of the buffer to reserve
			 */
			void 																			Reserve(const std::size_t& size);
	};
}
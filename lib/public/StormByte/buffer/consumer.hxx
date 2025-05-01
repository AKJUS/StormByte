#pragma once

#include <StormByte/buffer/shared.hxx>
#include <memory>

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
	class Producer; // Forward declaration of the Producer class

	/**
	 * @class Consumer
	 * @brief A read-only interface for accessing a shared buffer.
	 *
	 * The `Consumer` class provides a thread-safe, read-only interface for accessing data in a shared buffer.
	 * Instances of `Consumer` cannot be created directly; they are created and managed by their `Producer` counterpart.
	 *
	 * **Key Features:**
	 * - **Read-Only Access**: Allows only read operations on the buffer.
	 * - **Non-Copyable**: Instances of `Consumer` cannot be copied to ensure data integrity.
	 * - **Thread-Safe**: All operations are thread-safe and synchronized using the underlying `Shared` buffer.
	 * - **Shared Buffer Behavior**: While the class is designed to have only one producer and one consumer instance,
	 *   if `Consumer` instances are copied, they will share the same buffer. This is allowed, but it is up to the user
	 *   to ensure that the produced-consumed data flow remains in sync.
	 */
	class STORMBYTE_PUBLIC Consumer final {
		friend class Producer;

		public:
			/**
			 * @brief Default constructor
			 * 
			 * Creates an unconnected `Consumer` instance.
			 */
			Consumer() noexcept;

			/**
			 * @brief Deleted copy constructor
			 * 
			 * The `Consumer` class cannot be copied to ensure data integrity and prevent unintended sharing of the buffer.
			 */
			Consumer(const Consumer& other) 							= default;

			/**
			 * @brief Default move constructor
			 * 
			 * Allows moving a `Consumer` instance. The moved-from instance is left in a valid but unspecified state.
			 */
			Consumer(Consumer&& other) noexcept 						= default;

			/**
			 * @brief Destructor
			 * 
			 * Cleans up the `Consumer` instance. The destructor ensures that any resources held by the `Consumer` are properly released.
			 */
			~Consumer() noexcept = default;

			/**
			 * @brief Deleted copy assignment operator
			 * 
			 * The `Consumer` class cannot be copied to ensure data integrity and prevent unintended sharing of the buffer.
			 */
			Consumer& operator=(const Consumer& other) 					= default;

			/**
			 * @brief Default move assignment operator
			 * 
			 * Allows moving a `Consumer` instance. The moved-from instance is left in a valid but unspecified state.
			 */
			Consumer& operator=(Consumer&& other) noexcept 				= default;

			/**
			 * @brief Retrieves the number of bytes available for reading.
			 * @return The number of bytes available for reading.
			 */
			std::size_t 												AvailableBytes() const noexcept;

			/**
			 * @brief Retrieves a copy of the buffer data.
			 * @return A copy of the buffer data.
			 */
			Buffer::Data 												Data() const noexcept;

			/**
			 * @brief Discards data from the buffer.
			 * @param length The number of bytes to discard.
			 * @param mode The read position mode.
			 */
			void 														Discard(const std::size_t& length, const Read::Position& mode) noexcept;

			/**
			 * @brief Checks if the read position is at the end.
			 * @return True if the read position is at the end, false otherwise.
			 */
			bool 														End() const noexcept;

			/**
			 * @brief Checks if the shared buffer is empty.
			 * @return True if the buffer is empty, false otherwise.
			 */
			bool 														Empty() const noexcept;

			/**
			 * @brief Extracts a specific size of data, taking ownership of the read data and removing it from the shared buffer.
			 * @param length The number of bytes to extract.
			 * @return The extracted data.
			 */
			ExpectedData<BufferOverflow> 								Extract(const size_t& length);

			/**
			 * @brief Extracts a specific size of data and moves it directly into the provided buffer.
			 * @param length The number of bytes to extract.
			 * @param output The buffer where the extracted data will be moved.
			 * @return The status of the operation.
			 */
			Read::Status 												ExtractInto(const size_t& length, Shared& output) noexcept;

			/**
			 * @brief Checks if the shared buffer has enough data starting from the current read position.
			 * @param length The number of bytes to check.
			 * @return True if there is enough data, false otherwise.
			 */
			bool 														HasEnoughData(const std::size_t& length) const;

			/**
			 * @brief Retrieves the stored value as a hexadecimal string.
			 * @param column_size The number of bytes per column in the output.
			 * @return The hexadecimal representation of the buffer data.
			 */
			std::string 												HexData(const std::size_t& column_size = 16) const;

			/**
			 * @brief Checks if the buffer is at the end of the file/data.
			 * @return True if the buffer is at the end, false otherwise.
			 */
			bool 														IsEoF() const noexcept;

			/**
			 * @brief Checks if the buffer is readable.
			 * @return True if the buffer is readable, false otherwise.
			 */
			bool 														IsReadable() const noexcept;

			/**
			 * @brief Peeks at the next byte in the shared buffer without advancing the read position.
			 * @return The next byte in the buffer.
			 */
			ExpectedByte<BufferOverflow> 								Peek() const;

			/**
			 * @brief Retrieves the read position.
			 * @return The current read position.
			 */
			std::size_t 												Position() const noexcept;

			/**
			 * @brief Reads a specific size of data starting from the current read position.
			 * @param length The number of bytes to read.
			 * @return The read data.
			 */
			ExpectedData<BufferOverflow> 								Read(const size_t& length) const;

			/**
			 * @brief Moves the read pointer within the shared buffer based on the specified position and mode.
			 * @param position The position to move to.
			 * @param mode The read position mode.
			 */
			void 														Seek(const std::ptrdiff_t& position, const Read::Position& mode) const;

			/**
			 * @brief Retrieves the length of the shared buffer.
			 * @return The length of the buffer.
			 */
			std::size_t 												Size() const noexcept;

			/**
			 * @brief Gets the buffer status.
			 * @return The current status of the buffer.
			 */
			enum Status 												Status() const noexcept;

			/**
			 * @brief Creates a producer instance for this consumer.
			 * @return A `Producer` instance bound to this consumer's buffer.
			 */
			Producer 													Producer() const noexcept;

		private:
			std::shared_ptr<Shared> m_shared; 							///< The shared buffer instance.

			/**
			 * @brief Constructor that initializes the consumer with a shared buffer.
			 * @param shared A shared pointer to the shared buffer.
			 */
			explicit Consumer(std::shared_ptr<Shared> shared) noexcept;

			
	};
}
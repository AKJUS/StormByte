#pragma once

#include <StormByte/buffers/simple.hxx>

#include <cstddef>
#include <memory>
#include <mutex>
#include <shared_mutex>
#include <string>

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
	/**
	 * @class Shared
	 * @brief The class to store and manipulate shared byte buffers.
	 *
	 * The `Shared` class provides a flexible and efficient way to store and manipulate byte data.
	 * It supports various operations such as appending, extracting, reading, and seeking within the buffer.
	 *
	 * **Key Features:**
	 * - **Thread-Safe**: The class ensures thread safety using a `std::shared_mutex`. Reads are non-blocking
	 *   and allow concurrent access, while writes are exclusive and block other operations.
	 * - **Flexible Buffers::Data Management**: Supports appending data from various sources (e.g., strings, vectors, spans),
	 *   extracting data destructively, and reading data non-destructively.
	 * - **Position Management**: Provides methods to manage the read position, including seeking and resetting.
	 *
	 * This class is ideal for scenarios where multiple threads need to read from or write to a shared buffer
	 * while maintaining data integrity and avoiding race conditions.
	 */
	class STORMBYTE_PUBLIC Shared final : public Simple {
		public:
			/**
			 * @brief Default constructor
			 * Initializes an empty shared buffer.
			 */
			Shared() noexcept;

			/**
			 * @brief Constructor reserving initial size
			 * @param size Size of the buffer to reserve.
			 */
			explicit Shared(const std::size_t& size);

			/**
			 * @brief Constructor
			 * @param data Pointer to the data to set.
			 * @param length Length of the data.
			 */
			Shared(const char* data, const std::size_t& length);

			/**
			 * @brief Constructor
			 * @param data String to set as buffer content.
			 */
			Shared(const std::string& data);

			/**
			 * @brief Constructor
			 * @param data Vector of bytes to set as buffer content.
			 */
			Shared(const Buffers::Data& data);

			/**
			 * @brief Constructor
			 * @param data Vector of bytes to move into the buffer.
			 */
			Shared(Buffers::Data&& data);

			/**
			 * @brief Constructor
			 * @param data Span of bytes to set as buffer content.
			 */
			Shared(const std::span<const Byte>& data);

			/**
			 * @brief Copy constructor
			 * @param other Shared buffer to copy from.
			 */
			Shared(const Shared& other);

			/**
			 * @brief Move constructor
			 * @param other Shared buffer to move from.
			 */
			Shared(Shared&& other) noexcept;

			/**
			 * @brief Destructor
			 * Cleans up the shared buffer.
			 */
			~Shared() noexcept = default;

			/**
			 * @brief Copy assignment operator
			 * @param other Shared buffer to copy from.
			 * @return Reference to the updated shared buffer.
			 */
			Shared& 															operator=(const Shared& other);

			/**
			 * @brief Move assignment operator
			 * @param other Shared buffer to move from.
			 * @return Reference to the updated shared buffer.
			 */
			Shared& 															operator=(Shared&& other) noexcept;

			/**
			 * @brief Equality operator
			 * @param other Shared buffer to compare.
			 * @return True if the shared buffers are equal, false otherwise.
			 */
			bool 																operator==(const Shared& other) const;

			/**
			 * @brief Inequality operator
			 * @param other Shared buffer to compare.
			 * @return True if the shared buffers are not equal, false otherwise.
			 */
			bool 																operator!=(const Shared& other) const;

			/**
			 * @brief Appends a buffer to the current buffer
			 * @param buffer Shared buffer to append.
			 * @return Reference to the updated simple buffer.
			 */
			Shared& 															operator<<(const Simple& buffer);

			/**
			 * @brief Moves a buffer and appends to the current buffer
			 * @param buffer Shared buffer to append.
			 * @return Reference to the updated buffer.
			 */
			Shared& 															operator<<(Simple&& buffer);

			/**
			 * @brief Appends a string to the current simple buffer
			 * @param data String to append.
			 * @return Reference to the updated buffer.
			 */
			Shared& 															operator<<(const std::string& data);

			/**
			 * @brief Appends a byte vector to the current simple buffer
			 * @param data Byte vector to append.
			 * @return Reference to the updated simple buffer.
			 */
			Shared& 															operator<<(const Buffers::Data& data);

			/**
			 * @brief Moves a byte vector and appends to the current simple buffer
			 * @param data Byte vector to append.
			 * @return Reference to the updated simple buffer.
			 */
			Shared& 															operator<<(Buffers::Data&& data);

			/**
			 * @brief Appends current simple buffer to target simple buffer
			 * @param buffer Target simple buffer to append to.
			 * @return Reference to the updated simple buffer.
			 */
			Shared& 															operator>>(Shared& buffer);

			/**
			 * @brief Clears the shared buffer
			 * Removes all data and resets the read position.
			 */
			void 																Clear() noexcept override;

			/**
			 * @brief Retrieves the stored value
			 * @return Span of the stored value.
			 */
			Buffers::Data 														Data() const noexcept override;

			/**
			 * @brief Deleted function to prevent usage of Span in Shared.
			 * 
			 * The `Span()` function is deleted in the `Shared` class to
			 * ensure thread safety. Returning a `std::span` provides a
			 * lightweight view into the internal buffer, but it does not
			 * own the data. In a multi-threaded environment, concurrent
			 * modifications to the buffer could invalidate the span,
			 * leading to undefined behavior or memory access issues.
			 * 
			 * Use the `Data()` method instead to retrieve a thread-safe
			 * copy of the buffer's contents.
			 */
			std::span<Byte> 													Span() noexcept = delete;

			/**
			 * @brief Deleted function to prevent usage of Span in Shared.
			 * 
			 * The `Span()` function is deleted in the `Shared` class to
			 * ensure thread safety. Returning a `std::span` provides a
			 * lightweight view into the internal buffer, but it does not
			 * own the data. In a multi-threaded environment, concurrent
			 * modifications to the buffer could invalidate the span,
			 * leading to undefined behavior or memory access issues.
			 * 
			 * Use the `Data()` method instead to retrieve a thread-safe
			 * copy of the buffer's contents.
			 */
			const std::span<const Byte> 										Span() const noexcept = delete;

			/**
			 * @brief Retrieves the stored value as a hexadecimal string
			 * @param column_size Number of bytes per column in the output.
			 * @return Hexadecimal string representation of the stored value.
			 */
			std::string 														HexData(const std::size_t& column_size = 16) const;

			/**
			 * @brief Checks if the shared buffer has enough data starting from the current read position.
			 * @param length Length of the data to check.
			 * @return True if the shared buffer has enough data starting from the current position, false otherwise.
			 */
			bool 																HasEnoughData(const std::size_t& length) const;

			/**
			 * @brief Retrieves the read position
			 * @return Current read position in the shared buffer.
			 */
			std::size_t 														Position() const noexcept override;

			/**
			 * @brief Gets a shared buffer of a specific length since current read position
			 * @param length Length of the shared buffer to read.
			 * @return Span of the requested length or a BufferOverflow error.
			 */
			ExpectedConstByteSpan<BufferOverflow> 								Read(const size_t& length) const;

			/**
			 * @brief Extracts a specific size of data, taking ownership of the read data and removing it from the shared buffer.
			 * @param length Length of the data to read and remove from the shared buffer.
			 * @return `ExpectedBuffers::DataType` containing the requested data, or an `Unexpected` with a `BufferOverflow` error if
			 *         insufficient data exists.
			 */
			ExpectedData<BufferOverflow> 										Extract(const size_t& length);

			/**
			 * @brief Reserves shared buffer size
			 * Ensures the shared buffer has enough capacity for the specified size.
			 * @param size Size to reserve.
			 */
			void 																Reserve(const std::size_t& size);

			/**
			 * @brief Moves the read pointer within the shared buffer based on the specified position and mode.
			 * @param position The position to move to, interpreted based on the specified mode.
			 * @param mode The mode to use for seeking (e.g., `Begin`, `End`, `Relative`, `Absolute`).
			 */
			void 																Seek(const std::ptrdiff_t& position, const Read::Position& mode);

			/**
			 * @brief Retrieves the length of the shared buffer
			 * @return Length of the shared buffer.
			 */
			std::size_t 														Size() const noexcept;

			/**
			 * @brief Checks if shared buffer is empty
			 * @return True if the shared buffer is empty, false otherwise.
			 */
			bool 																Empty() const noexcept;

			/**
			 * @brief Checks if the read position is at the end
			 * @return True if the read position is at the end, false otherwise.
			 */
			bool 																End() const noexcept;

		private:
			mutable std::shared_mutex m_data_mutex; 							///< Mutex for thread safety.
	};
}
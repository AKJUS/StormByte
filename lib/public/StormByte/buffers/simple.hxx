#pragma once

#include <StormByte/buffers/exception.hxx>
#include <StormByte/buffers/typedefs.hxx>
#include <StormByte/visibility.h>

/**
 * @namespace Buffers
 * @brief Namespace for buffer-related components in the StormByte library.
 *
 * The `StormByte::Buffers` namespace provides classes and utilities for managing various types of buffers,
 * including simple, shared, and producer/consumer buffers. These buffers are designed to support
 * both single-threaded and multi-threaded environments, offering flexibility and efficiency.
 */
namespace StormByte::Buffers {
	/**
	 * @class Simple
	 * @brief The class to store and manipulate simple byte buffers.
	 *
	 * The `Simple` class provides a lightweight and efficient way to store and manipulate byte data.
	 * It supports various operations such as appending, extracting, reading, and seeking within the buffer.
	 *
	 * **Key Features:**
	 * - **Lightweight**: Designed for single-threaded environments with minimal overhead.
	 * - **Flexible Data Management**: Supports appending data from various sources (e.g., strings, vectors, spans),
	 *   extracting data destructively, and reading data non-destructively.
	 * - **Position Management**: Provides methods to manage the read position, including seeking and resetting.
	 *
	 * This class is ideal for scenarios where performance is critical, and thread safety is not required.
	 */
	class STORMBYTE_PUBLIC Simple {
		public:
			/**
			 * @brief Default constructor
			 * Initializes an empty simple buffer.
			 */
			Simple() noexcept;

			/**
			 * @brief Constructor reserving initial size
			 * @param size Size of the buffer to reserve.
			 */
			explicit Simple(const std::size_t& size);

			/**
			 * @brief Constructor
			 * @param data Pointer to the data to set.
			 * @param length Length of the data.
			 */
			Simple(const char* data, const std::size_t& length);

			/**
			 * @brief Constructor
			 * @param data String to set as buffer content.
			 */
			Simple(const std::string& data);

			/**
			 * @brief Constructor
			 * @param data Vector of bytes to set as buffer content.
			 */
			Simple(const Data& data);

			/**
			 * @brief Constructor
			 * @param data Vector of bytes to move into the buffer.
			 */
			Simple(Data&& data);

			/**
			 * @brief Constructor
			 * @param data Span of bytes to set as buffer content.
			 */
			Simple(const std::span<const Byte>& data);

			/**
			 * @brief Copy constructor
			 * @param other Simple buffer to copy from.
			 */
			Simple(const Simple& other);

			/**
			 * @brief Move constructor
			 * @param other Simple buffer to move from.
			 */
			Simple(Simple&& other) noexcept;

			/**
			 * @brief Destructor
			 * Cleans up the simple buffer.
			 */
			virtual ~Simple() noexcept 												= default;

			/**
			 * @brief Copy assignment operator
			 * @param other Simple buffer to copy from.
			 * @return Reference to the updated simple buffer.
			 */
			Simple& operator=(const Simple& other);

			/**
			 * @brief Move assignment operator
			 * @param other Simple buffer to move from.
			 * @return Reference to the updated simple buffer.
			 */
			Simple& operator=(Simple&& other) noexcept;

			/**
			 * @brief Checks if two simple buffers are equal
			 * @param other Simple buffer to compare with.
			 * @return True if simple buffers are equal, false otherwise.
			 */
			virtual bool 															operator==(const Simple& other) const;

			/**
			 * @brief Checks if two simple buffers are not equal
			 * @param other Simple buffer to compare with.
			 * @return True if simple buffers are not equal, false otherwise.
			 */
			virtual bool 															operator!=(const Simple& other) const;

			/**
			 * @brief Appends a simple buffer to the current simple buffer
			 * @param buffer Simple buffer to append.
			 * @return Reference to the updated simple buffer.
			 */
			virtual Simple& 														operator<<(const Simple& buffer);

			/**
			 * @brief Moves a simple buffer and appends to the current simple buffer
			 * @param buffer Simple buffer to append.
			 * @return Reference to the updated simple buffer.
			 */
			virtual Simple& 														operator<<(Simple&& buffer);

			/**
			 * @brief Appends a string to the current simple buffer
			 * @param data String to append.
			 * @return Reference to the updated simple buffer.
			 */
			virtual Simple& 														operator<<(const std::string& data);

			/**
			 * @brief Appends a byte vector to the current simple buffer
			 * @param data Byte vector to append.
			 * @return Reference to the updated simple buffer.
			 */
			virtual Simple& 														operator<<(const Data& data);

			/**
			 * @brief Moves a byte vector and appends to the current simple buffer
			 * @param data Byte vector to append.
			 * @return Reference to the updated simple buffer.
			 */
			virtual Simple& 														operator<<(Data&& data);

			/**
			 * @brief Appends current simple buffer to target simple buffer
			 * @param buffer Target simple buffer to append to.
			 * @return Reference to the updated simple buffer.
			 */
			virtual Simple& 														operator>>(Simple& buffer);

			/**
			 * @brief Clears the simple buffer
			 * Removes all data and resets the read position.
			 */
			virtual void 															Clear();

			/**
			 * @brief Retrieves a copy of the buffer data.
			 * 
			 * This method returns a copy of the data stored in the buffer.
			 * It is suitable for scenarios where the caller needs to work
			 * with an independent copy of the buffer's contents.
			 * 
			 * @return A copy of the buffer data as a vector of bytes.
			 */
			virtual Buffers::Data 													Data() const noexcept;

			/**
			 * @brief Retrieves a const view (span) to the stored value.
			 * 
			 * This method provides a lightweight, read-only view of the
			 * buffer's contents without copying the data. The returned
			 * span remains valid as long as the buffer is not modified.
			 * 
			 * @return A read-only span of the stored value.
			 * @note Modifying the buffer invalidates the returned span.
			 */
			const std::span<const Byte> 											Span() const noexcept;

			/**
			 * @brief Retrieves a view (span) to the stored value.
			 * 
			 * This method provides a lightweight, mutable view of the
			 * buffer's contents without copying the data. The returned
			 * span remains valid as long as the buffer is not modified.
			 * 
			 * @return A mutable span of the stored value.
			 * @note Modifying the buffer invalidates the returned span.
			 */
			std::span<Byte> 														Span() noexcept;

			/**
			 * @brief Retrieves the stored value as a hexadecimal string.
			 *
			 * Converts the buffer's contents into a human-readable hexadecimal string representation.
			 * This is useful for debugging or logging purposes.
			 *
			 * @param column_size Number of bytes per column in the output. Defaults to 16.
			 * @return Hexadecimal string representation of the stored value.
			 */
			virtual std::string 													HexData(const std::size_t& column_size = 16) const;

			/**
			 * @brief Checks if the simple buffer has enough data starting from the current read position.
			 * @param length Length of the data to check.
			 * @return True if the simple buffer has enough data starting from the current position, false otherwise.
			 */
			virtual bool 															HasEnoughData(const std::size_t& length) const;

			/**
			 * @brief Retrieves the read position
			 * @return Current read position in the simple buffer.
			 */
			virtual std::size_t 													Position() const noexcept;

			/**
			 * @brief Gets a simple buffer of a specific length since current read position
			 * @param length Length of the simple buffer to read.
			 * @return Span of the requested length or a BufferOverflow error.
			 */
			virtual ExpectedConstByteSpan<BufferOverflow> 							Read(const size_t& length) const;

			/**
			 * @brief Extracts a specific size of data, taking ownership of the read data and removing it from the simple buffer.
			 * @param length Length of the data to read and remove from the simple buffer.
			 * @return `ExpectedDataType` containing the requested data, or an `Unexpected` with a `BufferOverflow` error if
			 *         insufficient data exists.
			 */
			virtual ExpectedData<BufferOverflow> 									Extract(const size_t& length);

			/**
			 * @brief Reserves simple buffer size
			 * Ensures the simple buffer has enough capacity for the specified size.
			 * @param size Size to reserve.
			 */
			virtual void 															Reserve(const std::size_t& size);

			/**
			 * @brief Moves the read pointer within the simple buffer based on the specified position and mode.
			 * @param position The position to move to, interpreted based on the specified mode.
			 * @param mode The mode to use for seeking (e.g., `Begin`, `End`, `Relative`, `Absolute`).
			 */
			virtual void 															Seek(const std::ptrdiff_t& position, const Read::Position& mode) const;

			/**
			 * @brief Retrieves the length of the simple buffer
			 * @return Length of the simple buffer.
			 */
			virtual std::size_t 													Size() const noexcept;

			/**
			 * @brief Checks if simple buffer is empty
			 * @return True if the simple buffer is empty, false otherwise.
			 */
			virtual bool 															Empty() const noexcept;

			/**
			 * @brief Checks if the read position is at the end
			 * @return True if the read position is at the end, false otherwise.
			 */
			virtual bool 															End() const noexcept;

		private:
			std::vector<std::byte> m_data; 											///< Stored value.
			mutable std::size_t m_position;											///< Read position.
	};
}
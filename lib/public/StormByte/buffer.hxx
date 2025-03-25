#pragma once

#include <StormByte/expected.hxx>
#include <StormByte/exception.hxx>
#include <StormByte/helpers.hxx>

#include <atomic>
#include <cstddef>
#include <memory>
#include <mutex>
#include <shared_mutex>
#include <string>
#include <span>
#include <type_traits>
#include <vector>

/**
 * @namespace StormByte
 * @brief Main namespace for the StormByte library and components
 */
namespace StormByte {
	/**
	 * @class Buffer
	 * @brief The class to store and manipulate byte buffers.
	 *
	 * The `Buffer` class provides a flexible and efficient way to store and manipulate byte data.
	 * It supports various operations such as appending, extracting, reading, and seeking within the buffer.
	 *
	 * **Key Features:**
	 * - **Thread-Safe**: The class ensures thread safety using a `std::shared_mutex`. Reads are non-blocking
	 *   and allow concurrent access, while writes are exclusive and block other operations.
	 * - **Flexible Data Management**: Supports appending data from various sources (e.g., strings, vectors, spans),
	 *   extracting data destructively, and reading data non-destructively.
	 * - **Position Management**: Provides methods to manage the read position, including seeking and resetting.
	 *
	 * This class is ideal for scenarios where multiple threads need to read from or write to a shared buffer
	 * while maintaining data integrity and avoiding race conditions.
	 */
	class STORMBYTE_PUBLIC Buffer {
		public:
			using Byte 		= std::byte;																///< The type of the byte.
			using DataType 	= std::vector<Byte>;														///< The type of the data stored in the buffer.
			using ExpectedConstByte = StormByte::Expected<const Byte&, BufferOverflow>;					///< The type of the expected byte.
			using ExpectedConstByteSpan = StormByte::Expected<std::span<const Byte>, BufferOverflow>;	///< The type of the expected byte span.
			using ExpectedDataType = StormByte::Expected<DataType, BufferOverflow>;						///< The type of the expected data.

			/**
			 * @enum ReadPosition
			 * @brief The enumeration to define the read position.
			 */
			enum class ReadPosition: unsigned short {
				Begin 		= 0x01,																		///< The read position is at the beginning of the buffer.
				End			= 0x02,																		///< The read position is at the end of the buffer.
				Relative	= 0x04,																		///< The read position is relative to the current position.
				Absolute	= 0x08																		///< The read position is absolute.
			};

			/**
			 * @brief Default constructor
			 * Initializes an empty buffer.
			 */
			Buffer() noexcept;

			/**
			 * @brief Constructor reserving initial size
			 * @param size Size of the buffer to reserve.
			 */
			explicit Buffer(const std::size_t& size);

			/**
			 * @brief Constructor
			 * @param data Pointer to the data to set.
			 * @param length Length of the data.
			 */
			Buffer(const char* data, const std::size_t& length);

			/**
			 * @brief Constructor
			 * @param data String to set as buffer content.
			 */
			Buffer(const std::string& data);

			/**
			 * @brief Constructor
			 * @param data Vector of bytes to set as buffer content.
			 */
			Buffer(const DataType& data);

			/**
			 * @brief Constructor
			 * @param data Vector of bytes to move into the buffer.
			 */
			Buffer(DataType&& data);

			/**
			 * @brief Constructor
			 * @param data Span of bytes to set as buffer content.
			 */
			Buffer(const std::span<const Byte>& data);

			/**
			 * @brief Copy constructor
			 * @param other Buffer to copy from.
			 */
			Buffer(const Buffer& other);

			/**
			 * @brief Move constructor
			 * @param other Buffer to move from.
			 */
			Buffer(Buffer&& other) noexcept;

			/**
			 * @brief Copy assignment operator
			 * @param other Buffer to copy from.
			 * @return Reference to the updated buffer.
			 */
			Buffer& operator=(const Buffer& other);

			/**
			 * @brief Move assignment operator
			 * @param other Buffer to move from.
			 * @return Reference to the updated buffer.
			 */
			Buffer& operator=(Buffer&& other) noexcept;

			/**
			 * @brief Destructor
			 * Cleans up the buffer.
			 */
			virtual ~Buffer() noexcept											= default;

			/**
			 * @brief Checks if two buffers are equal
			 * @param other Buffer to compare with.
			 * @return True if buffers are equal, false otherwise.
			 */
			bool 																operator==(const Buffer& other) const;

			/**
			 * @brief Checks if two buffers are not equal
			 * @param other Buffer to compare with.
			 * @return True if buffers are not equal, false otherwise.
			 */
			bool 																operator!=(const Buffer& other) const;

			/**
			 * @brief Gets a buffer character at a specific index
			 * @param index Index of the character.
			 * @return Expected byte at the specified index or a BufferOverflow error.
			 */
			ExpectedConstByte													operator[](const std::size_t& index) const;

			/**
			 * @brief Appends a buffer to the current buffer
			 * @param buffer Buffer to append.
			 * @return Reference to the updated buffer.
			 */
			Buffer&																operator<<(const Buffer& buffer);

			/**
			 * @brief Moves a buffer and appends to the current buffer
			 * @param buffer Buffer to append.
			 * @return Reference to the updated buffer.
			 */
			Buffer&																operator<<(Buffer&& buffer);

			/**
			 * @brief Appends a string to the current buffer
			 * @param data String to append.
			 * @return Reference to the updated buffer.
			 */
			Buffer&																operator<<(const std::string& data);

			/**
			 * @brief Appends a byte vector to the current buffer
			 * @param data Byte vector to append.
			 * @return Reference to the updated buffer.
			 */
			Buffer&																operator<<(const DataType& data);

			/**
			 * @brief Moves a byte vector and appends to the current buffer
			 * @param data Byte vector to append.
			 * @return Reference to the updated buffer.
			 */
			Buffer&																operator<<(DataType&& data);

			/**
			 * @brief Appends current buffer to target buffer
			 * @param buffer Target buffer to append to.
			 * @return Reference to the updated buffer.
			 */
			Buffer&																operator>>(Buffer& buffer);

			/**
			 * @brief Clears the buffer
			 * Removes all data and resets the read position.
			 */
			void 																Clear();

			/**
			 * @brief Retrieves the stored value
			 * @return Span of the stored value.
			 *
			 * @note The returned `std::span` is a view into the buffer's internal data. It is up to the user
			 *       to ensure that the buffer's lifetime is managed properly so the span does not become invalid.
			 */
			const std::span<const Byte>											Data() const noexcept;

			/**
			 * @brief Retrieves the stored value
			 * @return Mutable span of the stored value.
			 *
			 * @note The returned `std::span` is a view into the buffer's internal data. It is up to the user
			 *       to ensure that the buffer's lifetime is managed properly so the span does not become invalid.
			 */
			std::span<Byte>														Data() noexcept;

			/**
			 * @brief Retrieves the stored value as a hexadecimal string
			 * @param column_size Number of bytes per column in the output.
			 * @return Hexadecimal string representation of the stored value.
			 */
			std::string 														HexData(const std::size_t& column_size = 16) const;

			/**
			 * @brief Checks if the buffer has enough data starting from the current read position.
			 *
			 * This function determines whether the buffer contains at least the specified amount of data
			 * starting from the current read position (`Position()`). It calculates the required data
			 * as `Position() + length` and compares it with the total size of the buffer (`Size()`).
			 *
			 * @param length Length of the data to check.
			 * @return True if the buffer has enough data starting from the current position, false otherwise.
			 *
			 * @note Thread-safe: This function ensures thread safety by locking the buffer during the operation.
			 */
			bool 																HasEnoughData(const std::size_t& length) const;

			/**
			 * @brief Retrieves the read position
			 * @return Current read position in the buffer.
			 */
			inline std::size_t 													Position() const {
				return m_position.load();
			}

			/**
			 * @brief Gets a buffer of a specific length since current read position
			 * @param length Length of the buffer to read.
			 * @return Span of the requested length or a BufferOverflow error.
			 */
			ExpectedConstByteSpan												Read(const size_t& length) const;

			/**
			 * @brief Extracts a specific size of data, taking ownership of the read data and removing it from the buffer.
			 *
			 * This function extracts the requested number of bytes from the buffer starting at the current read position.
			 * The extracted data is returned as a new `DataType` (i.e., `std::vector<std::byte>`), and the corresponding
			 * bytes are removed from the buffer. The read position (`m_position`) is not modified because the data before
			 * the current position is removed, leaving the position pointing to the same logical location in the updated buffer.
			 *
			 * If the requested length exceeds the available data in the buffer, an `Unexpected` with a `BufferOverflow` error
			 * is returned.
			 *
			 * @param length Length of the data to read and remove from the buffer.
			 * @return `ExpectedDataType` containing the requested data, or an `Unexpected` with a `BufferOverflow` error if
			 *         insufficient data exists.
			 *
			 * @note This operation modifies the buffer by removing the extracted data. It is destructive and should be
			 *       used with care in scenarios where the buffer's integrity must be preserved.
			 * @note Thread-safe: This function ensures thread safety by locking the buffer during the operation.
			 */
			ExpectedDataType 													Extract(const size_t& length);

			/**
			 * @brief Reserves buffer size
			 * Ensures the buffer has enough capacity for the specified size.
			 * @param size Size to reserve.
			 */
			void 																Reserve(const std::size_t& size);

			/**
			 * @brief Resets the read position
			 * Sets the read position to the beginning of the buffer.
			 */
			inline void 														ResetPosition() const {
				m_position.store(0);
			}

			/**
			 * @brief Moves the read pointer within the buffer based on the specified position and mode.
			 *
			 * This function adjusts the read position (`m_position`) in the buffer according to the specified mode.
			 * The mode determines how the position is interpreted (e.g., relative to the current position, absolute,
			 * from the beginning, or from the end of the buffer). If the calculated position is negative, it is clamped
			 * to `0`. If the position exceeds the size of the buffer, it is clamped to the buffer's size.
			 *
			 * Supported modes:
			 * - `ReadPosition::Begin`: Sets the position relative to the beginning of the buffer.
			 * - `ReadPosition::End`: Sets the position relative to the end of the buffer.
			 * - `ReadPosition::Relative`: Adjusts the position relative to the current read position.
			 * - `ReadPosition::Absolute`: Sets the position to an absolute value.
			 *
			 * @param position The position to move to, interpreted based on the specified mode.
			 * @param mode The mode to use for seeking (e.g., `Begin`, `End`, `Relative`, `Absolute`).
			 *
			 * @note If multiple modes are specified (e.g., `Begin | Relative`), they are combined using bitwise OR.
			 *       The resulting position is calculated based on the combined modes.
			 * @note Thread-safe: This function ensures thread safety by locking the buffer during operations that
			 *       require access to the buffer's size or data.
			 */
			void																Seek(const std::ptrdiff_t& position, const ReadPosition& mode) const;

			/**
			 * @brief Retrieves the length of the buffer
			 * @return Length of the buffer.
			 */
			std::size_t 														Size() const noexcept;

			/**
			 * @brief Checks if buffer is empty
			 * @return True if the buffer is empty, false otherwise.
			 */
			bool 																Empty() const noexcept;

			/**
			 * @brief Checks if the read position is at the end
			 * @return True if the read position is at the end, false otherwise.
			 */
			bool 																End() const noexcept;

		private:
			mutable std::shared_mutex m_data_mutex;								///< Mutex for thread safety.
			std::vector<std::byte> m_data;										///< Stored value.
			mutable std::atomic<std::size_t> m_position;						///< Read position.
	};
}
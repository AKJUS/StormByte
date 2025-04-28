#pragma once

#include <StormByte/buffer/simple.hxx>

#include <atomic>
#include <cstddef>
#include <memory>
#include <mutex>
#include <shared_mutex>
#include <string>

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
     * @class Shared
     * @brief A thread-safe version of the `Simple` buffer.
     *
     * The `Shared` class extends the `Simple` buffer to provide thread-safe operations for storing and manipulating byte data.
     * It ensures data integrity in multi-threaded environments by using a `std::shared_mutex` for synchronization.
     *
     * **Key Features:**
     * - **Thread-Safe**: Ensures thread safety using a `std::shared_mutex`. Reads allow concurrent access, while writes are exclusive.
     * - **Flexible Data Management**: Supports appending, extracting, and reading data with thread-safe guarantees.
     * - **Position Management**: Provides methods to manage the read position, including seeking and resetting.
     *
     * This class is ideal for scenarios where multiple threads need to read from or write to a shared buffer
     * while maintaining data integrity and avoiding race conditions.
     */
    class STORMBYTE_PUBLIC Shared: public Simple {
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
            Shared(const Buffer::Data& data);

            /**
             * @brief Constructor
             * @param data Vector of bytes to move into the buffer.
             */
            Shared(Buffer::Data&& data);

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
            virtual ~Shared() noexcept override = default;

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
             * @brief Sets the buffer status
             * @see Buffer::Status
             */
            Shared& 															operator<<(const Status& status);

            /**
             * @brief Appends a buffer to the current buffer
             * Thread-safe version of @see Simple::operator<<.
             */
            Shared& 															operator<<(const Simple& buffer);

            /**
             * @brief Moves a buffer and appends to the current buffer
             * Thread-safe version of @see Simple::operator<<.
             */
            Shared& 															operator<<(Simple&& buffer);

            /**
             * @brief Appends a string to the current shared buffer
             * Thread-safe version of @see Simple::operator<<.
             */
            Shared& 															operator<<(const std::string& data);

            /**
             * @brief Appends a byte vector to the current shared buffer
             * Thread-safe version of @see Simple::operator<<.
             */
            Shared& 															operator<<(const Buffer::Data& data);

            /**
             * @brief Moves a byte vector and appends to the current shared buffer
             * Thread-safe version of @see Simple::operator<<.
             */
            Shared& 															operator<<(Buffer::Data&& data);

			/**
			 * @brief Appends a numeric value to the current simple buffer.
			 * 
			 * This templated method allows appending numeric values (e.g., integers, floating-point numbers)
			 * to the buffer. The numeric value is serialized into its binary representation and appended
			 * to the buffer.
			 * 
			 * @tparam NumericType The type of the numeric value to append.
			 * @param value The numeric value to append.
			 * @return Reference to the updated simple buffer.
			 */
			template <typename NumericType, typename = std::enable_if_t<std::is_arithmetic_v<std::decay_t<NumericType>>>>
			Shared& operator<<(const NumericType& value) {
				std::unique_lock lock(m_data_mutex);
				Simple::operator<<(value);
				return *this;
			}

            /**
             * @brief Appends current shared buffer to target shared buffer
             * Thread-safe version of @see Simple::operator>>.
             */
            Shared& 															operator>>(Shared& buffer);

			/**
			 * @brief Gets the available bytes to read from current position
			 * @return Number of bytes available to read.
			 */
			virtual size_t 														AvailableBytes() const noexcept;

            /**
             * @brief Retrieves the capacity of the shared buffer
             * Thread-safe version of @see Simple::Capacity.
             */
            size_t 																Capacity() const noexcept override;

            /**
             * @brief Clears the shared buffer
             * Thread-safe version of @see Simple::Clear.
             */
            void 																Clear() noexcept override;

            /**
             * @brief Retrieves a copy of the buffer data
             * Thread-safe version of @see Simple::Data.
             */
            Buffer::Data 														Data() const noexcept override;

            /**
             * @brief Discards data from the buffer
             * Thread-safe version of @see Simple::Discard.
             */
            void 																Discard(const std::size_t& length, const Read::Position& mode) noexcept override;

            /**
             * @brief Checks if the read position is at the end
             * Thread-safe version of @see Simple::End.
             */
            bool 																End() const noexcept override;

            /**
             * @brief Checks if the shared buffer is empty
             * Thread-safe version of @see Simple::Empty.
             */
            bool 																Empty() const noexcept override;

            /**
             * @brief Extracts a specific size of data, taking ownership of the read data and removing it from the shared buffer.
             * 
             * Unlike `Simple::Extract`, this method waits for the requested data to become available while the buffer is `IsReadable`.
             * If the buffer becomes unreadable (e.g., due to `EoF` or an error), it returns a `BufferOverflow` error.
             * 
             * @param length Length of the data to read and remove from the shared buffer.
             * @return `ExpectedDataType` containing the requested data, or an `Unexpected` with a `BufferOverflow` error if
             *         insufficient data exists and the buffer is no longer readable.
             * @see Simple::Extract
             */
            ExpectedData<BufferOverflow> 										Extract(const size_t& length) override;

            /**
             * @brief Extracts a specific size of data and moves it directly into the provided buffer.
             * 
             * Unlike `Simple::ExtractInto`, this method waits for the requested data to become available while the buffer is `IsReadable`.
             * If the buffer becomes unreadable (e.g., due to `EoF` or an error), it returns `Read::Status::Error`.
             * 
             * @param length Length of the data to extract.
             * @param output Buffer where the extracted data will be moved.
             * @return `Read::Status` indicating the success or failure of the operation.
             * @see Simple::ExtractInto
             */
            Read::Status 														ExtractInto(const size_t& length, Shared& output) noexcept;

            /**
             * @brief Checks if the shared buffer has enough data starting from the current read position
             * Thread-safe version of @see Simple::HasEnoughData.
             */
            bool 																HasEnoughData(const std::size_t& length) const override;

            /**
             * @brief Retrieves the stored value as a hexadecimal string
             * Thread-safe version of @see Simple::HexData.
             */
            std::string 														HexData(const std::size_t& column_size = 16) const override;

            /**
             * @brief Checks if the buffer is at the end of the file/data in case its write end have been closed
			 * @return True if the buffer is at the end and write closed, false otherwise.
             */
            bool 																IsEoF() const noexcept override;

            /**
             * @brief Checks if the buffer is readable.
             * 
             * A buffer is considered readable if it is not in an `Closed` or `Error` state.
             * @return True if the buffer is readable, false otherwise.
             */
            bool 																IsReadable() const noexcept;

            /**
             * @brief Checks if the buffer is writable.
             * 
             * A buffer is considered writable if it is not in an `Closed` or `Error` state.
             * @return True if the buffer is writable, false otherwise.
             */
            bool 																IsWritable() const noexcept;

            /**
             * @brief Locks the shared buffer for exclusive access
             * Prevents other threads from reading or writing to the buffer until `Unlock()` is called.
             */
            void 																Lock();

            /**
             * @brief Peeks at the next byte in the shared buffer without advancing the read position
             * Thread-safe version of @see Simple::Peek.
             */
            ExpectedByte<BufferOverflow> 										Peek() const override;

            /**
             * @brief Retrieves the read position
             * Thread-safe version of @see Simple::Position.
             */
            std::size_t 														Position() const noexcept override;

            /**
             * @brief Reads a specific size of data starting from the current read position.
             * 
             * Unlike `Simple::Read`, this method waits for the requested data to become available while the buffer is `IsReadable`.
             * If the buffer becomes unreadable (e.g., due to `EoF` or an error), it returns a `BufferOverflow` error.
             * 
             * @param length Length of the data to read.
             * @return `ExpectedDataType` containing a copy of the requested data, or an `Unexpected` with a `BufferOverflow` error
             *         if insufficient data exists and the buffer is no longer readable.
             * @see Simple::Read
             */
            ExpectedData<BufferOverflow> 										Read(const size_t& length) const override;

            /**
             * @brief Reserves shared buffer size
             * Thread-safe version of @see Simple::Reserve.
             */
            void 																Reserve(const std::size_t& size) override;

            /**
             * @brief Moves the read pointer within the shared buffer based on the specified position and mode
             * Thread-safe version of @see Simple::Seek.
             */
            void 																Seek(const std::ptrdiff_t& position, const Read::Position& mode) const override;

            /**
             * @brief Retrieves the length of the shared buffer
             * Thread-safe version of @see Simple::Size.
             */
            std::size_t 														Size() const noexcept override;

            /**
             * @brief Gets the buffer status
             */
            enum Status 														Status() const noexcept;

            /**
             * @brief Unlocks the shared buffer, releasing exclusive access
             * Allows other threads to access the buffer after it has been locked using `Lock()`.
             */
            void 																Unlock();

            /**
             * @brief Writes a simple buffer to the current shared buffer
             * Thread-safe version of @see Simple::Write.
             *
             * @param buffer Simple buffer to write.
             * @return Write::Status of the operation.
             */
            Write::Status 														Write(const Simple& buffer) override;

            /**
             * @brief Moves a simple buffer and writes it to the current shared buffer
             * Thread-safe version of @see Simple::Write.
             *
             * @param buffer Simple buffer to write.
             * @return Write::Status of the operation.
             */
            Write::Status 														Write(Simple&& buffer) override;

            /**
             * @brief Writes a string to the current shared buffer
             * Thread-safe version of @see Simple::Write.
             *
             * @param data String to write.
             * @return Write::Status of the operation.
             */
            Write::Status 														Write(const std::string& data) override;

            /**
             * @brief Writes a byte vector to the current shared buffer
             * Thread-safe version of @see Simple::Write.
             *
             * @param data Byte vector to write.
             * @return Write::Status of the operation.
             */
            Write::Status 														Write(const Buffer::Data& data) override;

            /**
             * @brief Moves a byte vector and writes it to the current shared buffer
             * Thread-safe version of @see Simple::Write.
             *
             * @param data Byte vector to write.
             * @return Write::Status of the operation.
             */
            Write::Status 														Write(Buffer::Data&& data) override;

        protected:
            mutable std::shared_mutex m_data_mutex; 							///< Mutex for thread safety.
            std::atomic<enum Status> m_status;									///< Buffer status.

            /**
             * @brief Waits for a specific amount of data to become available in the buffer.
             * 
             * This function blocks until the requested amount of data is available in the buffer
             * or until the buffer is marked as `EoF` (End of File) or `Error`.
             * 
             * **Behavior:**
             * - If the required data becomes available, the function returns `Read::Status::Success`.
             * - If the buffer is marked as `EoF` or encounters an error while waiting, the function
             *   returns `Read::Status::Error`.
             * 
             * @param length The number of bytes to wait for.
             * @return `Read::Status` indicating the result of the wait operation.
             */
            Read::Status 														Wait(const std::size_t length) const noexcept;
    };
}
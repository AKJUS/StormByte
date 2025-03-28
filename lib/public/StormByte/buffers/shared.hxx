#pragma once

#include <StormByte/buffers/simple.hxx>

#include <atomic>
#include <cstddef>
#include <memory>
#include <mutex>
#include <shared_mutex>
#include <string>

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
    /**
     * @class Shared
     * @brief A thread-safe version of the `Simple` buffer.
     *
     * The `Shared` class extends the `Simple` buffer to provide thread-safe operations for storing and manipulating byte data.
     * It supports various operations such as appending, extracting, reading, and seeking within the buffer, while ensuring
     * data integrity in multi-threaded environments.
     *
     * **Key Features:**
     * - **Thread-Safe**: Ensures thread safety using a `std::shared_mutex`. Reads are non-blocking
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
            virtual ~Shared() noexcept override 								= default;

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
			 * Sets buffer status
			 * @see Buffers::Status
			 */
			Shared& 															operator<<(const Status& status);

            /**
             * @brief Appends a buffer to the current buffer
             * @see Simple::operator<<
             */
            Shared& 															operator<<(const Simple& buffer);

            /**
             * @brief Moves a buffer and appends to the current buffer
             * @see Simple::operator<<
             */
            Shared& 															operator<<(Simple&& buffer);

            /**
             * @brief Appends a string to the current shared buffer
             * @see Simple::operator<<
             */
            Shared& 															operator<<(const std::string& data);

            /**
             * @brief Appends a byte vector to the current shared buffer
             * @see Simple::operator<<
             */
            Shared& 															operator<<(const Buffers::Data& data);

            /**
             * @brief Moves a byte vector and appends to the current shared buffer
             * @see Simple::operator<<
             */
            Shared& 															operator<<(Buffers::Data&& data);

            /**
             * @brief Appends current shared buffer to target shared buffer
             * @see Simple::operator>>
             */
            Shared& 															operator>>(Shared& buffer);

            /**
             * @brief Retrieves the capacity of the shared buffer
             * @see Simple::Capacity
             */
            virtual size_t 														Capacity() const noexcept override;

            /**
             * @brief Clears the shared buffer
             * Thread-safe version of @see Simple::Clear.
             */
            virtual void 														Clear() noexcept override;

            /**
             * @brief Retrieves a copy of the buffer data.
             * Thread-safe version of @see Simple::Data.
             */
            virtual Buffers::Data 												Data() const noexcept override;

            /**
             * @brief Discards data from the buffer.
             * Thread-safe version of @see Simple::Discard.
             */
            virtual void 														Discard(const std::size_t& length, const Read::Position& mode) noexcept override;

            /**
             * @brief Checks if the read position is at the end
             * Thread-safe version of @see Simple::End.
             */
            virtual bool 														End() const noexcept override;

            /**
             * @brief Checks if the shared buffer is empty
             * Thread-safe version of @see Simple::Empty.
             */
            virtual bool 														Empty() const noexcept override;

            /**
             * @brief Extracts a specific size of data, taking ownership of the read data and removing it from the shared buffer.
             * Thread-safe version of @see Simple::Extract.
             */
            virtual ExpectedData<BufferOverflow> 								Extract(const size_t& length) override;

            /**
             * @brief Checks if the shared buffer has enough data starting from the current read position.
             * Thread-safe version of @see Simple::HasEnoughData.
             */
            virtual bool 														HasEnoughData(const std::size_t& length) const override;

            /**
             * @brief Retrieves the stored value as a hexadecimal string
             * Thread-safe version of @see Simple::HexData.
             */
            virtual std::string 												HexData(const std::size_t& column_size = 16) const override;

            /**
             * @brief Locks the shared buffer for exclusive access.
             * Prevents other threads from reading or writing to the buffer until `Unlock()` is called.
             */
            virtual void 														Lock();

            /**
             * @brief Peeks at the next byte in the shared buffer without advancing the read position.
             * Thread-safe version of @see Simple::Peek.
             */
            virtual ExpectedByte<BufferOverflow>								Peek() const override;

            /**
             * @brief Retrieves the read position
             * Thread-safe version of @see Simple::Position.
             */
            virtual std::size_t 												Position() const noexcept override;

            /**
             * @brief Reads a specific size of data starting from the current read position.
             * Thread-safe version of @see Simple::Read.
             */
            virtual ExpectedData<BufferOverflow> 								Read(const size_t& length) const override;

            /**
             * @brief Reserves shared buffer size
             * Thread-safe version of @see Simple::Reserve.
             */
            virtual void 														Reserve(const std::size_t& size) override;

            /**
             * @brief Moves the read pointer within the shared buffer based on the specified position and mode.
             * Thread-safe version of @see Simple::Seek.
             */
            virtual void 														Seek(const std::ptrdiff_t& position, const Read::Position& mode) const override;

            /**
             * @brief Retrieves the length of the shared buffer
             * Thread-safe version of @see Simple::Size.
             */
            virtual std::size_t 												Size() const noexcept override;

			/**
			 * Gets buffer status
			 */
			enum Status 														Status() const noexcept;

            /**
             * @brief Unlocks the shared buffer, releasing exclusive access.
             * Allows other threads to access the buffer after it has been locked using `Lock()`.
             */
            virtual void 														Unlock();

            /**
             * @brief Writes a simple buffer to the current shared buffer.
             * Thread-safe version of @see Simple::Write.
             *
             * @param buffer Simple buffer to write.
             * @return Write::Status of the operation.
             */
            virtual Write::Status 												Write(const Simple& buffer) override;

            /**
             * @brief Moves a simple buffer and writes it to the current shared buffer.
             * Thread-safe version of @see Simple::Write.
             *
             * @param buffer Simple buffer to write.
             * @return Write::Status of the operation.
             */
            virtual Write::Status 												Write(Simple&& buffer) override;

            /**
             * @brief Writes a string to the current shared buffer.
             * Thread-safe version of @see Simple::Write.
             *
             * @param data String to write.
             * @return Write::Status of the operation.
             */
            virtual Write::Status 												Write(const std::string& data) override;

            /**
             * @brief Writes a byte vector to the current shared buffer.
             * Thread-safe version of @see Simple::Write.
             *
             * @param data Byte vector to write.
             * @return Write::Status of the operation.
             */
            virtual Write::Status 												Write(const Buffers::Data& data) override;

            /**
             * @brief Moves a byte vector and writes it to the current shared buffer.
             * Thread-safe version of @see Simple::Write.
             *
             * @param data Byte vector to write.
             * @return Write::Status of the operation.
             */
            virtual Write::Status 												Write(Buffers::Data&& data) override;

        protected:
            mutable std::shared_mutex m_data_mutex; 							///< Mutex for thread safety.
			std::atomic<enum Status> m_status;									///< Buffer status.

			/**
			 * Reads a specific size of data starting from the current read position waiting for data available if needed
			 * 
			 * If buffer is marked EoF (or error) while waiting for data will return BufferOverflow error
			 */
			ExpectedData<BufferOverflow> 										ReadWaiting(const std::size_t& length) const noexcept;

			/**
			 * Checks if the buffer is readable
			 */
			bool 																IsReadable() const noexcept;

			/**
			 * Checks if the buffer is writable
			 */
			bool 																IsWritable() const noexcept;

			/**
			 * Checks if the buffer is at the end of the file/data
			 */
			bool 																IsEoF() const noexcept;
    };
}
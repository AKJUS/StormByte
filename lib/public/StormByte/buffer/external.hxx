#pragma once

#include <StormByte/buffer/reader.hxx>
#include <StormByte/buffer/shared.hxx>

/**
 * @namespace Buffer
 * @brief Namespace for buffer-related components in the StormByte library.
 *
 * The Buffer namespace provides classes and utilities for managing simple, shared, and producer/consumer
 * buffers in both single-threaded and multi-threaded environments. It supports a variety of use cases, including:
 * - **Simple Buffer**: Lightweight, non-thread-safe buffers for single-threaded environments.
 * - **Shared Buffer**: Flexible and efficient storage for byte data with concurrent access support.
 * - **External Buffer**: An enhancement of the `Shared` buffer allowing external reader function be executed before every read.
 * - **Producer/Consumer Buffer**: Advanced models for managing data flow between producers and consumers
 *   with status tracking (e.g., `Ready`, `EoF`, `Error`).
 * - **Thread Safety**: Shared and producer/consumer buffers are designed to ensure consistent behavior in multi-threaded environments.
 */
namespace StormByte::Buffer {
	/**
	 * @class External
	 * @brief A specialized buffer that extends the functionality of the `Shared` buffer by integrating an external `Reader` class.
	 *
	 * The `External` buffer builds upon the `Shared` buffer by allowing an external `Reader` class
	 * to dynamically fetch or generate data before every read operation. This enables integration with external sources
	 * such as files, network streams, or other producers.
	 *
	 * **Key Features:**
	 * - **Dynamic Data Retrieval:** Automatically fetches or generates data using the `Reader` class when needed.
	 * - **Thread-Safe Operations:** Inherits thread-safe behavior from the `Shared` buffer.
	 * - **EOF and Error Handling:** Supports marking the buffer as `EoF` (End of File) or `Error` based on the `Reader`'s return value.
	 * - **Flexible Data Appending:** Allows appending data from various sources, including other buffers, strings, and numeric values.
	 *
	 * This class is ideal for scenarios where data is not readily available in memory and must be fetched or generated
	 * dynamically during read operations.
	 *
	 * @see Reader
	 */
	class STORMBYTE_PUBLIC External: public Shared {
		public:
			/**
			 * @brief Constructs an `External` buffer with a specified `Reader` instance.
			 * @param reader The external `Reader` instance to be used for dynamic data retrieval.
			 *
			 * The `Reader` class provides the `Read()` method, which dynamically fetches or generates data
			 * to populate the buffer before read operations. For more details, see the `Reader` class documentation.
			 *
			 * @see Reader
			 */
			External(const Reader& reader) noexcept;

			/**
			 * @brief Constructs an `External` buffer taking ownership of a specified `Reader` instance.
			 * @param reader The external `Reader` instance to be used for dynamic data retrieval.
			 *
			 * The `Reader` class provides the `Read()` method, which dynamically fetches or generates data
			 * to populate the buffer before read operations. For more details, see the `Reader` class documentation.
			 *
			 * @see Reader
			 */
			External(Reader&& reader) noexcept;

			/**
			 * @brief Copy constructor.
			 * @param other The `External` buffer to copy from.
			 *
			 * Copies the state of the `External` buffer, including its `Shared` buffer functionality and the associated `Reader`.
			 */
			External(const External& other)										= default;

			/**
			 * @brief Move constructor.
			 * @param other The `External` buffer to move from.
			 *
			 * Moves the state of the `External` buffer, including its `Shared` buffer functionality and the associated `Reader`.
			 */
			External(External&& other) noexcept									= default;

			/**
			 * @brief Destructor.
			 * Cleans up the buffer and releases any associated resources.
			 *
			 * Extends the `Shared` buffer's cleanup process to include the external `Reader` instance.
			 */
			~External() noexcept override 										= default;

			/**
			 * @brief Copy assignment operator.
			 * @param other The `External` buffer to copy from.
			 * @return Reference to the updated buffer.
			 *
			 * Copies the state of the `External` buffer, including its `Shared` buffer functionality and the associated `Reader`.
			 */
			External& operator=(const External& other) 							= default;

			/**
			 * @brief Move assignment operator.
			 * @param other The `External` buffer to move from.
			 * @return Reference to the updated buffer.
			 *
			 * Moves the state of the `External` buffer, including its `Shared` buffer functionality and the associated `Reader`.
			 */
			External& operator=(External&& other) noexcept						= default;

			/**
			 * @brief Sets the buffer status.
			 * @param status The new status to set.
			 * @return Reference to the updated buffer.
			 * @see Buffer::Status
			 *
			 * Extends the `Shared` buffer's status management by supporting external reader integration.
			 */
			External& 															operator<<(const Buffer::Status& status);

			/**
			 * @brief Appends data from another buffer to this buffer.
			 * @param buffer The source buffer to append from.
			 * @return Reference to the updated buffer.
			 *
			 * Extends the `Shared` buffer's appending functionality to support external reader integration.
			 */
			External& 															operator<<(const Simple& buffer);

			/**
			 * @brief Moves data from another buffer and appends it to this buffer.
			 * @param buffer The source buffer to move from.
			 * @return Reference to the updated buffer.
			 *
			 * Extends the `Shared` buffer's appending functionality to support external reader integration.
			 */
			External& 															operator<<(Simple&& buffer);

			/**
			 * @brief Appends a string to the buffer.
			 * @param data The string to append.
			 * @return Reference to the updated buffer.
			 *
			 * Extends the `Shared` buffer's appending functionality to support external reader integration.
			 */
			External& 															operator<<(const std::string& data);

			/**
			 * @brief Appends a byte vector to the buffer.
			 * @param data The byte vector to append.
			 * @return Reference to the updated buffer.
			 *
			 * Extends the `Shared` buffer's appending functionality to support external reader integration.
			 */
			External& 															operator<<(const Buffer::Data& data);

			/**
			 * @brief Moves a byte vector and appends it to the buffer.
			 * @param data The byte vector to move and append.
			 * @return Reference to the updated buffer.
			 *
			 * Extends the `Shared` buffer's appending functionality to support external reader integration.
			 */
			External& 															operator<<(Buffer::Data&& data);

			/**
			 * @brief Appends a numeric value to the buffer.
			 * @tparam NumericType The type of the numeric value to append.
			 * @param value The numeric value to append.
			 * @return Reference to the updated buffer.
			 *
			 * The numeric value is serialized into its binary representation and appended to the buffer.
			 * Extends the `Shared` buffer's appending functionality to support external reader integration.
			 */
			template <typename NumericType, typename = std::enable_if_t<std::is_arithmetic_v<std::decay_t<NumericType>>>>
			External& 															operator<<(const NumericType& value) {
				Shared::operator<<(value);
				return *this;
			}

			/**
			 * @brief Appends the current buffer's data to another buffer.
			 * @param buffer The target buffer to append to.
			 * @return Reference to the updated buffer.
			 *
			 * Extends the `Shared` buffer's appending functionality to support external reader integration.
			 */
			External& 															operator>>(External& buffer);

			/**
			 * @brief Sets the external `Reader` instance.
			 * @param reader The external `Reader` instance to set.
			 *
			 * The `Reader` class provides the `Read()` method, which dynamically fetches or generates data
			 * to populate the buffer before read operations. For more details, see the `Reader` class documentation.
			 *
			 * @see Reader
			 */
			void 																Reader(const class Reader& reader) noexcept;

			/**
			 * @brief Sets the external `Reader` instance taking ownership of it.
			 * @param reader The external `Reader` instance to set.
			 *
			 * The `Reader` class provides the `Read()` method, which dynamically fetches or generates data
			 * to populate the buffer before read operations. For more details, see the `Reader` class documentation.
			 *
			 * @see Reader
			 */
			void 																Reader(class Reader&& reader) noexcept;

			/**
			 * @brief Gets the number of bytes available to read from the buffer.
			 * @return The number of bytes available to read.
			 *
			 * Extends the `Shared` buffer's functionality by transparently calling the `Reader`'s `Read()` method
			 * to ensure the buffer is populated with the latest data before returning the available byte count.
			 *
			 * @see Reader
			 */
			size_t 																AvailableBytes() const noexcept override;

			/**
			 * @brief Checks if the buffer has enough data to satisfy a read request.
			 * @param length The number of bytes to check for.
			 * @return `true` if enough data is available, `false` otherwise.
			 *
			 * This method transparently calls the `Reader`'s `Read()` method to ensure the buffer is populated
			 * with the latest data before performing the check.
			 *
			 * @see Reader
			 */
			bool 																HasEnoughData(const std::size_t& length) const override;

			/**
			 * @brief Reads a specific size of data from the buffer.
			 * @param length The number of bytes to read.
			 * @return The requested data or a `BufferOverflow` error if insufficient data exists.
			 *
			 * This method waits for the requested data to become available while the buffer is readable.
			 *
			 * @see Reader
			 */
			ExpectedData<BufferOverflow> 										Read(const size_t& length) const override;

		private:
			std::shared_ptr<class Reader> m_external_reader; 					///< The external `Reader` instance.

			/**
			 * @brief Waits for a specific amount of data to become available in the buffer.
			 * @param length The number of bytes to wait for.
			 * @return `Read::Status` indicating the result of the wait operation.
			 *
			 * This method blocks until the requested data is available or the buffer is marked as `EoF` or `Error`.
			 */
			Read::Status 														Wait(const std::size_t& length) const noexcept override;

			/**
			 * @brief Reads data from the external `Reader` and writes it to the buffer.
			 *
			 * This method is called automatically before every read operation to ensure the buffer is populated
			 * with the latest data from the external `Reader`.
			 *
			 * @see Reader
			 */
			bool 																ReadExternalData(const size_t& bytes) noexcept;
	};
}
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
	/**
	 * @class Consumer
	 * @brief A thread-safe class for handling consumer buffers with destructive read operations.
	 *
	 * The `Consumer` class is designed for use in producer-consumer models where the consumer
	 * reads data from a shared buffer. This class provides methods to extract data and skip unwanted
	 * data from the start of the buffer.
	 *
	 * **Key Features:**
	 * - **Destructive read operations**: The `Consumer` allows reading data through the `Extract` method.
	 *   Each read operation removes the extracted data from the buffer, making it unavailable for subsequent reads.
	 *   No data can be inserted into the buffer using this class.
	 * - **Always reads from the start**: The `Consumer` always reads data from the start of the buffer.
	 *   This design eliminates the concept of a "read position." If unwanted data exists (e.g., padding), it can
	 *   be removed using the `Skip` method.
	 * - **Thread-safe**: All operations are thread-safe, ensuring consistent behavior in multi-threaded environments.
	 * - **Shared buffer**: Copies of a `Consumer` share the same underlying buffer, meaning changes to the
	 *   buffer's state (e.g., data removal) by one instance will be reflected in all copies.
	 *
	 * This class is ideal for scenarios where multiple threads need to consume data from a shared buffer
	 * while maintaining thread safety and avoiding data corruption.
	 */
	class STORMBYTE_PUBLIC Consumer final: public Async {
		public:
			/**
			 * @brief Constructor
			 * Creates a `Consumer` instance linked to the specified buffer.
			 * @param async The async buffer to copy from
			 */
			explicit Consumer(const Async& async) noexcept;
			
			/**
			 * @brief Copy constructor
			 * Creates a new `Consumer` that shares the same underlying buffer as the original.
			 * @param other Consumer to copy
			 */
			Consumer(const Consumer& other)													= default;

			/**
			 * @brief Move constructor
			 * Moves the contents of another `Consumer` into this instance.
			 * @param other Consumer to move
			 */
			Consumer(Consumer&& other) noexcept												= default;

			/**
			 * @brief Destructor
			 * Cleans up the `Consumer` instance.
			 */
			~Consumer() noexcept override													= default;

			/**
			 * @brief Copy assignment operator
			 * Copies the state of another `Consumer` into this instance.
			 * @param other Consumer to copy
			 * @return Reference to this buffer
			 */
			Consumer& operator=(const Consumer& other)										= default;

			/**
			 * @brief Move assignment operator
			 * Moves the contents of another `Consumer` into this instance.
			 * @param other Consumer to move
			 * @return Reference to this buffer
			 */
			Consumer& operator=(Consumer&& other) noexcept									= default;

			/**
			 * @brief Extracts a specific size of data, taking ownership of the read data and removing it from the buffer.
			 *
			 * This function extracts the requested number of bytes from the start of the buffer. The extracted data
			 * is returned as a new `DataType` (i.e., `std::vector<std::byte>`), and the corresponding bytes are removed
			 * from the buffer. If the requested length exceeds the available data in the buffer, an `Unexpected` with
			 * a `BufferOverflow` error is returned.
			 *
			 * **Thread Safety:**
			 * - This operation is thread-safe and can be used in multi-threaded environments.
			 *
			 * @param length Length of the data to read and remove from the buffer.
			 * @return `ExpectedDataType` containing the requested data, or an `Unexpected` with a `BufferNotReady` error
			 *         if insufficient data exists.
			 * @note This operation modifies the buffer by removing the extracted data.
			 */
			ExpectedData<Buffers::Exception>												Extract(const size_t& length);

			/**
			 * @brief Extracts all current data from the buffer, removing it in the process.
			 *
			 * This function extracts all the data currently available in the buffer, starting from the beginning.
			 * The extracted data is returned as a new `DataType` (i.e., `std::vector<std::byte>`), and the corresponding
			 * bytes are removed from the buffer. Calling this function does not guarantee that the buffer will remain
			 * empty afterward, as producers may add more data to the buffer at any time.
			 *
			 * **Thread Safety:**
			 * - This operation is thread-safe and can be used in multi-threaded environments.
			 *
			 * @return `ExpectedDataType` containing all the current data, or an `Unexpected` with a `BufferNotReady` error
			 *         if an error occurs during extraction.
			 * @note This operation modifies the buffer by removing the extracted data.
			 */
			ExpectedData<Buffers::Exception>												Extract();

			/**
			 * @brief Skips a specified number of characters (removing them from the buffer).
			 *
			 * This function removes the specified number of characters (`length`) from the start of the buffer.
			 * The skipped data is discarded and cannot be retrieved afterward. The function ensures thread safety
			 * and will block if there is insufficient data available in the buffer to skip the requested number
			 * of characters.
			 *
			 * **Behavior:**
			 * - If the buffer does not contain enough data to skip the requested length, the function will block
			 *   and wait until sufficient data becomes available or the buffer's status changes.
			 * - If the buffer's status changes to `EoF` (End of File) and there is still insufficient data, the
			 *   function will return an error.
			 * - If the buffer's status changes to `Error`, the function will immediately return an error.
			 *
			 * **Thread Safety:**
			 * - This operation is thread-safe and can be used in multi-threaded environments.
			 *
			 * @param length The number of characters to skip (remove) from the buffer.
			 * @return `Expected<void, BufferNotReady>`:
			 *         - `void` on success, indicating that the requested number of characters were successfully skipped.
			 *         - `BufferNotReady` error if the buffer is in an `Error` state or if insufficient data exists while the buffer is in `EoF`.
			 *
			 * @note This function modifies the buffer by removing the skipped data.
			 * @note The function will block and wait for sufficient data unless the buffer is in an `EoF` or `Error` state.
			 */
			Expected<void, BufferNotReady>													Skip(const std::size_t& length);

			/**
			 * @brief Sets the status of the buffer.
			 * @param status The new `Status` to assign to the buffer.
			 * @return Reference to the updated `Consumer` instance.
			 */
			Consumer& 																		operator<<(const enum Status& status);
	};
}
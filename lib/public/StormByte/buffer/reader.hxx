#pragma once

#include <StormByte/buffer/simple.hxx>
#include <StormByte/clonable.hxx>

/**
 * @namespace StormByte::Buffer
 * @brief Namespace for buffer-related components in the StormByte library.
 *
 * The `Buffer` namespace provides classes and utilities for managing various types of buffers in both single-threaded
 * and multi-threaded environments. It supports a variety of use cases, including:
 * - **Simple Buffer**: Lightweight, non-thread-safe buffers for single-threaded environments.
 * - **Shared Buffer**: Flexible and efficient storage for byte data with concurrent access support.
 * - **External Buffer**: An extension of the `Shared` buffer that allows an external reader function to be executed
 *   before every read operation.
 * - **Producer/Consumer Buffer**: Advanced models for managing data flow between producers and consumers with status
 *   tracking (e.g., `Ready`, `EoF`, `Error`).
 * - **Thread Safety**: Shared and producer/consumer buffers are designed to ensure consistent behavior in multi-threaded environments.
 */
namespace StormByte::Buffer {
	/**
	 * @class Reader
	 * @brief Abstract base class for implementing external data readers.
	 *
	 * The `Reader` class provides an interface for reading external data into a buffer. It is designed to be used
	 * with the `External` buffer, enabling dynamic data retrieval from external sources such as files, network streams,
	 * or other producers.
	 *
	 * **Key Features:**
	 * - Defines a virtual `Read()` method that must be implemented by derived classes.
	 * - Supports returning data as a `Buffer::Simple` object or throwing a `Buffer::Exception` in case of errors.
	 * - Allows empty buffers to be returned as valid responses.
	 *
	 * This class is ideal for scenarios where data needs to be fetched dynamically from external sources and integrated
	 * into the `External` buffer.
	 */
	class STORMBYTE_PUBLIC Reader: public Clonable<Reader> {
		public:
			/**
			 * @brief Default constructor.
			 *
			 * Initializes a `Reader` instance. Derived classes should implement the `Read()` method to provide
			 * specific functionality for reading external data.
			 */
			Reader() noexcept													= default;

			/**
			 * @brief Copy constructor.
			 * @param other The `Reader` instance to copy from.
			 */
			Reader(const Reader& other) 										= default;

			/**
			 * @brief Move constructor.
			 * @param other The `Reader` instance to move from.
			 */
			Reader(Reader&& other) noexcept										= default;

			/**
			 * @brief Destructor.
			 *
			 * Cleans up the `Reader` instance. Derived classes should ensure that any resources used for external
			 * data retrieval are properly released.
			 */
			virtual ~Reader() noexcept											= default;

			/**
			 * @brief Copy assignment operator.
			 * @param other The `Reader` instance to copy from.
			 * @return Reference to the updated `Reader` instance.
			 */
			Reader& operator=(const Reader& other) 								= default;

			/**
			 * @brief Move assignment operator.
			 * @param other The `Reader` instance to move from.
			 * @return Reference to the updated `Reader` instance.
			 */
			Reader& operator=(Reader&& other) noexcept							= default;

			/**
			 * @brief Reads data from an external source.
			 * @param min The minimum number of bytes to read (0 to read any available bytes).
			 * @return An `Expected<Buffer::Simple, Buffer::Exception>` containing the read data or an exception.
			 *
			 * This method must be implemented by derived classes to provide functionality for reading data from
			 * an external source. The returned data is encapsulated in a `Buffer::Simple` object.
			 *
			 * **Behavior:**
			 * - If data is successfully read, it should be returned as a `Buffer::Simple` object.
			 * - If an error occurs during the read operation, an `Unexpected<Buffer::Exception>` should be returned.
			 * - An empty buffer (i.e., a `Buffer::Simple` object with no data) is considered a valid response.
			 *
			 * **Error Handling:**
			 * - Instead of throwing exceptions, this method uses `Unexpected<Buffer::Exception>` to signal errors.
			 * - The `Buffer::Exception` provides details about the error, such as the reason for failure.
			 *
			 * **Usage:**
			 * This method is typically called by the `External` buffer to dynamically fetch or generate data
			 * before performing read operations.
			 */
			virtual Expected<Buffer::Simple, Buffer::Exception> 				Read(const size_t& min = 0) const noexcept = 0;
	};
}
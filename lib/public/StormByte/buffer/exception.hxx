#pragma once

#include <StormByte/exception.hxx>

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
     * @class Exception
     * @brief Base exception class for buffer-related errors.
     *
     * The `Exception` class serves as the base class for all exceptions related
     * to buffer operations in the `Buffer` namespace. It provides a unified interface
     * for handling errors that occur during buffer operations, such as overflows or
     * readiness issues.
     *
     * Inherits all functionality from the `StormByte::Exception` class.
     */
    class STORMBYTE_PUBLIC Exception: public StormByte::Exception {
        public:
            /**
             * @brief Constructor
             * @param reason A description of the exception as a `std::string`.
             */
            using StormByte::Exception::Exception;
    };

    /**
     * @class BufferOverflow
     * @brief Exception class for buffer overflow errors.
     *
     * The `BufferOverflow` exception is thrown when an operation attempts to write
     * more data to a buffer than it can hold. This exception ensures that buffer
     * integrity is maintained by preventing overflows.
     *
     * Inherits all functionality from the `StormByte::Buffer::Exception` class.
     */
    class STORMBYTE_PUBLIC BufferOverflow: public StormByte::Buffer::Exception {
        public:
            /**
             * @brief Constructor
             * @param reason A description of the exception as a `std::string`.
             */
            using StormByte::Buffer::Exception::Exception;
    };

    /**
     * @class BufferNotReady
     * @brief Exception class for buffer readiness errors.
     *
     * The `BufferNotReady` exception is thrown when an operation is attempted on a
     * buffer that is not in a ready state. This can occur if the buffer is empty,
     * has reached the end of its data (EOF), or is in an error state.
     *
     * This exception ensures that operations are only performed when the buffer is
     * in a valid state, preventing undefined behavior.
     *
     * Inherits all functionality from the `StormByte::Buffer::Exception` class.
     */
    class STORMBYTE_PUBLIC BufferNotReady: public StormByte::Buffer::Exception {
        public:
            /**
             * @brief Constructor
             * @param reason A description of the exception as a `std::string`.
             */
            using StormByte::Buffer::Exception::Exception;
    };
}
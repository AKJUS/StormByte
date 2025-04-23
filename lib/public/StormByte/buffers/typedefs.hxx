#pragma once

#include <StormByte/expected.hxx>

#include <cstddef>
#include <functional>
#include <memory>
#include <span>
#include <type_traits>
#include <vector>

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
	 * @namespace Read
	 * @brief Namespace for read-related utilities in the StormByte library.
	 *
	 * The `Read` namespace provides enumerations and utilities for managing read positions
	 * within buffers.
	 */
	namespace Read {
		/**
		 * @enum Position
		 * @brief Enumeration to define the read position within a buffer.
		 *
		 * The `Read::Position` enumeration provides options for specifying the position
		 * to move the read pointer within a buffer. It supports absolute, relative,
		 * and boundary-based positioning.
		 *
		 * **Values:**
		 * - `Begin`: The read position is set to the beginning of the buffer.
		 * - `End`: The read position is set to the end of the buffer.
		 * - `Relative`: The read position is adjusted relative to the current position.
		 * - `Absolute`: The read position is set to an absolute offset within the buffer.
		 */
		enum class Position: unsigned short {
			Begin 		= 0x01,		///< The read position is at the beginning of the buffer.
			End 		= 0x02,		///< The read position is at the end of the buffer.
			Relative 	= 0x04,		///< The read position is relative to the current position.
			Absolute 	= 0x08		///< The read position is absolute.
		};

		/**
		 * @enum Status
		 * @brief Defines the status of a read operation.
		 *
		 * The `Read::Status` enumeration specifies the result of a read operation.
		 *
		 * **Values:**
		 * - `Success`: The read operation completed successfully.
		 * - `Error`: The read operation encountered an error.
		 */
		enum class Status: unsigned short {
			Success, 				///< Indicates the read operation was successful.
			Error    				///< Indicates an error occurred during the read operation.
		};
	}

	/**
	 * @namespace Write
	 * @brief Namespace for write-related utilities in the StormByte library.
	 *
	 * The `Write` namespace provides enumerations and utilities for managing the status
	 * of write operations within buffers.
	 */
	namespace Write {
		/**
		 * @enum Status
		 * @brief Defines the status of a write operation.
		 *
		 * The `Write::Status` enumeration specifies the result of a write operation.
		 *
		 * **Values:**
		 * - `Success`: The write operation completed successfully.
		 * - `Error`: The write operation encountered an error.
		 */
		enum class Status: unsigned short {
			Success, ///< Indicates the write operation was successful.
			Error    ///< Indicates the write operation encountered an error.
		};
	}

	/**
	 * @enum Status
	 * @brief Defines the status of the buffer during producer/consumer operations.
	 *
	 * The `Status` enumeration specifies the current state of the buffer.
	 *
	 * **Values:**
	 * - `Ready`: The buffer is ready for operations.
	 * - `EoF`: The buffer has reached the end of the file/data.
	 * - `Error`: The buffer has encountered an error.
	 */
	enum class Status {
		Ready, 																			///< Indicates the buffer is OK and ready for operations.
		ReadOnly,   																	///< Indicates the buffer has closed its write end.
		Error  																			///< Indicates the buffer has encountered an error.
	};
	
	// Forward declarations
	/**
	 * @class Simple
	 * @brief Forward declaration of the `Simple` class.
	 */
	class Simple;

	/**
	 * @class Shared
	 * @brief Forward declaration of the `Shared` class.
	 */
	class Shared;

	/**
	 * @class Producer
	 * @brief Forward declaration of the `Producer` class.
	 */
	class Producer;

	/**
	 * @class Consumer
	 * @brief Forward declaration of the `Consumer` class.
	 */
	class Consumer;

	// Data types
	using Byte							= std::byte;											///< Represents a single byte of data.
	using Data							= std::vector<Byte>;									///< Represents a collection of bytes stored in the buffer.
	template<class T>
	using ExpectedByte					= Expected<Byte, T>;									///< Represents a single byte with error handling.
	template<class T>
	using ExpectedByteRef				= Expected<Byte&, T>;									///< Represents a reference to a single byte with error handling.
	template<class T>
	using ExpectedByteSpan				= Expected<std::span<Byte>, T>;							///< Represents a span of bytes with error handling.
	template<class T>
	using ExpectedConstByte				= Expected<const Byte, T>;								///< Represents a constant byte with error handling.
	template<class T>
	using ExpectedConstByteRef			= Expected<const Byte&, T>;								///< Represents a reference to a constant byte with error handling.
	template<class T>
	using ExpectedConstByteSpan			= Expected<std::span<const Byte>, T>;					///< Represents a constant span of bytes with error handling.
	template<class T>
	using ExpectedData					= Expected<Data, T>;									///< Represents a collection of bytes with error handling.
	using PipeFunction					= std::function<void(Consumer, Producer)>;				///< Represents a function that processes a data pipe.
	using Processor						= std::function<std::shared_ptr<Simple>(const Simple&)>;///< Represents a function that processes a buffer.

	/**
	 * @brief Wrapper for `operator<<` to support `std::shared_ptr` or `std::unique_ptr` as the left-hand parameter.
	 *
	 * This template function allows appending data to instances of classes derived from `Simple` or `Async`
	 * when they are wrapped in smart pointers (`std::shared_ptr` or `std::unique_ptr`). It forwards the call
	 * to the existing `operator<<` implementation of the underlying object.
	 *
	 * @tparam PtrType The type of the smart pointer (e.g., `std::shared_ptr` or `std::unique_ptr`).
	 * @tparam T The type of the value to append.
	 * @param ptr Smart pointer to an instance of a class derived from `Simple` or `Async`.
	 * @param value The value to append to the buffer.
	 * @return Reference to the updated smart pointer.
	 * @note If the smart pointer is null, the function does nothing and returns the original pointer.
	 */
	template <typename PtrType, typename T,
		typename = std::enable_if_t<
			(std::is_same_v<PtrType, std::shared_ptr<typename PtrType::element_type>> ||
			 std::is_same_v<PtrType, std::unique_ptr<typename PtrType::element_type>>)
		>
	>
	inline PtrType& operator<<(PtrType& ptr, const T& value) {
		static_assert(std::is_same_v<PtrType, std::shared_ptr<typename PtrType::element_type>> ||
					  std::is_same_v<PtrType, std::unique_ptr<typename PtrType::element_type>>,
					  "PtrType must be a std::shared_ptr or std::unique_ptr");
		if (ptr) {
			*ptr << value; // Forward the call to the existing operator<<
		}
		return ptr;
	}

	/**
	 * @brief Wrapper for `operator<<` to support `std::shared_ptr` or `std::unique_ptr` as the left-hand parameter (move version).
	 *
	 * This template function allows appending data to instances of classes derived from `Simple` or `Async`
	 * when they are wrapped in smart pointers (`std::shared_ptr` or `std::unique_ptr`). It forwards the call
	 * to the existing `operator<<` implementation of the underlying object, using move semantics.
	 *
	 * @tparam PtrType The type of the smart pointer (e.g., `std::shared_ptr` or `std::unique_ptr`).
	 * @tparam T The type of the value to append.
	 * @param ptr Smart pointer to an instance of a class derived from `Simple` or `Async`.
	 * @param value The value to append to the buffer.
	 * @return Reference to the updated smart pointer.
	 * @note If the smart pointer is null, the function does nothing and returns the original pointer.
	 */
	template <typename PtrType, typename T,
			typename = std::enable_if_t<
			(std::is_same_v<PtrType, std::shared_ptr<typename PtrType::element_type>> ||
			 std::is_same_v<PtrType, std::unique_ptr<typename PtrType::element_type>>)
		>
	>
	inline PtrType& operator<<(PtrType& ptr, T&& value) {
		static_assert(std::is_same_v<PtrType, std::shared_ptr<typename PtrType::element_type>> ||
					  std::is_same_v<PtrType, std::unique_ptr<typename PtrType::element_type>>,
					  "PtrType must be a std::shared_ptr or std::unique_ptr");
		if (ptr) {
			*ptr << std::move(value); // Forward the call to the existing operator<<
		}
		return ptr;
	}
}
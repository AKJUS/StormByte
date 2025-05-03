#pragma once

#include <StormByte/buffer/shared.hxx>
#include <memory>

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
	class Consumer; // Forward declaration of the Consumer class

	/**
	 * @class Producer
	 * @brief A write-only interface for accessing a shared buffer.
	 *
	 * The `Producer` class provides a thread-safe, write-only interface for modifying data in a shared buffer.
	 * Unlike `Consumer`, empty `Producer` instances can be created directly. This allows for flexibility in scenarios
	 * where a producer needs to be initialized without an associated shared buffer.
	 *
	 * **Key Features:**
	 * - **Write-Only Access**: Allows only write operations on the buffer.
	 * - **Non-Copyable**: Instances of `Producer` cannot be copied to ensure data integrity.
	 * - **Thread-Safe**: All operations are thread-safe and synchronized using the underlying `Shared` buffer.
	 * - **Buffer Status Management**: Producers are responsible for setting the buffer status (e.g., `Status::EoF` or `Status::Error`)
	 *   to indicate to consumers that they should stop processing. Failing to do so might cause consumers to wait indefinitely
	 *   for incoming data.
	 * - **Consumer Creation**: The only way to create a `Consumer` instance is via the `Consumer` method of this class, which
	 *   returns a `Consumer` instance bound to this producer's buffer.
	 * - **Shared Buffer Behavior**: While the class is designed to have only one producer and one consumer instance,
	 *   if `Producer` instances are copied, they will share the same buffer. This is allowed, but it is up to the user
	 *   to ensure that the produced-consumed data flow remains in sync.
	 */
	class STORMBYTE_PUBLIC Producer final {
		public:
			/**
			 * @brief Default constructor
			 * 
			 * Initializes an empty producer buffer. Unlike `Consumer`, empty `Producer` instances can be created directly.
			 * This allows for flexibility in scenarios where a producer needs to be initialized without an associated shared buffer.
			 */
			Producer() noexcept;

			/**
			 * @brief Constructor
			 * 
			 * Binds a producer with a consumer
			 * 
			 * @param consumer The consumer to bind to.
			 */
			Producer(const Consumer& consumer);

			/**
			 * @brief Constructor
			 * 
			 * Initializes a producer with a shared buffer.
			 * 
			 * @param shared The shared buffer to bind to.
			 */
			Producer(std::shared_ptr<Shared> shared) noexcept;

			/**
			 * @brief Constructor
			 * 
			 * Initializes a producer with a shared buffer.
			 * 
			 * @param shared The shared buffer to bind to.
			 */
			Producer(const Shared& shared) noexcept;

			/**
			 * @brief Move constructor
			 * 
			 * Initializes a producer with a shared buffer.
			 * 
			 * @param shared The shared buffer to bind to.
			 */
			Producer(Shared&& shared) noexcept;

			/**
			 * @brief Constructor
			 * 
			 * Initializes a producer with a simple buffer.
			 * 
			 * @param simple The simple buffer to bind to.
			 */
			Producer(const Simple& simple) noexcept;

			/**
			 * @brief Move constructor
			 * 
			 * Initializes a producer with a simple buffer.
			 * 
			 * @param simple The simple buffer to bind to.
			 */
			Producer(Simple&& simple) noexcept;

			/**
			 * @brief Deleted copy constructor
			 * 
			 * The `Producer` class cannot be copied to ensure data integrity and prevent unintended sharing of the buffer.
			 */
			Producer(const Producer& other) 							= default;

			/**
			 * @brief Default move constructor
			 * 
			 * Allows moving a `Producer` instance. The moved-from instance is left in a valid but unspecified state.
			 */
			Producer(Producer&& other) noexcept 						= default;

			/**
			 * @brief Destructor
			 * 
			 * Cleans up the `Producer` instance. The destructor ensures that any resources held by the `Producer` are properly released.
			 */
			~Producer() noexcept 										= default;

			/**
			 * @brief Deleted copy assignment operator
			 * 
			 * The `Producer` class cannot be copied to ensure data integrity and prevent unintended sharing of the buffer.
			 */
			Producer& operator=(const Producer& other) 					= default;

			/**
			 * @brief Default move assignment operator
			 * 
			 * Allows moving a `Producer` instance. The moved-from instance is left in a valid but unspecified state.
			 */
			Producer& operator=(Producer&& other) noexcept 				= default;

			/**
			 * @brief Assignment operator using a consumer
			 * 
			 * Allows binding a producer to a consumer. This is useful for initializing a producer with an existing consumer.
			 */
			Producer& operator=(const Consumer& consumer) noexcept;

			/**
			 * @brief Sets the buffer status
			 * @see Buffer::Status
			 */
			Producer& 													operator<<(const Status& status);

			/**
			 * @brief Appends a buffer to the current buffer.
			 * @param buffer The buffer to append.
			 * @return Reference to the updated producer.
			 */
			Producer& 													operator<<(const Simple& buffer);

			/**
			 * @brief Moves a buffer and appends it to the current buffer.
			 * @param buffer The buffer to append.
			 * @return Reference to the updated producer.
			 */
			Producer& 													operator<<(Simple&& buffer);

			/**
			 * @brief Appends a string to the current shared buffer.
			 * @param data The string to append.
			 * @return Reference to the updated producer.
			 */
			Producer& 													operator<<(const std::string& data);

			/**
			 * @brief Appends a byte vector to the current shared buffer.
			 * @param data The byte vector to append.
			 * @return Reference to the updated producer.
			 */
			Producer& 													operator<<(const Buffer::Data& data);

			/**
			 * @brief Moves a byte vector and appends it to the current shared buffer.
			 * @param data The byte vector to append.
			 * @return Reference to the updated producer.
			 */
			Producer& 													operator<<(Buffer::Data&& data);

			/**
			 * @brief Gets the consumer interface to consume this buffer.
			 * 
			 * Creates and returns a `Consumer` instance that is bound to this producer's shared buffer.
			 * The returned `Consumer` instance allows read-only access to the buffer.
			 * 
			 * @return A `Consumer` instance bound to this producer's buffer.
			 */
			Consumer 													Consumer() const noexcept;

			/**
			 * @brief Locks the shared buffer for exclusive access.
			 * Prevents other threads from reading or writing to the buffer until `Unlock()` is called.
			 */
			void 														Lock();

			/**
			 * @brief Reserves shared buffer size.
			 * @param size The size to reserve.
			 */
			void 														Reserve(const std::size_t& size);

			/**
			 * @brief Unlocks the shared buffer, releasing exclusive access.
			 * Allows other threads to access the buffer after it has been locked using `Lock()`.
			 */
			void 														Unlock();

			/**
			 * @brief Writes a simple buffer to the current shared buffer.
			 * @param buffer The simple buffer to write.
			 * @return Write::Status of the operation.
			 */
			Write::Status 												Write(const Simple& buffer);

			/**
			 * @brief Moves a simple buffer and writes it to the current shared buffer.
			 * @param buffer The simple buffer to write.
			 * @return Write::Status of the operation.
			 */
			Write::Status 												Write(Simple&& buffer);

			/**
			 * @brief Writes a string to the current shared buffer.
			 * @param data The string to write.
			 * @return Write::Status of the operation.
			 */
			Write::Status 												Write(const std::string& data);

			/**
			 * @brief Writes a byte vector to the current shared buffer.
			 * @param data The byte vector to write.
			 * @return Write::Status of the operation.
			 */
			Write::Status 												Write(const Buffer::Data& data);

			/**
			 * @brief Moves a byte vector and writes it to the current shared buffer.
			 * @param data The byte vector to write.
			 * @return Write::Status of the operation.
			 */
			Write::Status 												Write(Buffer::Data&& data);

		private:
			std::shared_ptr<Shared> m_shared; ///< The shared buffer instance.
	};
}
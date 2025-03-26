#pragma once

#include <StormByte/buffer.hxx>
#include <StormByte/helpers.hxx>

/**
 * @namespace StormByte
 * @brief Main namespace for the StormByte library and components.
 */
namespace StormByte {
	class ConsumerBuffer; 																		///< Forward declaration of ConsumerBuffer.
	class ProducerBuffer; 																		///< Forward declaration of ProducerBuffer.

	/**
	 * @class AsyncBuffer
	 * @brief Manages byte buffers in a multi-threaded consumer/producer model.
	 *
	 * The `AsyncBuffer` class enables multiple threads to safely read and write data,
	 * where `ConsumerBuffer` is responsible for reading data and `ProducerBuffer` handles
	 * writing data. The `ProducerBuffer` must also update the buffer status to `EoF` (End of File)
	 * or `Error` to signal the end of operations or any issues encountered during processing.
	 *
	 * If the `ProducerBuffer` fails to update the buffer status, the `ConsumerBuffer` may enter
	 * an infinite wait state after consuming all available data. While this undesired behavior
	 * is expected in such cases, proper handling of the buffer status can prevent it.
	 *
	 * The class is designed to operate in a multi-threaded environment, including detached threads.
	 * To ensure safe resource management, it is recommended to work with `std::shared_ptr` when
	 * interacting with `AsyncBuffer`. The `operator<<` methods transparently accept instances
	 * of `Buffer` or `Buffer::DataType` to append data seamlessly.
	 *
	 * Note: Any operations performed on a buffer with a status of `Error` will be ignored,
	 * except for updating the buffer's status.
	 */
	class STORMBYTE_PUBLIC AsyncBuffer {
		friend class ConsumerBuffer;															///< Friend class ConsumerBuffer.
		friend class ProducerBuffer;															///< Friend class ProducerBuffer.

		public:
			using ExpectedDataType = StormByte::Expected<Buffer::DataType, BufferException>;	///< The type of the expected data.

			/**
			 * @enum Status
			 * @brief Defines the status of the buffer during producer/consumer operations.
			 */
			enum class Status {
				Ready, 																			///< Indicates the buffer is OK and ready for operations.
				EoF,   																			///< Indicates the buffer has reached the end of the file/data.
				Error  																			///< Indicates the buffer has encountered an error.
			};

			/**
			 * @brief Constructs an empty `AsyncBuffer` instance.
			 */
			AsyncBuffer() noexcept;

			/**
			 * @brief Copy constructor.
			 * @param other `AsyncBuffer` instance to copy from.
			 */
			AsyncBuffer(const AsyncBuffer& other) 												= default;

			/**
			 * @brief Move constructor.
			 * @param other `AsyncBuffer` instance to move from.
			 */
			AsyncBuffer(AsyncBuffer&& other) noexcept 											= default;

			/**
			 * @brief Destructor.
			 */
			virtual ~AsyncBuffer() noexcept 													= default;

			/**
			 * @brief Copy assignment operator.
			 * @param other `AsyncBuffer` instance to copy from.
			 * @return Reference to the updated `AsyncBuffer` instance.
			 */
			AsyncBuffer& operator=(const AsyncBuffer& other) 									= default;

			/**
			 * @brief Move assignment operator.
			 * @param other `AsyncBuffer` instance to move from.
			 * @return Reference to the updated `AsyncBuffer` instance.
			 */
			AsyncBuffer& operator=(AsyncBuffer&& other) noexcept 								= default;

			/**
			 * @brief Retrieves the size of the buffer.
			 * @return Size of the buffer.
			 */
			constexpr size_t 																	Size() const noexcept {
				return m_internal_buffer->buffer->Size();
			}

			/**
			 * @brief Retrieves the current status of the buffer.
			 * @return The current `Status` of the buffer.
			 */
			AsyncBuffer::Status 																Status() const noexcept;

			/**
			 * @brief Sets the status of the buffer.
			 * @param status The new `Status` to assign to the buffer.
			 * @return Reference to the updated `AsyncBuffer` instance.
			 */
			AsyncBuffer& 																		operator<<(const enum Status& status);

			/**
			 * @brief Creates a producer buffer linked to the current buffer.
			 * @return `ProducerBuffer` instance linked to the current buffer.
			 */
			std::shared_ptr<ProducerBuffer>														Producer() const noexcept;

			/**
			 * @brief Creates a consumer buffer linked to the current buffer.
			 * @return `ConsumerBuffer` instance linked to the current buffer.
			 */
			std::shared_ptr<ConsumerBuffer> 													Consumer() const noexcept;

		protected:
			/**
			 * @struct InternalBuffer
			 * @brief Internal structure that encapsulates the buffer and its status.
			 */
			struct InternalBuffer {
				std::shared_ptr<Buffer> buffer; 												///< The buffer itself.
				std::atomic<enum AsyncBuffer::Status> status; 									///< Atomic status of the buffer.
			};
			std::shared_ptr<InternalBuffer> m_internal_buffer; 									///< Internal buffer and its status.

			/**
			 * @brief Retrieves the string representation of the buffer status.
			 * @return String representation of the buffer status.
			 */
			constexpr std::string StatusString() const noexcept {
				switch(m_internal_buffer->status.load()) {
					case AsyncBuffer::Status::Ready:	return "Ready";
					case AsyncBuffer::Status::EoF: 		return "EoF";
					case AsyncBuffer::Status::Error: 	return "Error";
				}
			}
	};
}

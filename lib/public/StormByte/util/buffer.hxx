#pragma once

#include <StormByte/visibility.h>

#include <cstddef>

/**
 * @namespace Util
 * @brief Contains utility classes and functions.
 */
namespace StormByte::Util {
	class STORMBYTE_PUBLIC Buffer {
		public:
			/**
			 * Default constructor
			 */
			Buffer() noexcept;

			/**
			 * Copy constructor
			 * @param value value to set
			 */
			Buffer(const Buffer& other);

			/**
			 * Move constructor
			 * @param value value to set
			 */
			Buffer(Buffer&& other) noexcept;

			/**
			 * Copy assignment operator
			 * @param value value to set
			 */
			Buffer& operator=(const Buffer& other);

			/**
			 * Move assignment operator
			 * @param value value to set
			 */
			Buffer& operator=(Buffer&& other) noexcept;

			/**
			 * Destructor
			 */
			~Buffer() noexcept;

			/**
			 * Concatenate two buffers
			 * @param other buffer to concatenate
			 * @return concatenated buffer
			 */
			Buffer 												operator+(const Buffer& other) const;

			/**
			 * Concatenate two buffers
			 * @param other buffer to concatenate
			 * @return concatenated buffer
			 */
			Buffer& 											operator+=(const Buffer& other);

			/**
			 * Append a value to the buffer
			 * @param data data to append
			 */
			void 												Append(const char* data, const std::size_t& length);

			/**
			 * Clears buffer
			 */
			void 												Clear();

			/**
			 * Retrieves the stored value
			 * @return stored value
			 */
			const char* 										Data() const;

			/**
			 * Retrieves the length of the buffer
			 * @return length of the buffer
			 */
			const std::size_t& 									Length() const;

		private:
			mutable char* m_buffer;								///< Stored value
			mutable std::size_t m_length;						///< Length of the buffer
	};
}
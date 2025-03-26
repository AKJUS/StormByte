#pragma once

#include <mutex>

/**
 * @namespace StormByte
 * @brief Main namespace for the StormByte library.
 *
 * The `StormByte` namespace serves as the root for all components and utilities in the StormByte library.
 * It provides foundational classes and tools for building robust, thread-safe, and efficient applications.
 */
namespace StormByte {
	template<typename T>
	class Mutexed {
		public:
			/**
			 * @brief Default constructor.
			 */
			Mutexed() noexcept = default;
			
			/**
			 * @brief Constructor with value.
			 * @param value The value to set.
			 */
			Mutexed(const T& value) noexcept : m_value(value) {}

			/**
			 * @brief Copy constructor.
			 * @param other The other Mutexed object to copy.
			 */
			Mutexed(const Mutexed<T>& other) noexcept {
				std::lock_guard<std::mutex> lock(m_mutex);
				std::lock_guard<std::mutex> lock_other(other.m_mutex);
				m_value = other.m_value;
			}

			/**
			 * @brief Move constructor.
			 * @param other The other Mutexed object to move.
			 */
			Mutexed(Mutexed<T>&& other) noexcept {
				std::lock_guard<std::mutex> lock(m_mutex);
				std::lock_guard<std::mutex> lock_other(other.m_mutex);
				m_value = std::move(other.m_value);
			}

			/**
			 * @brief Copy assignment operator.
			 * @param other The other Mutexed object to assign.
			 * @return Reference to this object.
			 */
			Mutexed<T>& operator=(const Mutexed<T>& other) noexcept {
				if (this != &other) {
					std::lock_guard<std::mutex> lock(m_mutex);
					std::lock_guard<std::mutex> lock_other(other.m_mutex);
					m_value = other.m_value;
				}
				return *this;
			}

			/**
			 * @brief Move assignment operator.
			 * @param other The other Mutexed object to assign.
			 * @return Reference to this object.
			 */
			Mutexed<T>& operator=(Mutexed<T>&& other) noexcept {
				if (this != &other) {
					std::lock_guard<std::mutex> lock(m_mutex);
					std::lock_guard<std::mutex> lock_other(other.m_mutex);
					m_value = std::move(other.m_value);
				}
				return *this;
			}

			/**
			 * @brief Assignment operator.
			 * @param value The value to set.
			 * @return Reference to this object.
			 */
			virtual Mutexed<T>& operator=(const T& value) noexcept {
				std::lock_guard<std::mutex> lock(m_mutex);
				m_value = value;
				return *this;
			}

			/**
			 * @brief Assignment move operator.
			 * @param value The value to set.
			 * @return Reference to this object.
			 */
			virtual Mutexed<T>& operator=(T&& value) noexcept {
				std::lock_guard<std::mutex> lock(m_mutex);
				m_value = std::move(value);
				return *this;
			}

			/**
			 * @brief Destructor.
			 */
			virtual ~Mutexed() noexcept = default;

			/**
			 * @brief Spaceship operator for comparison.
			 * @param other The other Mutexed object to compare.
			 * @return Strong ordering result.
			 */
			std::strong_ordering operator<=>(const Mutexed& other) const noexcept {
				// Lock both mutexes for thread-safe comparison
				std::lock_guard<std::mutex> lock_this(m_mutex);
				std::lock_guard<std::mutex> lock_other(other.m_mutex);
				
				// Compare values manually to return strong ordering
				if (m_value == other.m_value) {
					return std::strong_ordering::equal;
				} else if (m_value < other.m_value) {
					return std::strong_ordering::less;
				} else {
					return std::strong_ordering::greater;
				}
			}

			/**
			 * @brief Inequality operator.
			 * @param other The other Mutexed object to compare.
			 * @return True if not equal, false otherwise.
			 */
			bool operator!=(const Mutexed& other) const noexcept {
				return operator<=>(other) != std::strong_ordering::equal;
			}

			/**
			 * @brief Dereference operator (use with explicit locking).
			 * @return Reference to the value.
			 */
			virtual T& operator*() noexcept {
				return m_value;
			}

			/**
			 * @brief Dereference operator (use with explicit locking).
			 * @return Const reference to the value.
			 */
			virtual const T& operator*() const noexcept {
				return m_value;
			}

			/**
			 * @brief Pointer dereference operator (use with explicit locking).
			 * @return Pointer to the value.
			 */
			virtual T* operator->() noexcept {
				return &m_value;
			}

			/**
			 * @brief Pointer dereference operator (use with explicit locking).
			 * @return Const pointer to the value.
			 */
			virtual const T* operator->() const noexcept {
				return &m_value;
			}

			/**
			 * @brief Locks the mutex.
			 */
			void Lock() const {
				m_mutex.lock();
			}

			/**
			 * @brief Unlocks the mutex.
			 */
			void Unlock() const {
				m_mutex.unlock();
			}

		protected:
			T m_value; ///< Value
			mutable std::mutex m_mutex; ///< Mutex
	};
}
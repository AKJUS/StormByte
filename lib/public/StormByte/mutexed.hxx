#pragma once

#include <mutex>

/**
 * @namespace StormByte
 * @brief Main namespace for the StormByte library and components
 */
namespace StormByte {
	template<typename T>
	class Mutexed {
		public:
			/**
			 * Default constructor
			 */
			Mutexed() noexcept 							= default;
			
			/**
			 * Constructor with value
			 * @param value value to set
			 */
			Mutexed(const T& value) noexcept: m_value(value) {}

			/**
			 * Copy constructor
			 * @param value value to set
			 */
			Mutexed(const Mutexed<T>& other) noexcept {
				std::lock_guard<std::mutex> lock(m_mutex);
				std::lock_guard<std::mutex> lock_other(other.m_mutex);
				m_value = other.m_value;
			}

			/**
			 * Move constructor
			 * @param value value to set
			 */
			Mutexed(Mutexed<T>&& other) noexcept {
				std::lock_guard<std::mutex> lock(m_mutex);
				std::lock_guard<std::mutex> lock_other(other.m_mutex);
				m_value = std::move(other.m_value);
			}

			/**
			 * Copy assignment operator
			 * @param value value to set
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
			 * Move assignment operator
			 * @param value value to set
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
			 * Assignment operator
			 * @param value value to set
			 */
			virtual Mutexed<T>& operator=(const T& value) noexcept {
				std::lock_guard<std::mutex> lock(m_mutex);
				m_value = value;
				return *this;
			}

			/**
			 * Assignment move operator
			 * @param value value to set
			 */
			virtual Mutexed<T>& operator=(T&& value) noexcept {
				std::lock_guard<std::mutex> lock(m_mutex);
				m_value = std::move(value);
				return *this;
			}

			/**
			 * Addition operator
			 * @param value value to add
			 * @return reference to this
			 */
			virtual ~Mutexed() noexcept = default;

			/**
			 * Spaceship operator
			 * @param other other value to compare
			 * @return strong ordering
			 */
			std::strong_ordering 											operator<=>(const Mutexed& other) const noexcept {
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
			 * InEquality operator
			 * @param other other value to compare
			 * @return false if equal
			 */
			bool 															operator!=(const Mutexed& other) const noexcept {
				return operator<=>(other) != std::strong_ordering::equal;
			}

			/**
			 * Dereference operator (use with explicit locking)
			 * @return value
			 */
			virtual T& 														operator*() noexcept {
				return m_value;
			}

			/**
			 * Dereference operator (use with explicit locking)
			 * @return value
			 */
			virtual const T& 												operator*() const noexcept {
				return m_value;
			}

			/**
			 * Dereference operator (use with explicit locking)
			 * @return value
			 */
			virtual T* 														operator->() noexcept {
				return &m_value;
			}

			/**
			 * Dereference operator (use with explicit locking)
			 * @return value
			 */
			virtual const T* 												operator->() const noexcept {
				return &m_value;
			}

			/**
			 * Locks the mutex
			 */
			void 															Lock() const {
				m_mutex.lock();
			}

			/**
			 * Unlocks the mutex
			 */
			void 															Unlock() const {
				m_mutex.unlock();
			}

		protected:
			T 					m_value;									///< Value
			mutable std::mutex 	m_mutex;									///< Mutex
	};
}
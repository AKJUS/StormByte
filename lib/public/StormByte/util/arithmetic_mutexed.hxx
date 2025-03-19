#pragma once

#include <StormByte/visibility.h>
#include <StormByte/util/mutexed.hxx>

/**
 * @namespace Util
 * @brief Contains utility classes and functions.
 */
namespace StormByte::Util {
	template<typename T>
	class STORMBYTE_PUBLIC ArithmeticMutexed final: public Mutexed<T> {
		public:
			/**
			 * Default constructor
			 */
			ArithmeticMutexed() noexcept											= default;
			
			/**
			 * Constructor with value
			 * @param value value to set
			 */
			ArithmeticMutexed(const T& value) noexcept {
				std::lock_guard<std::mutex> lock(this->m_mutex);
				this->m_value = value;
			}

			/**
			 * Constructor with value
			 * @param value value to set
			 */
			ArithmeticMutexed(T&& value) noexcept {
				std::lock_guard<std::mutex> lock(this->m_mutex);
				this->m_value = std::move(value);
			}

			/**
			 * Copy constructor
			 * @param value value to set
			 */
			ArithmeticMutexed(const ArithmeticMutexed& other) noexcept				= default;

			/**
			 * Move constructor
			 * @param value value to set
			 */
			ArithmeticMutexed(ArithmeticMutexed&& other) noexcept					= default;

			/**
			 * Copy assignment operator
			 * @param value value to set
			 */
			ArithmeticMutexed& operator=(const ArithmeticMutexed& other) noexcept	= default;

			/**
			 * Move assignment operator
			 * @param value value to set
			 */
			ArithmeticMutexed& operator=(ArithmeticMutexed&& other) noexcept		= default;

			/**
			 * Assignment operator
			 * @param value value to set
			 */
			ArithmeticMutexed& operator=(const T& value) noexcept override {
				std::lock_guard<std::mutex> lock(this->m_mutex);
				this->m_value = value;
				return *this;
			}

			/**
			 * Assignment move operator
			 * @param value value to set
			 */
			ArithmeticMutexed& operator=(T&& value) noexcept override {
				std::lock_guard<std::mutex> lock(this->m_mutex);
				this->m_value = std::move(value);
				return *this;
			}

			/**
			 * Spaceship operator
			 * @param other other value to compare
			 * @return strong ordering
			 */
			std::strong_ordering operator<=>(const ArithmeticMutexed& other) const noexcept {
				return Mutexed<T>::operator<=>(other);
			}

			/**
			 * InEquality operator
			 * @param other other value to compare
			 * @return false if equal
			 */
			bool 										operator!=(const ArithmeticMutexed& other) const noexcept {
				return operator<=>(other) != std::strong_ordering::equal;
			}

			/**
			 * Addition operator
			 * @param value value to add
			 * @return reference to this
			 */
			ArithmeticMutexed operator+(const ArithmeticMutexed& value) const noexcept {
				std::lock_guard<std::mutex> lock(this->m_mutex);
				std::lock_guard<std::mutex> lock_other(value.m_mutex);
				return ArithmeticMutexed(this->m_value + value.m_value);
			}

			/**
			 * Addition operator
			 * @param value value to add
			 * @return reference to this
			 */
			ArithmeticMutexed operator+(ArithmeticMutexed&& value) const noexcept {
				std::lock_guard<std::mutex> lock(this->m_mutex);
				std::lock_guard<std::mutex> lock_other(value.m_mutex);
				return ArithmeticMutexed(this->m_value + std::move(value.m_value));
			}

			/**
			 * Addition operator with assignment
			 * @param value value to add
			 * @return reference to this
			 */
			ArithmeticMutexed& operator+=(ArithmeticMutexed& value) noexcept {
				std::lock_guard<std::mutex> lock(this->m_mutex);
				std::lock_guard<std::mutex> lock_other(value.m_mutex);
				this->m_value += value.m_value;
				return *this;
			}

			/**
			 * Addition operator with assignment
			 * @param value value to add
			 * @return reference to this
			 */
			ArithmeticMutexed& operator+=(ArithmeticMutexed&& value) noexcept {
				std::lock_guard<std::mutex> lock(this->m_mutex);
				std::lock_guard<std::mutex> lock_other(value.m_mutex);
				this->m_value += std::move(value.m_value);
				return *this;
			}

			/**
			 * Addition operator
			 * @param value value to add
			 * @return reference to this
			 */
			T operator+(const T& value) const noexcept {
				std::lock_guard<std::mutex> lock(this->m_mutex);
				return this->m_value + value;
			}
		
			/**
			 * Addition operator
			 * @param value value to add
			 * @return reference to this
			 */
			T operator+(T&& value) const noexcept {
				std::lock_guard<std::mutex> lock(this->m_mutex);
				return this->m_value + std::move(value);
			}

			/**
			 * Addition operator with assignment
			 * @param value value to add
			 * @return reference to this
			 */
			T& operator+=(T& value) noexcept {
				std::lock_guard<std::mutex> lock(this->m_mutex);
				this->m_value += value;
				return this->m_value;
			}

			/**
			 * Addition operator with assignment
			 * @param value value to add
			 * @return reference to this
			 */
			T& operator+=(T&& value) noexcept {
				std::lock_guard<std::mutex> lock(this->m_mutex);
				this->m_value += std::move(value);
				return this->m_value;
			}
	};
}
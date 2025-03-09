#pragma once

#include <mutex>

/**
 * @namespace Templates
 * @brief All the utility templates
 */
namespace StormByte::Util::Templates {
	/**
	 * @class Mutexer
	 * @brief Mutexed variable
	 * @tparam T type of the variable
	 */
	template<typename T>
	class Mutexer final {
		public:
			/**
			 * Constructor
			 * @param value value
			 */
			Mutexer(const T& value): m_value(value) {}

			/**
			 * Constructor
			 * @param value value
			 */
			Mutexer(T&& value): m_value(std::move(value)) {}

			/**
			 * Copy constructor
			 * @param other other
			 */
			Mutexer& operator=(const T& value) {
				std::lock_guard<std::mutex> lock(m_mutex);
				m_value = value;
				return *this;
			}

			/**
			 * Move constructor
			 * @param other other
			 */
			Mutexer& operator=(T&& value) {
				std::lock_guard<std::mutex> lock(m_mutex);
				m_value = std::move(value);
				return *this;
			}

			/**
			 * Destructor
			 */
			Mutexer& operator=(const T& value) {
				std::lock_guard<std::mutex> lock(m_mutex);
				m_value = value;
				return *this;
			}

			/**
			 * Destructor
			 */
			Mutexer& operator=(T&& value) {
				std::lock_guard<std::mutex> lock(m_mutex);
				m_value = std::move(value);
				return *this;
			}

			/**
			 * Destructor
			 */
			~Mutexer() 							= default;

			/**
			 * Gets the value
			 * @return value
			 */
			operator T() const {
				std::lock_guard<std::mutex> lock(m_mutex);
				return m_value;
			}

			/**
			 * Gets the value
			 * @return value
			 */
			T operator->() const {
				std::lock_guard<std::mutex> lock(m_mutex);
				return m_value;
			}

			/**
			 * Gets the value
			 * @return value
			 */
			const T& operator->() const {
				std::lock_guard<std::mutex> lock(m_mutex);
				return m_value;
			}

		private:
			T m_value;							///< Value
			std::mutex m_mutex;					///< Mutex
	};
}
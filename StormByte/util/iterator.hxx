#include <StormByte/visibility.h>

/**
 * @namespace Util
 * @brief All the utility classes and functions
 */
namespace StormByte::Util {
	/**
	 * @class Iterator
	 * @brief iterator to Storage
	 */
	template<typename Storage> class STORMBYTE_PUBLIC Iterator {
		public:
			/**
			 * Constructor
			 */
			constexpr Iterator() noexcept						= default;

			/**
			 * Copy constructor
			 */
			constexpr Iterator(const Iterator&)					= default;

			/**
			 * Move constructor
			 */
			constexpr Iterator(Iterator&&) noexcept				= default;

			/**
			 * Assignment operator
			 */
			constexpr Iterator& operator=(const Iterator&)		= default;

			/**
			 * Move assignment operator
			 */
			constexpr Iterator& operator=(Iterator&&) noexcept	= default;

			/**
			 * Destructor
			 */
			constexpr ~Iterator() noexcept 						= default;

			/**
			 * Pre increment operator
			 */
			constexpr Iterator& 								operator++() noexcept {
				++m_it;
				return *this;
			}

			/**
			 * Post increment operator
			 */
			constexpr Iterator 									operator++(int) noexcept {
				Iterator copy = *this;
				++m_it;
				return copy;
			}

			/**
			 * Pre decrement operator
			 */
			constexpr Iterator& 								operator--() noexcept {
				--m_it;
				return *this;
			}

			/**
			 * Post decrement operator
			 */
			constexpr Iterator 									operator--(int) noexcept {
				Iterator copy = *this;
				--m_it;
				return copy;
			}

			/**
			 * Equal comparison operator
			 */
			constexpr bool 										operator==(const Iterator& other) const noexcept {
				return m_it == other.m_it;
			}

			/**
			 * Different comparison operator
			 */
			constexpr bool 										operator!=(const Iterator& other) const noexcept {
				return m_it != other.m_it;
			}

			/**
			 * Indirect operator
			 */
			constexpr Storage::value_type* 						operator->() noexcept {
				return &*m_it;
			}

			/**
			 * Access operator
			 */
			constexpr Storage::value_type& 						operator*() noexcept {
				return *m_it;
			}

			/**
			 * Gets an iterator pointing to first element
			 * @param storage storage
			 * @return iterator
			 */
			constexpr static Iterator 							Begin(Storage& storage) noexcept {
				Iterator it;
				it.m_it = storage.begin();
				return it;
			}

			/**
			 * Gets an iterator pointing to past last element
			 * @param storage storage
			 * @return iterator
			 */
			constexpr static Iterator 							End(Storage& storage) noexcept {
				Iterator it;
				it.m_it = storage.end();
				return it;
			}

		private:
			/**
			 * Internal iterator to original data
			 */
			Storage::iterator m_it;	///< Internal iterator to original data
	};

	/**
	 * @class ConstIterator
	 * @brief Constant iterator to Storage
	 */
	template<typename Storage> class STORMBYTE_PUBLIC ConstIterator {
		public:
			/**
			 * Constructor
			 */
			constexpr ConstIterator() noexcept								= default;

			/**
			 * Copy constructor
			 */
			constexpr ConstIterator(const ConstIterator&)					= default;

			/**
			 * Move constructor
			 */
			constexpr ConstIterator(ConstIterator&&) noexcept				= default;

			/**
			 * Assignment operator
			 */
			constexpr ConstIterator& operator=(const ConstIterator&)		= default;

			/**
			 * Move assignment operator
			 */
			constexpr ConstIterator& operator=(ConstIterator&&) noexcept	= default;

			/**
			 * Destructor
			 */
			constexpr ~ConstIterator() noexcept 							= default;

			/**
			 * Pre increment operator
			 */
			constexpr ConstIterator& 										operator++() noexcept {
				++m_it;
				return *this;
			}

			/**
			 * Post increment operator
			 */
			constexpr ConstIterator 										operator++(int) noexcept {
				ConstIterator copy = *this;
				++m_it;
				return copy;
			}

			/**
			 * Pre decrement operator
			 */
			constexpr ConstIterator& 										operator--() noexcept {
				--m_it;
				return *this;
			}

			/**
			 * Post decrement operator
			 */
			constexpr ConstIterator 										operator--(int) noexcept {
				ConstIterator copy = *this;
				--m_it;
				return copy;
			}

			/**
			 * Equal comparison operator
			 */
			constexpr bool 													operator==(const ConstIterator& other) const noexcept {
				return m_it == other.m_it;
			}

			/**
			 * Different comparison operator
			 */
			constexpr bool 													operator!=(const ConstIterator& other) const noexcept {
				return m_it != other.m_it;
			}

			/**
			 * Indirect operator
			 */
			constexpr const Storage::value_type* 							operator->() const noexcept {
				return &*m_it;
			}

			/**
			 * Access operator
			 */
			constexpr const Storage::value_type& 							operator*() const noexcept {
				return *m_it;
			}

			/**
			 * Gets an iterator pointing to first element
			 * @param storage storage
			 * @return const iterator
			 */
			constexpr static ConstIterator 									Begin(const Storage& storage) noexcept {
				ConstIterator it;
				it.m_it = storage.cbegin();
				return it;
			}

			/**
			 * Gets an iterator pointing to past last element
			 * @param storage storage
			 * @return const iterator
			 */
			constexpr static ConstIterator 									End(const Storage& storage) noexcept {
				ConstIterator it;
				it.m_it = storage.cend();
				return it;
			}

		private:
			/**
			 * Internal iterator to original data
			 */
			Storage::const_iterator m_it;	///< Internal iterator to original data
	};
}
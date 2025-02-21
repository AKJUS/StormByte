#include <StormByte/visibility.h>
#include <StormByte/compatibility.h>

#include <string>

namespace StormByte {
	/**
	 * @class Exception
	 * @brief Exception base class for all the StormByte library
	 * 
	 * The purpose of this class is to handle a generic exception for all the StormByte library
	 * and also to cover the Windows/Linux differences in exception handling, specially in passing
	 * std::string across library boundaries fixed by handling the const char* memory management
	 * without inheriting from std::exception so we avoid the need to expose the std::exception
	 */
	class STORMBYTE_PUBLIC Exception {
		public:
			/**
			 * Constructor
			 * @param message message
			 */
			Exception(const std::string& message);
			/**
			 * Copy constructor
			 */
			Exception(const Exception&);
			/**
			 * Move constructor
			 */
			Exception(Exception&&) noexcept;
			/**
			 * Assignment operator
			 */
			Exception& operator=(const Exception&);
			/**
			 * Move operator
			 */
			Exception& operator=(Exception&&) noexcept;
			/**
			 * Destructor
			 */
			virtual ~Exception() noexcept;
			
			/**
			 * Gets the exception message
			 * @return message
			 */
			virtual const char* what() const noexcept;

		private:
			/**
			 * Exception message
			 */
			const char* m_what;

			/**
			 * Copies a string
			 * @param str string
			 * @return copied string
			 */
			const char*			copy_str(const char* str) noexcept;
			/**
			 * Frees a string
			 */
			void 				free_str() noexcept;
	};
}
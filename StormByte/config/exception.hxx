#pragma once

#include <StormByte/config/item.hxx>
#include <StormByte/exception.hxx>

/**
 * @namespace StormByte::Config
 * @brief All the classes for handling configuration files and items
 */
namespace StormByte::Config {
	/**
	 * @class Exception
	 * @brief Base class for Config exceptions
	 */
	class STORMBYTE_PUBLIC Exception: public StormByte::Exception {
		public:
			/**
			 * Constructor
			 * @param reason
			 */
			Exception(const std::string& reason);
			/**
			 * Copy constructor
			 */
			Exception(const Exception&)					= default;
			/**
			 * Move constructor
			 */
			Exception(Exception&&) noexcept				= default;
			/**
			 * Assignment operator
			 */
			Exception& operator=(const Exception&)		= default;
			/**
			 * Move assignment operator
			 */
			Exception& operator=(Exception&&) noexcept	= default;
			/**
			 * Destructor
			 */
			~Exception() noexcept						= default;		
	};

	/**
	 * @class WrongValueTypeConversion
	 * @brief Exception thrown when value type requested is not correct
	 */
	class STORMBYTE_PUBLIC WrongValueTypeConversion final: public Exception {
		public:
			/**
			 * Constructor
			 * @param item NamedItem trying to be converted
			 * @param method method tried to use for converting
			 */
			WrongValueTypeConversion(const Item& item, const std::string& method);
			/**
			 * Copy constructor
			 */
			WrongValueTypeConversion(const WrongValueTypeConversion&)				= default;
			/**
			 * Move constructor
			 */
			WrongValueTypeConversion(WrongValueTypeConversion&&)					= default;
			/**
			 * Assignment operator
			 */
			WrongValueTypeConversion& operator=(const WrongValueTypeConversion&)	= default;
			/**
			 * Move assignment operator
			 */
			WrongValueTypeConversion& operator=(WrongValueTypeConversion&&)			= default;
			/**
			 * Destructor
			 */
			~WrongValueTypeConversion() noexcept override							= default;
	};

	/**
	 * @class ValueFailure
	 * @brief Exception thrown when setting a value to the wrong type
	 */
	class STORMBYTE_PUBLIC ValueFailure final: public Exception {
		public:
			/**
			 * Constructor
			 * @param item NamedItem trying to set value
			 * @param type Type of the value
			 */
			ValueFailure(const Item& item, const Item::Type& type);
			/**
			 * Copy constructor
			 */
			ValueFailure(const ValueFailure&)				= default;
			/**
			 * Move constructor
			 */
			ValueFailure(ValueFailure&&)					= default;
			/**
			 * Assignment operator
			 */
			ValueFailure& operator=(const ValueFailure&)	= default;
			/**
			 * Move assignment operator
			 */
			ValueFailure& operator=(ValueFailure&&)			= default;
			/**
			 * Destructor
			 */
			~ValueFailure() noexcept override				= default;
	};

	/**
	 * @class InvalidName
	 * @brief Exception thrown when adding to a group an item with invalid name
	 */
	class STORMBYTE_PUBLIC InvalidName final: public Exception {
		public:
			/**
			 * Constructor
			 * @param name name which was tried to use
			 */
			InvalidName(const std::string& name);
			/**
			 * Copy operator
			 */
			InvalidName(const InvalidName&)				= default;
			/**
			 * Move operator
			 */
			InvalidName(InvalidName&&)					= default;
			/**
			 * Assignment operator
			 */
			InvalidName& operator=(const InvalidName&)	= default;
			/**
			 * Move assignment operator
			 */
			InvalidName& operator=(InvalidName&&)		= default;
			/**
			 * Destructor
			 */
			~InvalidName() noexcept override			= default;
	};

	/**
	 * @class ParseError
	 * @brief Thrown when a parser error is found when parsing File
	 */
	class STORMBYTE_PUBLIC ParseError final: public Exception {
		public:
			/**
			 * Constructor
			 * @param error the error string to show
			 */
			ParseError(const std::string& error);
			/**
			 * Copy constructor
			 */
			ParseError(const ParseError&)				= default;
			/**
			 * Move constructor
			 */
			ParseError(ParseError&&)					= default;
			/**
			 * Assignment operator
			 */
			ParseError& operator=(const ParseError&)	= default;
			/**
			 * Move assignment operator
			 */
			ParseError& operator=(ParseError&&)			= default;
			/**
			 * Destructor
			 */
			~ParseError() noexcept override				= default;
	};

	/**
	 * @class NamedItemNotFound
	 * @brief Exception thrown lookup path fails
	 */
	class STORMBYTE_PUBLIC NamedItemNotFound final: public Exception {
		public:
			/**
			 * Constructor
			 * @param name item name which was not found
			 */
			NamedItemNotFound(const std::string& name);
			/**
			 * Copy constructor
			 */
			NamedItemNotFound(const NamedItemNotFound&)				= default;
			/**
			 * Move constructor
			 */
			NamedItemNotFound(NamedItemNotFound&&)					= default;
			/**
			 * Assignment operator
			 */
			NamedItemNotFound& operator=(const NamedItemNotFound&)	= default;
			/**
			 * Move assignment operator
			 */
			NamedItemNotFound& operator=(NamedItemNotFound&)			= default;
			/**
			 * Destructor
			 */
			~NamedItemNotFound() noexcept override				= default;
	};

	/**
	 * @class NamedItemNameAlreadyExists
	 * @brief Exception thrown when adding an item which name already exists
	 */
	class STORMBYTE_PUBLIC NamedItemNameAlreadyExists final: public Exception {
		public:
			/**
			 * Constructor
			 * @param name item name which was not found
			 */
			NamedItemNameAlreadyExists(const std::string& name);
			/**
			 * Copy constructor
			 */
			NamedItemNameAlreadyExists(const NamedItemNameAlreadyExists&)				= default;
			/**
			 * Move constructor
			 */
			NamedItemNameAlreadyExists(NamedItemNameAlreadyExists&&)					= default;
			/**
			 * Assignment operator
			 */
			NamedItemNameAlreadyExists& operator=(const NamedItemNameAlreadyExists&)	= default;
			/**
			 * Move assignment operator
			 */
			NamedItemNameAlreadyExists& operator=(NamedItemNameAlreadyExists&)		= default;
			/**
			 * Destructor
			 */
			~NamedItemNameAlreadyExists() noexcept override						= default;
	};
}
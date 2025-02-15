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
			 * @param item Item trying to be converted
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
			 * @param item Item trying to set value
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
	 * @class ItemNotFound
	 * @brief Exception thrown lookup path fails
	 */
	class STORMBYTE_PUBLIC ItemNotFound final: public Exception {
		public:
			/**
			 * Constructor
			 * @param name item name which was not found
			 */
			ItemNotFound(const std::string& name);
			/**
			 * Copy constructor
			 */
			ItemNotFound(const ItemNotFound&)				= default;
			/**
			 * Move constructor
			 */
			ItemNotFound(ItemNotFound&&)					= default;
			/**
			 * Assignment operator
			 */
			ItemNotFound& operator=(const ItemNotFound&)	= default;
			/**
			 * Move assignment operator
			 */
			ItemNotFound& operator=(ItemNotFound&)			= default;
			/**
			 * Destructor
			 */
			~ItemNotFound() noexcept override				= default;
	};

	/**
	 * @class ItemNameAlreadyExists
	 * @brief Exception thrown when adding an item which name already exists
	 */
	class STORMBYTE_PUBLIC ItemNameAlreadyExists final: public Exception {
		public:
			/**
			 * Constructor
			 * @param name item name which was not found
			 */
			ItemNameAlreadyExists(const std::string& name);
			/**
			 * Copy constructor
			 */
			ItemNameAlreadyExists(const ItemNameAlreadyExists&)				= default;
			/**
			 * Move constructor
			 */
			ItemNameAlreadyExists(ItemNameAlreadyExists&&)					= default;
			/**
			 * Assignment operator
			 */
			ItemNameAlreadyExists& operator=(const ItemNameAlreadyExists&)	= default;
			/**
			 * Move assignment operator
			 */
			ItemNameAlreadyExists& operator=(ItemNameAlreadyExists&)		= default;
			/**
			 * Destructor
			 */
			~ItemNameAlreadyExists() noexcept override						= default;
	};
}
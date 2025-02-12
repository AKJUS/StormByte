#pragma once

#include <StormByte/config/item.hxx>
#include <StormByte/system/exception.hxx>

/**
 * @namespace StormByte::Config
 * @brief All the classes for handling configuration files and items
 */
namespace StormByte::Config {
	/**
	 * @class Exception
	 * @brief Base class for Config exceptions
	 */
	class STORMBYTE_PUBLIC Exception: public System::Exception {
		public:
			Exception(const std::string&);
			Exception(const Exception&)					= default;
			Exception(Exception&&) noexcept				= default;
			Exception& operator=(const Exception&)		= default;
			Exception& operator=(Exception&&) noexcept	= default;
			~Exception() noexcept						= default;		
	};

	/**
	 * @class WrongValueTypeConversion
	 * @brief Exception thrown when value type requested is not correct
	 */
	class STORMBYTE_PUBLIC WrongValueTypeConversion final: public Exception {
		public:
			WrongValueTypeConversion(const Item&, const std::string&);
			WrongValueTypeConversion(const WrongValueTypeConversion&)				= default;
			WrongValueTypeConversion& operator=(const WrongValueTypeConversion&)	= default;
			~WrongValueTypeConversion() noexcept override							= default;
	};

	/**
	 * @class ValueFailure
	 * @brief Exception thrown when setting a value to the wrong type
	 */
	class STORMBYTE_PUBLIC ValueFailure final: public Exception {
		public:
			ValueFailure(const Item&, const Item::Type&);
			ValueFailure(const ValueFailure&)				= default;
			ValueFailure& operator=(const ValueFailure&)	= default;
			~ValueFailure() noexcept override				= default;
	};

	/**
	 * @class InvalidName
	 * @brief Exception thrown when adding to a group an item with invalid name
	 */
	class STORMBYTE_PUBLIC InvalidName final: public Exception {
		public:
			InvalidName(const std::string&);
			InvalidName(const InvalidName&)				= default;
			InvalidName& operator=(const InvalidName&)	= default;
			~InvalidName() noexcept override			= default;
	};

	/**
	 * @class ParserError
	 * @brief Thrown when a parser error is found when parsing File
	 */
	class STORMBYTE_PUBLIC ParseError final: public Exception {
		public:
			ParseError(const std::string&);
			ParseError(const ParseError&)				= default;
			ParseError& operator=(const ParseError&)	= default;
			~ParseError() noexcept override				= default;
	};

	/**
	 * @class ItemNotFound
	 * @brief Exception thrown lookup path fails
	 */
	class STORMBYTE_PUBLIC ItemNotFound final: public Exception {
		public:
			ItemNotFound(const std::string&);
			ItemNotFound(const ItemNotFound&)				= default;
			ItemNotFound& operator=(const ItemNotFound&)	= default;
			~ItemNotFound() noexcept override				= default;
	};
}
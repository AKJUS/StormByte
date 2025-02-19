#pragma once

#include <StormByte/database/sqlite/type.hxx>

#include <cstdint>
#include <string>
#include <variant>

/**
 * @namespace Database::SQLite
 * @brief All the classes for handling SQLite databases
 */
namespace StormByte::Database::SQLite {
	/**
	 * @class Result
	 * @brief Result class for SQLite
	 */
	class STORMBYTE_PUBLIC Result {
		public:
			/**
			 * Constructor
			 * @param value integer
			 */
			Result(const int64_t& value);

			/**
			 * Constructor
			 * @param value string
			 */
			Result(const std::string& value);

			/**
			 * Constructor
			 * @param value string
			 */
			Result(std::string&& value);

			/**
			 * Constructor
			 * @param ignored A nullptr which will be ignored
			 */
			Result(std::nullptr_t ignored);

			/**
			 * Constructor
			 * @param value double
			 */
			Result(const double& value);

			/**
			 * Constructor
			 * @param value double
			 */
			Result(double&& value);

			/**
			 * Copy Constructor
			 */
			Result(const Result&)							= default;

			/**
			 * Move Constructor
			 */
			Result(Result&&) noexcept						= default;

			/**
			 * Assignment operator
			 */
			Result& operator=(const Result&)				= default;

			/**
			 * Move operator
			 */
			Result& operator=(Result&&) noexcept			= default;

			/**
			 * Destructor
			 */
			~Result() noexcept								= default;

			/**
			 * Checks if the result is null
			 * @return true if the result is null
			 */
			virtual bool 							IsNull() const noexcept;

			/**
			 * Gets the value
			 * @return result value
			 */
			template<typename T> const T&			Value() const;

			#ifdef MSVC
			/**
			 * Gets the value
			 * @return result value
			 */
			template<> const int&					Value<int>() const;

			/**
			 * Gets the value
			 * @return result value
			 */
			template<> const int64_t&				Value<int64_t>() const;

			/**
			 * Gets the value
			 * @return result value
			 */
			template<> const bool&					Value<bool>() const;

			/**
			 * Gets the value
			 * @return result value
			 */
			template<> const double&				Value<double>() const;

			/**
			 * Gets the value
			 * @return result value
			 */
			template<> const std::string&			Value<std::string>() const;
			#endif

			/**
			 * Gets the type
			 * @return result type
			 */
			const Type& 							GetType() const noexcept;

		protected:
			/**
			 * Constructor
			 * @param type type
			 */
			Type m_type;

			/**
			 * Internal value
			 */
			std::variant<int64_t, std::string, double> m_value;

			/**
			 * Internal to mark conversion to bool
			 */
			mutable bool m_bool_conversion;

			/**
			 * Internal to mark conversion to int
			 */
			mutable int m_int_conversion;
	};
}

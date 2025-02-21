#pragma once

#include <StormByte/database/exception.hxx>

#include <cstdint>
#include <memory>
#include <string>
#include <variant>

/**
 * @namespace Database
 * @brief Contains classes and functions for database operations.
 */
namespace StormByte::Database {
	/**
	 * @class Value
	 * @brief Value class for databases (the common ones, every database could have its own implementation to add more types)
	 */
	class STORMBYTE_PUBLIC Value {
		public:
			/**
			 * Default constructor.
			 */
			constexpr Value():m_value(nullptr) {}

			/**
			 * Constructor for int
			 * @param value value
			 */
			constexpr Value(const int& value): m_value(value) {}

			/**
			 * Constructor for int64
			 * @param value value
			 */
			constexpr Value(const int64_t& value): m_value(value) {}

			/**
			 * Constructor for double
			 * @param value value
			 */
			constexpr Value(const double& value): m_value(value) {}

			/**
			 * Constructor for bool
			 * @param value value
			 */
			constexpr Value(bool value): m_value(value) {}

			/**
			 * Constructor for string
			 * @param value value
			 */
			constexpr Value(const std::string& value): m_value(value) {}

			/**
			 * Move constructor for string
			 * @param value value
			 */
			constexpr Value(std::string&& value): m_value(std::move(value)) {}

			/**
			 * Default copy constructor
			 */
			Value(const Value&)					= default;

			/**
			 * Default move constructor
			 */
			constexpr Value(Value&&) noexcept				= default;

			/**
			 * Default copy assignment operator
			 */
			Value& operator=(const Value&)		= default;

			/**
			 * Default move assignment operator
			 */
			#if not defined(__clang__) || defined(CLANG_19_OR_LATER)
			constexpr Value& operator=(Value&&)	noexcept 	= default;
			#else
			Value& operator=(Value&&) noexcept 				= default;
			#endif

			/**
			 * Assignment operator for int
			 * @param value value
			 * @return reference to this
			 */
			constexpr Value& operator=(const int& value) {
				m_value = value;
				return *this;
			}

			/**
			 * Assignment operator for int64
			 * @param value value
			 * @return reference to this
			 */
			constexpr Value& operator=(const int64_t& value) {
				m_value = value;
				return *this;
			}

			/**
			 * Assignment operator for double
			 * @param value value
			 * @return reference to this
			 */
			constexpr Value& operator=(const double& value) {
				m_value = value;
				return *this;
			}

			/**
			 * Assignment operator for bool
			 * @param value value
			 * @return reference to this
			 */
			constexpr Value& operator=(bool value) {
				m_value = value;
				return *this;
			}

			/**
			 * Assignment operator for string
			 * @param value value
			 * @return reference to this
			 */
			constexpr Value& operator=(const std::string& value) {
				m_value = value;
				return *this;
			}

			/**
			 * Move assignment operator for string
			 * @param value value
			 * @return reference to this
			 */
			constexpr Value& operator=(std::string&& value) {
				m_value = std::move(value);
				return *this;
			}

			/**
			 * Default destructor.
			 */
			virtual ~Value() noexcept						= default;

			/**
			 * Gets the value.
			 * @return pointer to const reference to value
			 */
			template<typename T> expected_type_cref(T, WrongValueType)		Get() throws(WrongValueType) const;

			#ifdef MSVC
			/**
			 * Gets the value.
			 * @return pointer to const reference to value
			 */
			template<> expected_type_cref(int, WrongValueType) Value::Get<int>() const;

			/**
			 * Gets the value.
			 * @return pointer to const reference to value
			 */
			template<> expected_type_cref(int64_t, WrongValueType) Value::Get<int64_t>() const;

			/**
			 * Gets the value.
			 * @return pointer to const reference to value
			 */
			template<> expected_type_cref(double, WrongValueType) Value::Get<double>() const;

			/**
			 * Gets the value.
			 * @return pointer to const reference to value
			 */
			template<> expected_type_cref(bool, WrongValueType)	Value::Get<bool>() const;

			/**
			 * Gets the value.
			 * @return pointer to const reference to value
			 */
			template<> expected_type_cref(std::string, WrongValueType) Value::Get<std::string>() const;
			#endif

			/**
			 * Is value of type T?
			 * @return true if value is of type T
			 */
			template<typename T> constexpr bool								Is() const noexcept {
				return std::holds_alternative<T>(m_value);
			}

			/**
			 * Is value null?
			 * @return true if value is null
			 */
			constexpr bool													IsNull() const noexcept {
				return std::holds_alternative<std::nullptr_t>(m_value);
			}

		protected:
			/**
			 * Storage for the value
			 */
			using Storage = std::variant<int, int64_t, bool, double, std::string, std::nullptr_t>; //< Storage for the value

			/**
			 * Value
			 */
			Storage m_value; //< Value
	};
}
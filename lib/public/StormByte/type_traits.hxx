#pragma once

#include <optional>
#include <type_traits>

/**
 * @namespace StormByte
 * @brief Main namespace for the StormByte library and components
 */
namespace StormByte {
	/**
	 * @brief Type traits for checking if a type is a container
	 * @tparam T Type to check
	 */
	template<typename T, typename _ = void>
	struct is_container: std::false_type {};

	/**
	 * @brief Type traits for checking if a type is a container
	 * @tparam T Type to check
	 */
	template<typename T>
	struct is_container<T, std::void_t<decltype(std::declval<T>().begin()), decltype(std::declval<T>().end()), typename T::value_type>> : std::true_type {};

	/**
	 * @brief Type traits for checking if a type is an optional
	 * @tparam T Type to check
	 */
	template<typename T, typename _ = void>
	struct is_optional : std::false_type {};

	/**
	 * @brief Type traits specialization for std::optional
	 * @tparam T Type to check
	 */
	template<typename T>
	struct is_optional<T, std::void_t<typename T::value_type>> 
		: std::is_same<T, std::optional<typename T::value_type>> {};


	/**
	 * @brief Type traits for checking if a type is a pair
	 * @tparam T Type to check
	 */
	template<typename T, typename _ = void>
	struct is_pair: std::false_type {};

	/**
	 * @brief Type traits for checking if a type is a pair
	 * @tparam T Type to check
	 */
	template<typename T>
	struct is_pair<T, std::void_t<
		decltype(std::declval<T>().first), 
		decltype(std::declval<T>().second)
	>> : std::true_type {};

	/**
	 * @brief Type traits for checking if a type is a reference
	 * @tparam T Type to check
	 */
	template <typename T>
	struct is_reference {
		static constexpr bool value = std::is_reference_v<T>;
	};
}
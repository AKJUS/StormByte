#pragma once

#include <type_traits>

namespace StormByte::Network::Data {
	template<typename T, typename _ = void>
	struct is_container : std::false_type {};

	template<typename T>
	struct is_container<T, std::void_t<decltype(std::declval<T>().begin()), decltype(std::declval<T>().end()), typename T::value_type>> : std::true_type {};

	template<typename T, typename _ = void>
	struct is_pair: std::false_type {};

	template<typename T>
	struct is_pair<T, std::void_t<
		decltype(std::declval<T>().first), 
		decltype(std::declval<T>().second)
	>> : std::true_type {};
}
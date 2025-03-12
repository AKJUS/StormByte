#pragma once

#include <expected>
#include <memory>

/**
 * @namespace StormByte
 * @brief Main namespace for the StormByte library and components
 */
namespace StormByte {
	template<typename T, class E>
	using Expected 		= std::expected<T, std::shared_ptr<E>>; 													///< Expected type alias
	template<typename E>
	constexpr auto Unexpected(std::shared_ptr<E> error_ptr) {
		return std::unexpected<std::shared_ptr<E>>(std::move(error_ptr));											///< Unexpected type alias
	}
	template<typename E>
	constexpr auto Unexpected(E&& error) {
		return std::unexpected<std::shared_ptr<std::decay_t<E>>>(std::make_shared<std::decay_t<E>>(std::forward<E>(error)));
	}
	template<typename E, typename... Args>
	constexpr auto Unexpected(Args&&... args) {
		return std::unexpected<std::shared_ptr<E>>(std::make_shared<E>(std::forward<Args>(args)...));				///< Unexpected type alias
	}
}
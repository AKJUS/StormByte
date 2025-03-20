#pragma once

#include <StormByte/type_traits.hxx>

#include <expected>
#include <memory>
#include <functional> // For std::reference_wrapper

/**
 * @namespace StormByte
 * @brief Main namespace for the StormByte library and components
 */
namespace StormByte {
	/**
	 * @brief Expected type with support for reference types
	 * @tparam T Expected type
	 * @tparam E Error type
	 */
	template <typename T, class E>
	using Expected = std::conditional_t<
		is_reference<T>::value,
		std::expected<std::reference_wrapper<std::remove_reference_t<T>>, std::shared_ptr<E>>,
		std::expected<T, std::shared_ptr<E>>
	>;

	/**
	 * @brief Creates an std::unexpected with a shared_ptr to the error
	 * @tparam E Error type
	 * @param error_ptr A shared pointer to the error
	 * @return unexpected object
	 */
	template <typename E>
	constexpr auto Unexpected(std::shared_ptr<E> error_ptr) {
		return std::unexpected<std::shared_ptr<E>>(std::move(error_ptr));
	}

	/**
	 * @brief Creates an std::unexpected with a new error instance
	 * @tparam E Error type
	 * @param error Error instance
	 * @return unexpected object
	 */
	template <typename E>
	constexpr auto Unexpected(E&& error) {
		return std::unexpected<std::shared_ptr<std::decay_t<E>>>(
			std::make_shared<std::decay_t<E>>(std::forward<E>(error))
		);
	}

	/**
	 * @brief Creates an std::unexpected with a new error instance, constructed with arguments
	 * @tparam E Error type
	 * @tparam Args Constructor arguments types
	 * @param args Constructor arguments for the error
	 * @return unexpected object
	 */
	template <typename E, typename... Args>
	constexpr auto Unexpected(Args&&... args) {
		return std::unexpected<std::shared_ptr<E>>(
			std::make_shared<E>(std::forward<Args>(args)...)
		);
	}
}
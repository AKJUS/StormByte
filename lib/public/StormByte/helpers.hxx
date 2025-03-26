#pragma once

#include <memory>
#include <type_traits>

namespace StormByte {
    // Forward declarations
    class Buffer;
    class AsyncBuffer;

    /**
     * @brief Wrapper for `operator<<` to support `std::shared_ptr` or `std::unique_ptr` as the left-hand parameter.
     * @tparam PtrType The type of the smart pointer (e.g., `std::shared_ptr` or `std::unique_ptr`).
     * @tparam T The type of the value to append.
     * @param ptr Smart pointer to an instance of a class derived from `Buffer` or `AsyncBuffer`.
     * @param value The value to append to the buffer.
     * @return Reference to the updated smart pointer.
     */
    template <typename PtrType, typename T,
        typename = std::enable_if_t<
            std::is_base_of_v<Buffer, typename PtrType::element_type> ||
            std::is_base_of_v<AsyncBuffer, typename PtrType::element_type>>>
    inline PtrType& operator<<(PtrType& ptr, const T& value) {
        if (ptr) {
            *ptr << value; // Forward the call to the existing operator<<
        }
        return ptr;
    }

    /**
     * @brief Wrapper for `operator<<` to support `std::shared_ptr` or `std::unique_ptr` as the left-hand parameter (move version).
     * @tparam PtrType The type of the smart pointer (e.g., `std::shared_ptr` or `std::unique_ptr`).
     * @tparam T The type of the value to append.
     * @param ptr Smart pointer to an instance of a class derived from `Buffer` or `AsyncBuffer`.
     * @param value The value to append to the buffer.
     * @return Reference to the updated smart pointer.
     */
    template <typename PtrType, typename T,
        typename = std::enable_if_t<
            std::is_base_of_v<Buffer, typename PtrType::element_type> ||
            std::is_base_of_v<AsyncBuffer, typename PtrType::element_type>>>
    inline PtrType& operator<<(PtrType& ptr, T&& value) {
        if (ptr) {
            *ptr << std::move(value); // Forward the call to the existing operator<<
        }
        return ptr;
    }
}
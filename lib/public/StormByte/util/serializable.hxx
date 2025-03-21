#pragma once

#include <StormByte/expected.hxx>
#include <StormByte/type_traits.hxx>
#include <StormByte/util/buffer.hxx>
#include <StormByte/util/exception.hxx>

#include <optional>
#include <utility>

/**
 * @namespace Util
 * @brief Contains utility classes and functions.
 */
namespace StormByte::Util {
	/**
	 * @class Serializable
	 * @brief The class to serialize and deserialize data.
	 * @tparam T The type of the data to serialize and deserialize.
	 */
	template<typename T>
	class Serializable {
		using DecayedT = std::decay_t<T>;	///< The decayed type of the data to serialize and deserialize.
		public:
			/**
			 * @brief The constructor of the Serializable class.
			 * @param data The data to serialize.
			 */
			Serializable(const DecayedT& data) noexcept: m_data(data) {}

			/**
			 * @brief The copy constructor of the Serializable class.
			 * @param other The other Serializable to copy.
			 */
			Serializable(const Serializable& other) noexcept 				= delete;

			/**
			 * @brief The move constructor of the Serializable class.
			 * @param other The other Serializable to move.
			 */
			Serializable(Serializable&& other) noexcept 					= delete;

			/**
			 * @brief The destructor of the Serializable class.
			 */
			~Serializable() noexcept 										= default;

			/**
			 * @brief The assignment operator of the Serializable class.
			 * @param other The other Serializable to assign.
			 * @return The reference to the assigned Serializable.
			 */
			Serializable& operator=(const Serializable& other) noexcept 	= delete;

			/**
			 * @brief The move assignment operator of the Serializable class.
			 * @param other The other Serializable to assign.
			 * @return The reference to the assigned Serializable.
			 */
			Serializable& operator=(Serializable&& other) noexcept 			= delete;

			/**
			 * @brief The function to serialize the data.
			 * @param data The data to serialize.
			 * @return The serialized data.
			 */
			Util::Buffer													Serialize() const noexcept {
				if constexpr (std::is_trivially_copyable_v<T>) {
					return SerializeTrivial();
				} else if constexpr (is_container<T>::value) {
					return SerializeContainer();
				} else if constexpr (is_pair<T>::value) {
					return SerializePair();
				} else if constexpr (is_optional<T>::value) {
					return SerializeOptional();
				} else {
					return SerializeComplex();
				}
			}

			/**
			 * @brief The function to deserialize the data.
			 * @param data The data to deserialize.
			 * @return The deserialized data.
			 */
			static StormByte::Expected<T, BufferOverflow> 					Deserialize(const Buffer& data) noexcept {
				if constexpr (std::is_trivially_copyable_v<T>) {
					return DeserializeTrivial(data);
				} else if constexpr (is_container<T>::value) {
					return DeserializeContainer(data);
				} else if constexpr (is_pair<T>::value) {
					return DeserializePair(data);
				} else if constexpr (is_optional<T>::value) {
					return DeserializeOptional(data);
				} else {
					return DeserializeComplex(data);
				}
			}

			static std::size_t												Size(const DecayedT& data) noexcept {
				if constexpr (std::is_trivially_copyable_v<T>) {
					return sizeof(data);
				} else if constexpr (is_container<T>::value) {
					return SizeContainer(data);
				} else if constexpr (is_pair<T>::value) {
					return SizePair(data);
				} else if constexpr (is_optional<T>::value) {
					return SizeOptional(data);
				} else {
					return SizeComplex(data);
				}
			}

		private:
			const DecayedT& m_data;											///< The data to serialize.

			/**
			 * @brief The function to serialize the trivial data.
			 * @param data The data to serialize.
			 * @return The serialized data.
			 */
			Util::Buffer													SerializeTrivial() const noexcept {
				return { reinterpret_cast<const char*>(&m_data), sizeof(m_data) };
			}

			/**
			 * @brief The function to serialize the complex data.
			 * @param data The data to serialize.
			 * @return The serialized data.
			 */
			Util::Buffer													SerializeComplex() const noexcept;

			/**
			 * @brief The function to serialize the container data.
			 * @param data The data to serialize.
			 * @return The serialized data.
			 */
			Util::Buffer													SerializeContainer() const noexcept {
				std::size_t size = m_data.size();
				Serializable<std::size_t> size_serial(size);
				Util::Buffer buffer(std::move(size_serial.Serialize()));
				for (const auto& element: m_data) {
					Serializable<std::decay_t<decltype(element)>> element_serial(element);
					buffer << std::move(element_serial.Serialize());
				}
				return buffer;
			}

			/**
			 * @brief The function to serialize the pair data.
			 * @param data The data to serialize.
			 * @return The serialized data.
			 */
			Util::Buffer 													SerializePair() const noexcept {
				Serializable<std::decay_t<typename T::first_type>> first_serial(m_data.first);
				Serializable<std::decay_t<typename T::second_type>> second_serial(m_data.second);
				return first_serial.Serialize() << std::move(second_serial.Serialize());
			}

			/**
			 * @brief The function to serialize the optional data.
			 * @param data The data to serialize.
			 * @return The serialized data.
			 */
			Util::Buffer 													SerializeOptional() const noexcept {
				bool has_value = m_data.has_value();
				Util::Buffer buffer = std::move(Serializable<bool>(has_value).Serialize());
				if (m_data.has_value()) {
					Serializable<std::decay_t<decltype(m_data.value())>> value_serial(m_data.value());
					buffer << std::move(value_serial.Serialize());
				}
				return buffer;
			}

			/**
			 * @brief The function to get the size of the complex data.
			 * @param data The data to get the size.
			 * @return The size of the complex data.
			 */
			static std::size_t												SizeComplex(const DecayedT& data) noexcept;

			/**
			 * @brief The function to get the size of the container data.
			 * @param data The data to get the size.
			 * @return The size of the container data.
			 */
			static std::size_t												SizeContainer(const DecayedT& data) noexcept {
				std::size_t size = sizeof(std::size_t);
				for (const auto& element: data) {
					size += Serializable<std::decay_t<decltype(element)>>::Size(element);
				}
				return size;
			}

			/**
			 * @brief The function to get the size of the pair data.
			 * @param data The data to get the size.
			 * @return The size of the pair data.
			 */
			static std::size_t												SizePair(const DecayedT& data) noexcept {
				return
					Serializable<std::decay_t<typename T::first_type>>::Size(data.first) +
					Serializable<std::decay_t<typename T::second_type>>::Size(data.second);
			}

			/**
			 * @brief The function to get the size of the optional data.
			 * @param data The data to get the size.
			 * @return The size of the optional data.
			 */
			static std::size_t 												SizeOptional(const DecayedT& data) noexcept {
				std::size_t size = sizeof(bool);
				if (data.has_value()) {
					size += Serializable<std::decay_t<decltype(data.value())>>::Size(data.value());
				}
				return size;
			}

			/**
			 * @brief The function to deserialize the trivial data.
			 * @param data Serialized data
			 * @return The deserialized data.
			 */
			static StormByte::Expected<T, BufferOverflow>					DeserializeTrivial(const Buffer& data) noexcept {
				auto expected_value = data.Read(sizeof(T));
				if (!expected_value)
					return StormByte::Unexpected(expected_value.error());
				return T { *reinterpret_cast<const T*>(expected_value.value().data()) };
			}

			/**
			 * @brief The function to deserialize the complex data.
			 * @param data Serialized data
			 * @return The deserialized data.
			 */
			static StormByte::Expected<T, BufferOverflow>					DeserializeComplex(const Buffer& data) noexcept;

			/**
			 * @brief The function to deserialize the container data.
			 * @param data Serialized data
			 * @return The deserialized data.
			 */
			static StormByte::Expected<T, BufferOverflow> 					DeserializeContainer(const Buffer& data) noexcept {	
				auto expected_container_size = Serializable<std::size_t>::Deserialize(data);
				if (!expected_container_size)
					return StormByte::Unexpected(expected_container_size.error());
	
				std::size_t size = expected_container_size.value();
	
				T container;
				for (std::size_t i = 0; i < size; ++i) {
					auto expected_element = Serializable<std::decay_t<typename T::value_type>>::Deserialize(data);
					if (!expected_element)
						return StormByte::Unexpected(expected_element.error());

					container.insert(container.end(), std::move(expected_element.value()));
				}
				return container;
			}

			/**
			 * @brief The function to deserialize the pair data.
			 * @param data Serialized data
			 * @return The deserialized data.
			 */
			static StormByte::Expected<T, BufferOverflow>					DeserializePair(const Buffer& data) noexcept {	
				using FirstT = std::decay_t<typename T::first_type>;
				using SecondT = std::decay_t<typename T::second_type>;
	
				auto expected_first = Serializable<FirstT>::Deserialize(data);
				if (!expected_first)
					return StormByte::Unexpected(expected_first.error());
	
				auto expected_second = Serializable<SecondT>::Deserialize(data);
				if (!expected_second)
					return StormByte::Unexpected(expected_second.error());
	
				return T { expected_first.value(), expected_second.value() };
			}

			static StormByte::Expected<T, BufferOverflow>					DeserializeOptional(const Buffer& data) noexcept {
				auto expected_has_value = Serializable<bool>::Deserialize(data);
				if (!expected_has_value)
					return StormByte::Unexpected(expected_has_value.error());
	
				if (expected_has_value.value()) {
					auto expected_value = Serializable<std::decay_t<decltype(m_data.value())>>::Deserialize(data);
					if (!expected_value)
						return StormByte::Unexpected(expected_value.error());
	
					return T { expected_value.value() };
				} else {
					return T {};
				}
			}
	};
}
#pragma once

#include <StormByte/expected.hxx>
#include <StormByte/type_traits.hxx>
#include <StormByte/util/buffer.hxx>
#include <StormByte/util/exception.hxx>

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
			const Util::Buffer												Serialize() const noexcept {
				if constexpr (std::is_trivially_copyable_v<T>) {
					return SerializeTrivial();
				} else if constexpr (is_container<T>::value) {
					return SerializeContainer();
				} else if constexpr (is_pair<T>::value) {
					return SerializePair();
				} else {
					return SerializeComplex();
				}
			}

			/**
			 * @brief The function to deserialize the data.
			 * @param data The data to deserialize.
			 * @return The deserialized data.
			 */
			static StormByte::Expected<T, DeserializeError> 				Deserialize(const char* data, std::size_t length) noexcept {
				if constexpr (std::is_trivially_copyable_v<T>) {
					return DeserializeTrivial(data, length);
				} else if constexpr (is_container<T>::value) {
					return DeserializeContainer(data, length);
				} else if constexpr (is_pair<T>::value) {
					return DeserializePair(data, length);
				} else {
					return DeserializeComplex(data, length);
				}
			}

			static std::size_t												Size(const DecayedT& data) noexcept {
				if constexpr (std::is_trivially_copyable_v<T>) {
					return sizeof(data);
				} else if constexpr (is_container<T>::value) {
					return SizeContainer(data);
				} else if constexpr (is_pair<T>::value) {
					return SizePair(data);
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
				Util::Buffer buffer;
				buffer.Append(reinterpret_cast<const char*>(&m_data), sizeof(m_data));
				return buffer;
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
				Util::Buffer buffer;
				std::size_t size = m_data.size();
				Serializable<std::size_t> size_serial(size);
				buffer = size_serial.Serialize();
				for (const auto& element: m_data) {
					Serializable<std::decay_t<decltype(element)>> element_serial(element);
					buffer += element_serial.Serialize();
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
				return first_serial.Serialize() + second_serial.Serialize();
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
				return Serializable<std::decay_t<typename T::first_type>>::Size(data.first) + Serializable<std::decay_t<typename T::second_type>>::Size(data.second);
			}

			/**
			 * @brief The function to deserialize the trivial data.
			 * @param data Serialized data
			 * @return The deserialized data.
			 */
			static StormByte::Expected<T, DeserializeError>					DeserializeTrivial(const char* data, const std::size_t& length) noexcept {
				if (data == nullptr || length < sizeof(T))
					return StormByte::Unexpected<DeserializeError>("Insufficient data for trivial type deserialization");
				
				return T { *reinterpret_cast<const T*>(data) };
			}

			/**
			 * @brief The function to deserialize the complex data.
			 * @param data Serialized data
			 * @return The deserialized data.
			 */
			static StormByte::Expected<T, DeserializeError>					DeserializeComplex(const char* data, const std::size_t& length) noexcept;

			/**
			 * @brief The function to deserialize the container data.
			 * @param data Serialized data
			 * @return The deserialized data.
			 */
			static StormByte::Expected<T, DeserializeError> 				DeserializeContainer(const char* data, const std::size_t& length) noexcept {
				if (data == nullptr || length < sizeof(std::size_t)) {
					return StormByte::Unexpected<DeserializeError>("Insufficient data for container size");
				}
	
				auto expected_container_size = Serializable<std::size_t>::Deserialize(data, length);
				if (!expected_container_size) {
					return StormByte::Unexpected(expected_container_size.error());
				}
	
				std::size_t size = expected_container_size.value();
				std::size_t offset = sizeof(std::size_t);
				if (length < offset) {
					return StormByte::Unexpected<DeserializeError>("Insufficient data after container size");
				}
	
				T container;
				try {
					const char* current_data = data + offset;
					std::size_t remaining_length = length - offset;
	
					for (std::size_t i = 0; i < size; ++i) {
						auto expected_element = Serializable<std::decay_t<typename T::value_type>>::Deserialize(current_data, remaining_length);
						if (!expected_element) {
							return StormByte::Unexpected(expected_element.error());
						}
	
						std::size_t element_size = Serializable<std::decay_t<typename T::value_type>>::Size(expected_element.value());
						if (remaining_length < element_size) {
							return StormByte::Unexpected<DeserializeError>("Insufficient data for container element");
						}
	
						container.insert(container.end(), expected_element.value());
						current_data += element_size;
						remaining_length -= element_size;
					}
				} catch (const std::bad_alloc& ex) {
					return StormByte::Unexpected<DeserializeError>(ex.what());
				}
				return container;
			}

			/**
			 * @brief The function to deserialize the pair data.
			 * @param data Serialized data
			 * @return The deserialized data.
			 */
			static StormByte::Expected<T, DeserializeError>					DeserializePair(const char* data, const std::size_t length) noexcept {
				if (data == nullptr)
					return StormByte::Unexpected<DeserializeError>("Null data pointer");
	
				using FirstT = std::decay_t<typename T::first_type>;
				using SecondT = std::decay_t<typename T::second_type>;
	
				auto expected_first = Serializable<FirstT>::Deserialize(data, length);
				if (!expected_first) {
					return StormByte::Unexpected(expected_first.error());
				}
	
				std::size_t first_size = Serializable<FirstT>::Size(expected_first.value());
				if (length < first_size)
					return StormByte::Unexpected<DeserializeError>("Insufficient data for pair first element");
	
				const char* second_data = data + first_size;
				std::size_t remaining_length = length - first_size;
	
				auto expected_second = Serializable<SecondT>::Deserialize(second_data, remaining_length);
				if (!expected_second) {
					return StormByte::Unexpected(expected_second.error());
				}
	
				return T{expected_first.value(), expected_second.value()};
			}
	};
}
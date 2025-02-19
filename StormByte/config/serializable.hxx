#pragma once

#include <StormByte/visibility.h>

#include <memory>
#include <string>

/**
 * @namespace Config
 * @brief All the classes for handling configuration files and items
 */
namespace StormByte::Config {
	/**
	 * @class Serializable
	 * @brief Abstract class for a serializable object
	 */
	class STORMBYTE_PUBLIC Serializable {
		public:
			/**
			 * Constructor
			 */
			constexpr Serializable() noexcept 								= default;

			/**
			 * Copy constructor
			 */
			constexpr Serializable(const Serializable&) 					= default;

			/**
			 * Move constructor
			 */
			constexpr Serializable(Serializable&&) 							= default;

			/**
			 * Assignment operator
			 */
			constexpr Serializable& operator=(const Serializable&) 			= default;

			/**
			 * Move assignment operator
			 */
			constexpr Serializable& operator=(Serializable&&) 				= default;

			/**
			 * Destructor
			 */
			constexpr virtual ~Serializable() noexcept 						= default;

			/**
			 * Serialize object to string
			 * @param level intentation level
			 * @return serialized string
			 */
			virtual std::string Serialize(const int& level) const noexcept	= 0;

			/**
			 * Clone object
			 * @return cloned object
			 */
			virtual std::shared_ptr<Serializable> 							Clone() const = 0;

			/**
			 * Move object
			 * @return moved object
			 */
			virtual std::shared_ptr<Serializable> 							Move() = 0;

			/**
			 * Gets the number of items in the current level
			 * @return size_t number of items
			 */
			virtual size_t 													Size() const noexcept = 0;

			/**
			 * Gets the full number of items
			 * @return size_t number of items
			 */
			virtual size_t 													Count() const noexcept = 0;
	};
}
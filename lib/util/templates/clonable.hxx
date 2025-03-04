#pragma once

#include <memory>

namespace StormByte::Util::Templates {
	/**
	 * @class Clonable
	 * @brief A class that can be cloned
	 */
	template<class T> class Clonable {
		public:
			/**
			 * Constructor
			 */
			constexpr Clonable() 								= default;

			/**
			 * Copy constructor
			 */
			constexpr Clonable(const Clonable&) 				= default;

			/**
			 * Move constructor
			 */
			constexpr Clonable(Clonable&&) noexcept				= default;

			/**
			 * Assignment operator
			 */
			constexpr Clonable& operator=(const Clonable&) 		= default;

			/**
			 * Move assignment operator
			 */
			constexpr Clonable& operator=(Clonable&&) noexcept	= default;

			/**
			 * Destructor
			 */
			virtual constexpr ~Clonable() noexcept 				= default;

			/**
			 * Clones the object
			 * @return cloned object
			 */
			virtual std::shared_ptr<T> Clone() const 			= 0;

			/**
			 * Moves the object
			 * @return moved object
			 */
			virtual std::shared_ptr<T> Move() 					= 0;
	};
}
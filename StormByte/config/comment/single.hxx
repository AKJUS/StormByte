#pragma once

#include <StormByte/config/comment.hxx>

/**
 * @namespace Comment
 * @brief All the classes for handling comments on configuration files
 */
namespace StormByte::Config::Comment {
	/**
	 * @class Single
	 * @brief Class for a single line comment item starting with #
	 */
	class STORMBYTE_PUBLIC Single final: public Comment {
		public:
			/**
			 * Constructor
			 * @param comment comment string
			 */
			constexpr Single(const std::string& comment):Comment(comment) {
				m_type = Type::SingleLine;
			}

			/**
			 * Move Constructor
			 * @param comment comment string
			 */
			constexpr Single(std::string&& comment):Comment(std::move(comment)) {
				m_type = Type::SingleLine;
			}

			/**
			 * Copy constructor
			 */
			constexpr Single(const Single&)						= default;

			/**
			 * Move constructor
			 */
			constexpr Single(Single&&)							= default;

			/**
			 * Assignment operator
			 */
			constexpr Single& operator=(const Single&)			= default;

			/**
			 * Move assignment operator
			 */
			constexpr Single& operator=(Single&&)				= default;

			/**
			 * Destructor
			 */
			constexpr ~Single() noexcept override				= default;

			/**
			 * Gets the comment string
			 * @return comment string
			 */
			std::string 										Serialize(const int& indent_level) const noexcept override;

			/**
			 * Clones the object
			 * @return cloned object
			 */
			std::shared_ptr<Serializable>						Clone() const override;

			/**
			 * Moves the object
			 * @return moved object
			 */
			std::shared_ptr<Serializable>						Move() override;
	};
}
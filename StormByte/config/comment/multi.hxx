#pragma once

#include <StormByte/config/comment.hxx>

/**
 * @namespace Comment
 * @brief All the classes for handling comments on configuration files
 */
namespace StormByte::Config::Comment {
	/**
	 * @class Multi
	 * @brief Class for a multiline comment item in C/C++ style
	 */
	class STORMBYTE_PUBLIC Multi final: public Comment {
		public:
			/**
			 * Constructor
			 * @param comment comment string
			 */
			constexpr Multi(const std::string& comment):Comment(comment) {
				m_type = Type::MultiLine;
			}

			/**
			 * Move Constructor
			 * @param comment comment string
			 */
			constexpr Multi(std::string&& comment):Comment(std::move(comment)) {
				m_type = Type::MultiLine;
			}

			/**
			 * Copy constructor
			 */
			constexpr Multi(const Multi&)						= default;

			/**
			 * Move constructor
			 */
			constexpr Multi(Multi&&)							= default;

			/**
			 * Assignment operator
			 */
			constexpr Multi& operator=(const Multi&)			= default;

			/**
			 * Move assignment operator
			 */
			constexpr Multi& operator=(Multi&&)					= default;

			/**
			 * Destructor
			 */
			constexpr ~Multi() noexcept override				= default;

			/**
			 * Gets the comment string
			 * @return comment string
			 */
			std::string 										Serialize(const int& indent_level) const noexcept override;

			/**
			 * Clones the object
			 * @return cloned object
			 */
			std::shared_ptr<Comment> 							Clone() const override;

			/**
			 * Moves the object
			 * @return moved object
			 */
			std::shared_ptr<Comment> 							Move() override;
	};
}
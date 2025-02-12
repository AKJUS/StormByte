#pragma once

#include <StormByte/config/item.hxx>

/**
 * @namespace StormByte::Config
 * @brief All the classes for handling configuration files and items
 */
namespace StormByte::Config {
	/**
	 * @class Comment
	 * @brief Comment in configuration item
	 * @code
	 * # This is a comment in the configuration file!
	 * @endcode
	 */
	class STORMBYTE_PUBLIC Comment final: public Item {
		public:
			/**
			 * Constructor
			 */
			Comment();
			/**
			 * Copy constructor
			 */
			Comment(const Comment&)					= default;
			/**
			 * Move constructor
			 */
			Comment(Comment&&) noexcept				= default;
			/**
			 * Assignment operator
			 */
			Comment& operator=(const Comment&)		= default;
			/**
			 * Move assignment operator
			 */
			Comment& operator=(Comment&&) noexcept	= default;
			/**
			 * Destructor
			 */
			~Comment() noexcept override			= default;

			/**
			 * Value getter
			 * @return string value
			 */
			const std::string& 		AsString() const override;

			/**
			 * Value setter
			 * @param value value to set
			 */
			void					SetString(const std::string&) override;
			/**
			 * Value setter
			 * @param value value to move to this element
			 */
			void					SetString(std::string&&) override;

			/**
			 * Serializes the string item
			 * @return serialized string
			 */
			std::string					Serialize(const int& indent_level = 0) const noexcept override;

		private:
			std::string m_value;

			std::shared_ptr<Item>		Clone() override;
	};
}
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
			constexpr const std::string& 	AsString() const override {
				return m_value;
			}

			/**
			 * Value setter
			 * @param value value to set
			 */
			inline void						SetString(const std::string& value) override {
				m_value = value;
			}
			/**
			 * Value setter
			 * @param value value to move to this element
			 */
			inline void						SetString(std::string&& value) override {
				m_value = std::move(value);
			}

			/**
			 * Serializes the boolean item
			 * @param indent_level intentation level
			 * @return serialized string
			 */
			std::string				Serialize(const int& indent_level) const noexcept override;

		private:
			/**
			 * Clones this object
			 * @return a shared pointer for this item
			 */
			std::shared_ptr<Item>	Clone() override;

			/**
			 * Current item's value
			 */
			std::string m_value;
	};
}
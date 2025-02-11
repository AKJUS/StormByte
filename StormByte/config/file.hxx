#pragma once

#include <StormByte/config/exception.hxx>
#include <StormByte/config/item/group.hxx>

#include <filesystem>
#include <memory>
#include <string>

namespace StormByte::Config {
	class STORMBYTE_PUBLIC File {
		public:
			File(const std::filesystem::path&);
			File(std::filesystem::path&&);
			File(const File&);
			File(File&&) noexcept				= default;
			File& operator=(const File&);
			File& operator=(File&&) noexcept	= default;
			virtual ~File()						= default;

			std::shared_ptr<Item>	Add(const std::string&, const Item::Type&);
			void 					Clear() noexcept;
			void 					Read();
			void					ReadFromString(const std::string&);
			void 					Write();

			std::shared_ptr<Item>	Child(const std::string&) const;
			bool					Exists(const std::string&) const noexcept;
			std::shared_ptr<Item>	LookUp(const std::string&) const;

		protected:
			virtual void			PostRead() noexcept = 0;

			std::shared_ptr<Group> 	m_root;
			std::filesystem::path 	m_file;

		private:
			void 					Parse(std::istream&, std::shared_ptr<Group>& group);
			std::string 			ParseItemName(std::istream&);
			void 					ExpectEqualSign(std::istream&);
			Item::Type 				GuessType(std::istream&);
			int						ParseIntValue(std::istream&);
			std::string 			ParseStringValue(std::istream&);
			std::string 			ParseGroupContent(std::istream&);
			double 					ParseDoubleValue(std::istream&);
			bool 					FindAndParseComment(std::istream&, std::shared_ptr<Group>& group);
			void 					ConsumeEmptyChars(std::istream&);
			void 					ExpectSemicolon(std::istream&);
	};
}
#pragma once

#include <StormByte/config/item.hxx>

#include <cstddef>
#include <iterator>
#include <map>
#include <queue>

namespace StormByte::Config {
	class STORMBYTE_PUBLIC Group final: public Item {
		friend class File;
		using GroupStorage = std::vector<std::shared_ptr<Item>>;
		public:
			Group(const std::string&);
			Group(std::string&&);
			Group(const Group&);
			Group(Group&&) noexcept				= default;
			Group& operator=(const Group&);
			Group& operator=(Group&&) noexcept	= default;
			~Group() noexcept override			= default;

			Group&						AsGroup() override;
			const int& 					AsInteger() const override;
			const std::string& 			AsString() const override;

			std::shared_ptr<Item>		Add(const std::string&, const Type&);
			std::shared_ptr<Item>		Add(std::shared_ptr<Item>);					
			void						Remove(const std::string&);

			void						SetInteger(const int&) override;
			void						SetString(const std::string&) override;
			void						SetString(std::string&&) override;

			std::shared_ptr<Item>		Child(const std::string&) const;
			bool						Exists(const std::string&) const noexcept;
			std::shared_ptr<Item>		LookUp(const std::string&) const;

			std::string					Serialize(const int&) const noexcept override;

			class STORMBYTE_PUBLIC Iterator {
				friend class Group;
				public:
					Iterator(const Iterator&)					= default;
					Iterator(Iterator&&) noexcept				= default;
					Iterator& operator=(const Iterator&)		= default;
					Iterator& operator=(Iterator&&) noexcept	= default;
					~Iterator() noexcept 						= default;

					Iterator& operator++() noexcept;
					Iterator operator++(int) noexcept;
					Iterator& operator--() noexcept;
					Iterator operator--(int) noexcept;
					bool operator==(const Iterator&) const noexcept;
					bool operator!=(const Iterator&) const noexcept;
					Item* operator->() noexcept;
					Item operator*() noexcept = delete;

				private:
					Iterator() noexcept							= default;

					GroupStorage::iterator m_it;
			};

			class STORMBYTE_PUBLIC Const_Iterator {
				friend class Group;
				public:
					Const_Iterator(const Const_Iterator&)					= default;
					Const_Iterator(Const_Iterator&&) noexcept				= default;
					Const_Iterator& operator=(const Const_Iterator&)		= default;
					Const_Iterator& operator=(Const_Iterator&&) noexcept	= default;
					~Const_Iterator() noexcept 						= default;

					Const_Iterator& operator++() noexcept;
					Const_Iterator operator++(int) noexcept;
					Const_Iterator& operator--() noexcept;
					Const_Iterator operator--(int) noexcept;
					bool operator==(const Const_Iterator&) const noexcept;
					bool operator!=(const Const_Iterator&) const noexcept;
					const Item* operator->() const noexcept;
					const Item operator*() const noexcept = delete;

				private:
					Const_Iterator() noexcept								= default;

					GroupStorage::const_iterator m_it;
			};

			Iterator 					Begin() noexcept;
			Const_Iterator				Begin() const noexcept;
			Iterator 					End() noexcept;
			Const_Iterator 				End() const noexcept;
			Const_Iterator 				CBegin() const noexcept;
			Const_Iterator 				CEnd() const noexcept;

		private:
			std::shared_ptr<Item>		Clone() override;
			std::shared_ptr<Item>		LookUp(std::queue<std::string>&) const;
			bool						Exists(std::queue<std::string>&) const noexcept;
			std::queue<std::string> 	ExplodePath(const std::string&) const noexcept;

			std::map<std::string, std::shared_ptr<Item>>	m_children;
			std::vector<std::shared_ptr<Item>> 				m_ordered;
	};
}
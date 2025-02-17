#include <StormByte/config/list.hxx>
#include <StormByte/config/exception.hxx>

#include <sstream>

using namespace StormByte::Config;

Item& List::operator[](const size_t& index) {
	return const_cast<Item&>(m_ordered[index]);
}

const Item& List::operator[](const size_t& index) const {
	if (index >= m_ordered.size())
		throw OutOfBounds(index, m_ordered.size());
	return m_ordered[index];
}

Item& List::Add(const Item& item) {
	Item item_copy(item);
	return Add(std::move(item_copy));
}

Item& List::Add(Item&& item) {
	m_ordered.push_back(std::move(item));
	return m_ordered.back();
}

void List::AddComment(const std::string& value) {
	Item item(value);
	item.m_type = Item::Type::Comment;
	m_ordered.push_back(std::move(item));
}

void List::Remove(const size_t& index) {
	if (index >= m_ordered.size())
		throw OutOfBounds(index, m_ordered.size());
	m_ordered.erase(m_ordered.begin() + index);
}

std::string List::Serialize(const int& indent_level) const noexcept {
	std::string serial = "";
	for (size_t i = 0; i < m_ordered.size(); i++) {
		serial += m_ordered[i].Serialize(indent_level + 1);
		if (m_ordered[i].GetType() == Item::Type::Comment)
			serial += "\n";
		else if (m_ordered[i].GetType() != Item::Type::Group && m_ordered[i].GetType() != Item::Type::List) {
			if (i < m_ordered.size() - 1)
				serial += ",";
			serial += "\n";
		}
	}
	return serial;
}

List::Iterator& List::Iterator::operator++() noexcept {
	++m_it;
	return *this;
}

List::Iterator List::Iterator::operator++(int) noexcept {
	Iterator it = *this;
	m_it++;
	return it;
}

List::Iterator& List::Iterator::operator--() noexcept {
	--m_it;
	return *this;
}

List::Iterator List::Iterator::operator--(int) noexcept {
	Iterator it = *this;
	m_it--;
	return it;
}

bool List::Iterator::operator==(const Iterator& it) const noexcept {
	return m_it == it.m_it;
}

bool List::Iterator::operator!=(const Iterator& it) const noexcept {
	return m_it != it.m_it;
}

Item* List::Iterator::operator->() noexcept {
	return &*m_it;
}

Item& List::Iterator::operator*() noexcept {
	return *m_it;
}

List::Const_Iterator& List::Const_Iterator::operator++() noexcept {
	++m_it;
	return *this;
}

List::Const_Iterator List::Const_Iterator::operator++(int) noexcept {
	Const_Iterator it = *this;
	m_it++;
	return it;
}

List::Const_Iterator& List::Const_Iterator::operator--() noexcept {
	--m_it;
	return *this;
}

List::Const_Iterator List::Const_Iterator::operator--(int) noexcept {
	Const_Iterator it = *this;
	m_it--;
	return it;
}

bool List::Const_Iterator::operator==(const Const_Iterator& it) const noexcept {
	return m_it == it.m_it;
}

bool List::Const_Iterator::operator!=(const Const_Iterator& it) const noexcept {
	return m_it != it.m_it;
}

const Item* List::Const_Iterator::operator->() const noexcept {
	return &*m_it;
}

const Item& List::Const_Iterator::operator*() const noexcept {
	return *m_it;
}

List::Iterator List::Begin() noexcept {
	Iterator it;
	it.m_it = m_ordered.begin();
	return it;
}

List::Const_Iterator List::Begin() const noexcept {
	Const_Iterator it;
	it.m_it = m_ordered.begin();
	return it;
}

List::Iterator List::End() noexcept {
	Iterator it;
	it.m_it = m_ordered.end();
	return it;
}

List::Const_Iterator List::End() const noexcept {
	Const_Iterator it;
	it.m_it = m_ordered.end();
	return it;
}

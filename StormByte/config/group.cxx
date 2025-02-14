#include <StormByte/config/group.hxx>
#include <StormByte/config/exception.hxx>

#include <algorithm>
#include <sstream>

using namespace StormByte::Config;

Item& Group::operator[](const std::string& path) {
	return LookUp(path);
}

const Item& Group::operator[](const std::string& path) const {
	return LookUp(path);
}

Item& Group::Add(const Item& item, const OnNameClashAction& on_clash) {
	Item item_copy(item);
	return Add(std::move(item_copy), on_clash);
}

Item& Group::Add(Item&& item, const OnNameClashAction& on_clash) {
	if (!Item::IsNameValid(item.GetName()))
		throw InvalidName(item.GetName());
	if (item.GetType() != Item::Type::Comment && Exists(item.GetName())) {
		switch (on_clash) {
			case OnNameClashAction::KeepExistingItem:
				return LookUp(item.GetName());
				break;
			case OnNameClashAction::OverwriteExistingItem: {
				Remove(item.GetName());
				break;
			}
			case OnNameClashAction::ThrowException:
				throw ItemNameAlreadyExists(item.GetName());
				break;
		}
	}
	// We store the item name since we are going to move it and will not be available
	std::string inserted_name = item.GetName();
	m_ordered.push_back(std::move(item));
	// For returning the item we need to find it (it will be 100% present)
	return *std::find_if(
		m_ordered.begin(),
		m_ordered.end(),
		[&inserted_name](const Item& i) { return i.GetType() != Item::Type::Comment && i.GetName() == inserted_name; }
	);
}

void Group::Remove(const std::string& name) {
	std::queue<std::string> path = ExplodePath(name);
	try {
		Remove(path);
	}
	catch(const std::runtime_error&) {
		throw ItemNotFound(name);
	}
}

bool Group::Exists(const std::string& path) const noexcept {
	std::queue<std::string> exploded_path = ExplodePath(path);
	return Exists(exploded_path);
}

std::string Group::Serialize(const int& indent_level) const noexcept {
	std::string serial = "";
	for (auto it = m_ordered.begin(); it != m_ordered.end(); it++) {
		serial += it->Serialize(indent_level);
	}
	return serial;
}

Item& Group::LookUp(const std::string& path) {
	std::queue<std::string> exploded_path = ExplodePath(path);
	try {
		return const_cast<Item&>(LookUp(exploded_path));
	} catch(const std::runtime_error&) {
		throw ItemNotFound(path);
	}
}

const Item& Group::LookUp(const std::string& path) const {
	std::queue<std::string> exploded_path = ExplodePath(path);
	try {
		return LookUp(exploded_path);
	} catch(const std::runtime_error&) {
		throw ItemNotFound(path);
	}
}

const Item& Group::LookUp(std::queue<std::string>& path) const {
	if (path.size() > 0) {
		std::string item_path = path.front();
		path.pop();

		auto it = std::find_if(
			m_ordered.begin(),
			m_ordered.end(),
			[&item_path](const Item& i) { return i.GetType() != Item::Type::Comment && i.GetName() == item_path; }
		);

		if (it != m_ordered.end()) {
			if (path.size() == 0)
				return *it;
			else if (it->GetType() != Item::Type::Group)
				throw std::runtime_error("blasafel");
			else
				return it->Value<Group>().LookUp(path);
		}
	}
	// We should not have arrived here
	throw std::runtime_error("blasafel"); // Caller will throw the correct exception
}

bool Group::Exists(std::queue<std::string>& path) const noexcept {
	try {
		LookUp(path);
		return true;
	} catch(const std::runtime_error&) {
		return false;
	}
}

std::queue<std::string> Group::ExplodePath(const std::string& path) const noexcept {
	std::queue<std::string> result;
    std::stringstream ss(path);
    std::string item;

    while (std::getline(ss, item, '/')) {
        result.push(item);
    }

	return result;
}

Group::Iterator& Group::Iterator::operator++() noexcept {
	++m_it;
	return *this;
}

Group::Iterator Group::Iterator::operator++(int) noexcept {
	Iterator it = *this;
	m_it++;
	return it;
}

Group::Iterator& Group::Iterator::operator--() noexcept {
	--m_it;
	return *this;
}

Group::Iterator Group::Iterator::operator--(int) noexcept {
	Iterator it = *this;
	m_it--;
	return it;
}

bool Group::Iterator::operator==(const Iterator& it) const noexcept {
	return m_it == it.m_it;
}

bool Group::Iterator::operator!=(const Iterator& it) const noexcept {
	return m_it != it.m_it;
}

Item* Group::Iterator::operator->() noexcept {
	return &*m_it;
}

Item& Group::Iterator::operator*() noexcept {
	return *m_it;
}

Group::Const_Iterator& Group::Const_Iterator::operator++() noexcept {
	++m_it;
	return *this;
}

Group::Const_Iterator Group::Const_Iterator::operator++(int) noexcept {
	Const_Iterator it = *this;
	m_it++;
	return it;
}

Group::Const_Iterator& Group::Const_Iterator::operator--() noexcept {
	--m_it;
	return *this;
}

Group::Const_Iterator Group::Const_Iterator::operator--(int) noexcept {
	Const_Iterator it = *this;
	m_it--;
	return it;
}

bool Group::Const_Iterator::operator==(const Const_Iterator& it) const noexcept {
	return m_it == it.m_it;
}

bool Group::Const_Iterator::operator!=(const Const_Iterator& it) const noexcept {
	return m_it != it.m_it;
}

const Item* Group::Const_Iterator::operator->() const noexcept {
	return &*m_it;
}

const Item& Group::Const_Iterator::operator*() const noexcept {
	return *m_it;
}

Group::Iterator Group::Begin() noexcept {
	Iterator it;
	it.m_it = m_ordered.begin();
	return it;
}

Group::Const_Iterator Group::Begin() const noexcept {
	Const_Iterator it;
	it.m_it = m_ordered.begin();
	return it;
}

Group::Iterator Group::End() noexcept {
	Iterator it;
	it.m_it = m_ordered.end();
	return it;
}

Group::Const_Iterator Group::End() const noexcept {
	Const_Iterator it;
	it.m_it = m_ordered.end();
	return it;
}

void Group::Remove(std::queue<std::string>& path) {
	if (path.size() > 0) {
		std::string item_path = path.front();
		path.pop();

		auto it = std::find_if(
			m_ordered.begin(),
			m_ordered.end(),
			[&item_path](const Item& i) { return i.GetType() != Item::Type::Comment && i.GetName() == item_path; }
		);

		if (it != m_ordered.end()) {
			if (path.size() == 0)
				m_ordered.erase(it);
			else if (it->GetType() != Item::Type::Group)
				throw std::runtime_error("blasafel");
			else
				it->Value<Group>().Remove(path);
		}
	}
}
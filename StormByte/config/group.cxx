#include <StormByte/config/group.hxx>
#include <StormByte/config/exception.hxx>

#include <sstream>

using namespace StormByte::Config;

NamedItem& Group::operator[](const std::string& path) {
	return LookUp(path);
}

const NamedItem& Group::operator[](const std::string& path) const {
	return LookUp(path);
}

NamedItem& Group::Add(const NamedItem& item, const OnNameClashAction& on_clash) {
	NamedItem item_copy(item);
	return Add(std::move(item_copy), on_clash);
}

NamedItem& Group::Add(NamedItem&& item, const OnNameClashAction& on_clash) {
	if (!NamedItem::IsNameValid(item.GetName()))
		throw InvalidName(item.GetName());
	if (item.GetType() != Item::Type::Comment && Exists(item.GetName())) {
		switch (on_clash) {
			case OnNameClashAction::KeepExistingNamedItem:
				return LookUp(item.GetName());
				break;
			case OnNameClashAction::OverwriteExistingNamedItem: {
				Remove(item.GetName());
				break;
			}
			case OnNameClashAction::ThrowException:
				throw NamedItemNameAlreadyExists(item.GetName());
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
		[&inserted_name](const NamedItem& i) { return i.GetType() != Item::Type::Comment && i.GetName() == inserted_name; }
	);
}

void Group::AddComment(const std::string& value) {
	NamedItem item("", value);
	item.m_type = Item::Type::Comment;
	m_ordered.push_back(std::move(item));
}

void Group::Remove(const std::string& name) {
	if (!NamedItem::IsPathValid(name))
		throw InvalidName(name);
	std::queue<std::string> path = ExplodePath(name);
	try {
		Remove(path);
	}
	catch(const std::runtime_error&) {
		throw NamedItemNotFound(name);
	}
}

bool Group::Exists(const std::string& path) const noexcept {
	std::queue<std::string> exploded_path = ExplodePath(path);
	return Exists(exploded_path);
}

std::string Group::Serialize(const int& indent_level) const noexcept {
	std::string serial = "";
	for (auto it = m_ordered.begin(); it != m_ordered.end(); it++) {
		serial += it->Serialize(indent_level + 1);
	}
	return serial;
}

NamedItem& Group::LookUp(const std::string& path) {
	std::queue<std::string> exploded_path = ExplodePath(path);
	try {
		return const_cast<NamedItem&>(LookUp(exploded_path));
	} catch(const std::runtime_error&) {
		throw NamedItemNotFound(path);
	}
}

const NamedItem& Group::LookUp(const std::string& path) const {
	std::queue<std::string> exploded_path = ExplodePath(path);
	try {
		return LookUp(exploded_path);
	} catch(const std::runtime_error&) {
		throw NamedItemNotFound(path);
	}
}

const NamedItem& Group::LookUp(std::queue<std::string>& path) const {
	if (path.size() > 0) {
		std::string item_path = path.front();
		path.pop();

		auto it = std::find_if(
			m_ordered.begin(),
			m_ordered.end(),
			[&item_path](const NamedItem& i) { return i.GetType() != Item::Type::Comment && i.GetName() == item_path; }
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

NamedItem* Group::Iterator::operator->() noexcept {
	return &*m_it;
}

NamedItem& Group::Iterator::operator*() noexcept {
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

const NamedItem* Group::Const_Iterator::operator->() const noexcept {
	return &*m_it;
}

const NamedItem& Group::Const_Iterator::operator*() const noexcept {
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
			[&item_path](const NamedItem& i) { return i.GetType() != Item::Type::Comment && i.GetName() == item_path; }
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
#include <StormByte/config/config.hxx>
#include <StormByte/config/named_item.hxx>

#include <iostream> // For logger and std::istream
#include <regex>
#include <sstream>

using namespace StormByte::Config;

Config::Config():m_root(), m_on_name_clash_action(Group::OnNameClashAction::ThrowException) {}

Config::Config(const Config& config):
m_root(config.m_root),m_before_read_hooks(config.m_before_read_hooks),
m_after_read_hooks(config.m_after_read_hooks), m_on_name_clash_action(config.m_on_name_clash_action) {}

Config::Config(Config&& config) noexcept:
m_root(std::move(config.m_root)),m_before_read_hooks(std::move(config.m_before_read_hooks)),
m_after_read_hooks(std::move(config.m_after_read_hooks)), m_on_name_clash_action(config.m_on_name_clash_action) {}

Config& Config::operator=(const Config& config) {
	if (this != &config) {
		m_root = Group(config.m_root);
		m_on_name_clash_action = config.m_on_name_clash_action;
		m_before_read_hooks = config.m_before_read_hooks;
		m_after_read_hooks = config.m_after_read_hooks;
	}
	return *this;
}

Config& Config::operator=(Config&& config) noexcept {
	if (this != &config) {
		m_root = std::move(config.m_root);
		m_on_name_clash_action = config.m_on_name_clash_action;
		m_before_read_hooks = std::move(config.m_before_read_hooks);
		m_after_read_hooks = std::move(config.m_after_read_hooks);
	}
	return *this;
}

void Config::Clear() noexcept {
	m_root = Group();
}

NamedItem& Config::operator[](const std::string& path) {
	return m_root[path];
}

const NamedItem& Config::operator[](const std::string& path) const {
	return m_root[path];
}

Config& Config::operator<<(const Config& source) {
	// We will not use serialize for performance reasons
	for (auto it = source.Begin(); it != source.End(); it++)
		m_root.Add(*it, m_on_name_clash_action);
	return *this;
}

void Config::operator<<(std::istream& istream) { // 1
	Parse(istream, m_root);
}

void Config::operator<<(const std::string& str) { // 2
	std::istringstream istream(str);
	*this << istream;
}

Config& StormByte::Config::operator>>(std::istream& istream, Config& config) { // 3
	config << istream;
	return config;
}

Config& StormByte::Config::operator>>(const std::string& str, Config& config) { // 4
	config << str;
	return config;
}

Config& Config::operator>>(Config& dest) const {
	dest << *this;
	return dest;
}

std::ostream& Config::operator>>(std::ostream& ostream) const { // 5
	ostream << (std::string)*this;
	return ostream;
}

std::string& Config::operator>>(std::string& str) const { // 6
	str += *this; // Conversion should be done automatically by operator std::string()
	return str;
}

std::ostream& StormByte::Config::operator<<(std::ostream& ostream, const Config& config) { // 7
	ostream << (std::string)config;
	return ostream;
}

std::string& operator<<(std::string& str, const Config& config) { // 8
	str += config;
	return str;
}

Config::operator std::string() const {
	std::string serialized = "";
	for (auto it = Begin(); it != End(); it++) {
		serialized += it->Serialize(0);// + "\n";
	}
	return serialized;
}

bool Config::Exists(const std::string& path) const noexcept {
	return m_root.Exists(path);
}

Group::Iterator Config::Begin() noexcept {
	return m_root.Begin();
}

Group::Const_Iterator Config::Begin() const noexcept {
	return m_root.Begin();
}

Group::Iterator Config::End() noexcept {
	return m_root.End();
}

Group::Const_Iterator Config::End() const noexcept {
	return m_root.End();
}

template <const char start, const char end> std::string Config::ParseContainerContents(std::istream& istream) {
	char c;
	if (!istream.get(c) || c != start) {
		throw ParseError("Expected '" + std::string(1, start) + "' but found " + std::string(1, c) + " near " + GetCurrentLine(istream, -20));
	}
	else {
		int level = 1; // Assuming that we are already inside the container
		std::string buffer = "";
		bool string_started = false;
		bool escape_next = false;
		while (level > 0 && istream.get(c)) {
			// We will parse char by char looking for end character:
			// If it is inside a string or a comment it will be ignored
			// If a valid start character is found, level will be raised
			// When level == 0 we must check its final semicolon
			if (escape_next) {
				buffer += c;
				escape_next = false;
			}
			else {
				switch (c) {
					case '\\':
						escape_next = true;
						buffer += c;
						break;
					case '"':
						buffer += c;
						string_started = !string_started;
						break;
					case '#':
						if (!string_started) {
							std::string comment;
							std::getline(istream, comment);
							// Getline does not include the newline character
							buffer += c + comment + '\n';
						}
						else
							buffer += c;
						break;
					case start:
						buffer += c;
						if (!string_started)
							level++;
						break;
					case end:
						// In end case if level is 0 we don't want to add the character
						if (string_started)
							buffer += c;
						else
							if (--level > 0)
								buffer += c;
						break;
					default:
						buffer += c;
						break;
				}
			}
		}
		if (level > 0) {
			throw ParseError("Expected '" + std::string(1, end) + "' but found EOF near " + GetCurrentLine(istream, -20));
		}
		return buffer;
	}
}

template<> double Config::ParseValue<double>(std::istream& istream) {
	std::string buffer;
	istream >> std::skipws;
	istream >> buffer;

	// std::stod just ignore extra characters so we better check
	std::regex double_regex(R"(^[+-]?(\d+(\.\d*)?|\.\d+)([eE][+-]?\d+)?$)");
	if (!std::regex_match(buffer, double_regex))
		throw ParseError("Failed to parse double value '" + buffer + "'");
	try {
		double result = std::stod(buffer);
		istream >> std::noskipws;
		return result;
	}
	catch (std::invalid_argument&) {
		istream.seekg(-(int)buffer.size(), std::ios_base::cur); // Cast needed to avoid MSVC warning
		std::string line = GetCurrentLine(istream);
		throw Exception("Failed to parse double value near " + line);
	}
	catch (std::out_of_range&) {
		istream.seekg(-(int)buffer.size(), std::ios_base::cur); // Cast needed to avoid MSVC warning
		std::string line = GetCurrentLine(istream);
		throw Exception("Double value out of range near " + line);
	}
}

template<> int Config::ParseValue<int>(std::istream& istream) {
	std::string buffer;
	istream >> std::skipws;
	istream >> buffer;

	// stoi will ignore extra characters so we force check
	std::regex integer_regex(R"(^[+-]?\d+$)");
	if (!std::regex_match(buffer, integer_regex))
		throw ParseError("Failed to parse integer value '" + buffer + '"');
	try {
		int result = std::stoi(buffer);
		istream >> std::noskipws;
		return result;
	}
	catch (std::invalid_argument&) {
		istream.seekg(-(int)buffer.size(), std::ios_base::cur); // Cast needed to avoid MSVC warning
		std::string line = GetCurrentLine(istream);
		throw Exception("Failed to parse integer value near " + line);
	}
	catch (std::out_of_range&) {
		istream.seekg(-(int)buffer.size(), std::ios_base::cur); // Cast needed to avoid MSVC warning
		std::string line = GetCurrentLine(istream);
		throw Exception("Integer value out of range near " + line);
	}
}

template<> std::string Config::ParseValue<std::string>(std::istream& istream) {
	istream >> std::ws;
	std::string accumulator;
	// Guesser already detected the opened " so we skip it
	istream.seekg(1, std::ios::cur);
	bool string_closed = false;

	if (istream.eof())
		throw ParseError("String content was expected but found EOF");

	// Do not skip space characters
	istream >> std::noskipws;
	bool escape_next = false;
	char c;
	while (istream.get(c)) {
		if (escape_next) {
			// Handle escaped characters
			switch (c) {
				case '"':
				case '\\':
					accumulator += c;
					break;
				case 'n':
					accumulator += '\n';
					break;
				case 'r':
					accumulator += '\r';
					break;
				case 't':
					accumulator += '\t';
					break;
				default:
					throw ParseError(std::string("Invalid escape sequence: \\") + std::string(1, c) + " near: " + GetCurrentLine(istream, -20));
					break;
			}
			escape_next = false;
		} else {
			if (c == '\\') {
				// Indicate that the next character should be escaped
				escape_next = true;
			} else if (c == '"') {
				string_closed = true;
				break;
			} else {
				accumulator += c;
			}
		}
	}

	if (!string_closed)
		throw ParseError("Expected string closure but got EOF");

	return accumulator;
}

template<> bool Config::ParseValue<bool>(std::istream& istream) {
	bool result;
	istream >> std::skipws >> std::boolalpha;
	istream >> result;
	if (istream.fail()) {
		throw Exception("Failed to parse boolean value near " + GetCurrentLine(istream, -20));
	}
	istream >> std::noskipws >> std::noboolalpha;
	return result;
}

template<class C> bool Config::FindAndParseComment(std::istream& istream, C& container) {
	istream >> std::ws;
	if (istream.eof() || istream.fail())
		return false;
	std::string line;
	bool result = false;
	auto start_pos = istream.tellg();
	if (std::getline(istream, line)) {
		if (line[0] == '#') {
			line.erase(line.begin());
			container.AddComment(line);
			result = true;
		}
		else {
			istream.seekg(start_pos);
			result = false;
		}
	}
	else {
		istream.clear();
		istream.seekg(start_pos);
	}
	return result;
}

template<class C> void Config::FindAndParseComments(std::istream& istream, C& container) {
	while (FindAndParseComment(istream, container));
}

std::unique_ptr<NamedItem> Config::ParseItem(std::istream& istream, const Item::Type& type, std::string&& item_name) {
	switch(type) {
		case Item::Type::Group: {
			std::istringstream buffer(ParseContainerContents<'{', '}'>(istream));
			Group group;
			Parse(buffer, group);
			return std::make_unique<NamedItem>(std::move(item_name), std::move(group));
			break;
		}
		case Item::Type::Comment: // Not handled here but make compiler not emit any warning
		case Item::Type::String:
			return std::make_unique<NamedItem>(std::move(item_name), ParseValue<std::string>(istream));
		case Item::Type::Integer:
			return std::make_unique<NamedItem>(item_name, ParseValue<int>(istream));
		case Item::Type::Double:
			return std::make_unique<NamedItem>(item_name, ParseValue<double>(istream));
		case Item::Type::Bool:
			return std::make_unique<NamedItem>(item_name, ParseValue<bool>(istream));
		case Item::Type::List: {
			std::istringstream buffer(ParseContainerContents<'[', ']'>(istream));
			List list;
			Parse(buffer, list);
			return std::make_unique<NamedItem>(std::move(item_name), std::move(list));
			break;
		}
		default:
			throw ParseError("Failed to parse item");
	}
}

std::unique_ptr<Item> Config::ParseItem(std::istream& istream, const Item::Type& type) {
	switch(type) {
		case Item::Type::Group: {
			std::istringstream buffer(ParseContainerContents<'{', '}'>(istream));
			Group group;
			Parse(buffer, group);
			return std::make_unique<Item>(std::move(group));
			break;
		}
		case Item::Type::Comment: // Not handled here but make compiler not emit any warning
		case Item::Type::String:
			return std::make_unique<Item>(ParseValue<std::string>(istream));
		case Item::Type::Integer:
			return std::make_unique<Item>(ParseValue<int>(istream));
		case Item::Type::Double:
			return std::make_unique<Item>(ParseValue<double>(istream));
		case Item::Type::Bool:
			return std::make_unique<Item>(ParseValue<bool>(istream));
		case Item::Type::List: {
			std::istringstream buffer(ParseContainerContents<'[', ']'>(istream));
			List list;
			Parse(buffer, list);
			return std::make_unique<Item>(std::move(list));
			break;
		}
		default:
			throw ParseError("Failed to parse item");
	}
}

std::string Config::GetCurrentLine(std::istream& istream) {
	std::string line = "";
	auto start_pos = istream.tellg();
	if (std::getline(istream, line)) {
		istream.seekg(start_pos);
	}
	else {
		istream.clear();
		istream.seekg(start_pos);
	}
	return line;
}

std::string Config::GetCurrentLine(std::istream& istream, const int& offset) {
	istream.clear();
	istream.seekg(offset, std::ios_base::cur);
	return GetCurrentLine(istream);
}

void Config::Parse(std::istream& istream, Group& group) {
	bool halt = false;
	FindAndParseComments(istream, group);
	while (!halt && !istream.eof()) {
		char c;
		// Item Name
		std::string item_name = ParseItemName(istream);
		if (!NamedItem::IsNameValid(item_name)) {
			throw ParseError("Invalid item name: " + item_name);
		}
		// Equal expected
		istream >> std::ws;
		if (!istream.get(c) || c != '=') {
			throw ParseError("Expected '=' after item name: " + item_name);
		}

		// Guessing type
		auto type = ParseType(istream);
		std::unique_ptr<NamedItem> item = ParseItem(istream, type, std::move(item_name));
		if (!item)
			throw ParseError("Failed to parse item");
		else {
			group.Add(std::move(*item), m_on_name_clash_action);
		}
		// Group items require to have ending semicolon (but there might be comments)
		FindAndParseComments(istream, group);
		FindAndParseComments(istream, group);
		if (istream.fail()) halt = true;
	}
}

void Config::Parse(std::istream& istream, List& list) {
	bool halt = false;
	FindAndParseComments(istream, list);
	while (!halt && !istream.eof()) {
		// Guessing type
		auto type = ParseType(istream);
		std::unique_ptr<Item> item = ParseItem(istream, type);
		if (!item)
			throw ParseError("Failed to parse item");
		else {
			list.Add(std::move(*item));
		}
		istream >> std::ws;
		// List items only require colon separator when it is not the last element
		FindAndParseComments(istream, list);
		if (istream.fail()) halt = true;
	}
}

std::string Config::ParseItemName(std::istream& istream) {
	std::string name;
	istream >> std::skipws;
	istream >> name;
	istream >> std::noskipws;
	return name;
}

Item::Type Config::ParseType(std::istream& istream) {
	/**** THIS FUNCTION ONLY DETECTS TYPE NOT CHECKS FOR VALIDITY *****/
	std::unique_ptr<Item::Type> type;
	istream >> std::ws;
	auto start_position = istream.tellg();
	std::string line;
	std::getline(istream, line);
	/** COMMENTS ARE NOT HANDLED HERE **/
	switch (line[0]) {
		case '"':
			type = std::make_unique<Item::Type>(Item::Type::String);
			break;
		case '[':
			type = std::make_unique<Item::Type>(Item::Type::List);
			break;
		case '{':
			type = std::make_unique<Item::Type>(Item::Type::Group);
			break;
		case '-':
		case '+':
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9': {
			// Possible int or double, we assume int until we find a "." (not exp notation as without a . it is allowed in integers too)
			// NOTE: We don't do sanity check here!
			type = std::make_unique<Item::Type>(Item::Type::Integer);
			bool halt = false;
			size_t pos = 1;
			while (pos < line.size() && !halt) {
				if (line[pos++] == '.') {
					type = std::make_unique<Item::Type>(Item::Type::Double);
					halt = true;
				}
			}
			break;
		}
		case 't':
		case 'f':
			type = std::make_unique<Item::Type>(Item::Type::Bool);
			break;
		default: {
			throw ParseError("Unexpected character when parsing type near " + line);
		}
	}
	istream.seekg(start_position);
	return *type;
}
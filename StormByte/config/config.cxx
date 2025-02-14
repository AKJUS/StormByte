#include <StormByte/config/config.hxx>

#include <fstream>
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

Item& Config::operator[](const std::string& path) {
	return m_root[path];
}

const Item& Config::operator[](const std::string& path) const {
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
	for (auto it = Begin(); it != End(); it++)
		serialized += it->Serialize(0);
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

template<> int Config::ParseValue<int>(std::istream& stream) {
	ConsumeEmptyChars(stream);
	std::string accumulator = "";
	if (stream.eof())
		throw ParseError("Expected number but got EOF");
	while (!stream.eof()) {
		char c;
		stream.get(c);
		if (std::isdigit(c))
			accumulator += c;
		else if (c == ';')
			break;
		else
			throw ParseError((std::string)"Expected number or ; but got " + c);
	}
	if (accumulator.empty())
		throw ParseError("Expected number but got ; without any");
	
	return std::stoi(accumulator);
}

template<> std::string Config::ParseValue<std::string>(std::istream& stream) {
	ConsumeEmptyChars(stream);
	std::string accumulator;
	// Guesser already detected the opened " so we skip it
	stream.seekg(1, std::ios::cur);
	bool string_closed = false;

	if (stream.eof())
		throw ParseError("String content was expected but found EOF");

	// Do not skip space characters
	stream >> std::noskipws;
	bool escape_next = false;
	while (!stream.eof()) {
		char c;
		stream.get(c);

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
					throw ParseError(std::string("Invalid escape sequence: \\") + c);
					break;
			}
			escape_next = false;
		} else {
			if (c == '\\') {
				// Indicate that the next character should be escaped
				escape_next = true;
			} else if (c == '"') {
				string_closed = true;
				ExpectSemicolon(stream);
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

template<> double Config::ParseValue<double>(std::istream& stream) {
	ConsumeEmptyChars(stream);
	std::string accumulator = "";
	bool found_point = false;
	if (stream.eof())
		throw ParseError("Expected number but got EOF");
	while (!stream.eof()) {
		char c;
		stream.get(c);
		if (std::isdigit(c))
			accumulator += c;
		else if (c == '.') {
			if (found_point)
				throw ParseError("The double number is illformed as it contains more than one decimal dot");
			found_point = true;
			accumulator += c;
		}
		else if (c == ';')
			break;
		else if (c == 'e') {
			accumulator += "e" + ParseExpValue(stream);
			break;
		}
		else
			throw ParseError((std::string)"Expected number or ; but got " + c);
	}
	if (accumulator.empty())
		throw ParseError("Expected number but got ; without any");
	
	return std::stod(accumulator);
}

template<> bool Config::ParseValue<bool>(std::istream& stream) {
	std::string accumulator = "";
	bool value;
	char c;
	stream.get(c);
	accumulator += c;
	if (c == 't') {
		for (auto i = 0; i < 3 && !stream.eof(); i++) {
			stream.get(c);
			accumulator += c;
		}
	}
	else if (c == 'f') {
		for (auto i = 0; i < 4 && !stream.eof(); i++) {
			stream.get(c);
			accumulator += c;
		}
	}
	else
		throw ParseError(std::string("Expected initial t (for true) or f (for false) but got ") + c); // This should not happen but just in case

	if (accumulator == "true")
		value = true;
	else if (accumulator == "false")
		value = false;
	else
		throw ParseError("Expected true or false (lowercase) but got " + accumulator);

	ExpectSemicolon(stream);
	return value;
}

void Config::Parse(std::istream& stream, std::unique_ptr<Group>& group) {
	for (auto it = m_before_read_hooks.begin(); it != m_before_read_hooks.end(); it++)
		(*it)(*this);
	Parse(stream, *group);
	for (auto it = m_after_read_hooks.begin(); it != m_after_read_hooks.end(); it++)
		(*it)(*this);
}
#include <iostream>
void Config::Parse(std::istream& stream, Group& group) {
	while (FindAndParseComment(stream, group)) {}
	while (!stream.eof()) {
		// The first thing we expect to encounter is the item name
		std::string item_name = ParseItemName(stream);
		ExpectEqualSign(stream);
		Item::Type item_type = GuessType(stream);
		switch (item_type) {
			case Item::Type::Integer: {
				auto item = Item(item_name, ParseValue<int>(stream));
				group.Add(std::move(item), m_on_name_clash_action);
				break;
			}
			case Item::Type::String: {
				auto item = Item(item_name, ParseValue<std::string>(stream));
				group.Add(std::move(item), m_on_name_clash_action);
				break;
			}
			case Item::Type::Group: {
				auto new_group = Group();
				std::istringstream group_stream(ParseGroupContent(stream));
				Parse(group_stream, new_group);
				auto item = Item(item_name, new_group);
				group.Add(std::move(item), m_on_name_clash_action);
				break;
			}
			case Item::Type::Double: {
				auto item = Item(item_name, ParseValue<double>(stream));
				group.Add(std::move(item), m_on_name_clash_action);
				break;
			}
			case Item::Type::Comment:
				// This should never happen but we make compiler happy
				break;
			case Item::Type::Bool:
				auto item = Item(item_name, ParseValue<bool>(stream));
				group.Add(std::move(item), m_on_name_clash_action);
		}
		while (FindAndParseComment(stream, group)) {}
	}
}

std::string Config::ParseItemName(std::istream& stream) {
	ConsumeEmptyChars(stream);
	char c;
	std::string item_name = "";
	while (stream.get(c)) {
		if (std::isalnum(c) || c == '_')
			item_name += c;
		else if (c == ' ') break;
		else if (c == '=') {
			// Restore position so next character is the equal sign
			stream.seekg(-1, std::ios::cur);
			break;
		}
	}
	return item_name;
}

void Config::ExpectEqualSign(std::istream& stream) {
	ConsumeEmptyChars(stream);
	std::string fault_reason;
	if (stream.eof())
		fault_reason = "Expected equal sign but EOF met!";
	else {
		char c;
		stream.get(c);
		if (c != '=')
			fault_reason = (std::string)"Expected equal sign but got '" + c + std::string("'");
	}

	if (!fault_reason.empty())
		throw ParseError(fault_reason);
}

Item::Type Config::GuessType(std::istream& stream) {
	ConsumeEmptyChars(stream);
	// We store current position to restore once guessed the type
	auto current_position = stream.tellg();
	std::unique_ptr<Item::Type> item_type;
	if (stream.eof())
		throw ParseError("Expected number, \" or { but found EOF");
	// While is to implement in a future Item::Type::Double
	while (!stream.eof()) {
		char c;
		stream.get(c);
		if (std::isdigit(c)) {
			item_type = std::make_unique<Item::Type>(Item::Type::Integer);
			// We assumed integer but will keep reading until we find a . for double or a ; which confirms it is an integer
			while (!stream.eof()) {
				stream.get(c);
				if (c == '.') {
					item_type = std::make_unique<Item::Type>(Item::Type::Double);
					break;
				}
				else if (c == ';')
					break;
			}
			break;
		}
		else if (c == '"') {
			item_type = std::make_unique<Item::Type>(Item::Type::String);
			break;
		}
		else if (c == '{') {
			item_type = std::make_unique<Item::Type>(Item::Type::Group);
			break;
		}
		else if (c == '#') {
			item_type = std::make_unique<Item::Type>(Item::Type::Comment);
			break;
		}
		else if (c == 't' || c == 'f') {
			item_type = std::make_unique<Item::Type>(Item::Type::Bool);
			break;
		}
		else
			throw ParseError(std::string("Expected number, \", {, t or f but found ") + c);
	}
	stream.seekg(current_position);
	if (!item_type)
		throw ParseError("Could not guess Item Type!");
	return *item_type;
}

std::string Config::ParseGroupContent(std::istream& stream) {
	// This function will parse the whole content of a group
	// knowing that the next character is {, we will accumulate
	// every character and count the levels of "{" to be sure
	// that we get the contents, also we check for literal {
	// inside a string to ignore it
	std::string accumulator = "";
	short level = 1;
	bool string_started = false;
	bool halt = false;
	char c;
	stream.get(c);
	if (c != '{')
		throw ParseError((std::string)"Expected { character but got " + c);

	while (!stream.eof() && !halt) {
		stream.get(c);
		switch(c) {
			case '"':
				string_started = !string_started;
				accumulator += c;
				break;
			case '{':
				if (!string_started) level++;
				accumulator += c;
				break;
			case '}':
				if (string_started) accumulator += c;
				else {
					level--;
					if (level == 0) halt = true;
					else accumulator += c;
					continue;
				};
				break;
			default:
				accumulator += c;
				break;
		}
	}

	if (!halt)
		throw ParseError("We should expect the equivalent of the opening { but found EOF first");
	else
		ExpectSemicolon(stream);

	return accumulator;
}

std::string Config::ParseExpValue(std::istream& stream) {
	// We start here past the e item in exponential double
	std::string accumulator = "";
	char c;

	if (stream.eof())
		throw ParseError("Expected exponential notation when EOF found");

	// We expect a sign + or -
	stream.get(c);
	switch (c) {
		case '+':
		case '-':
			accumulator += c;
			accumulator += std::to_string(ParseValue<int>(stream));
			break;
		default:
			throw ParseError(std::string("Expected expontial sign + or - but got ") + c);
			break;
	}
	// ParseIntValue already skips ; so we have finished
	return accumulator;
}

bool Config::FindAndParseComment(std::istream& stream, Group& group) {
	ConsumeEmptyChars(stream);
	std::string accumulator;
	if (stream.eof())
		return false;
	char c;
	stream.get(c);
	if (c == '#') {
		std::getline(stream, accumulator, '\n');
		if (!accumulator.empty() && accumulator.back() == '\n')
			accumulator.pop_back();

		if (!accumulator.empty())
			group.Add(Item(accumulator));
	}
	else {
		// Leave the position before this read
		stream.seekg(-1, std::ios::cur);
	}

	return !accumulator.empty();
}

void Config::ConsumeEmptyChars(std::istream& stream) {
	stream >> std::ws;
	while (!stream.eof()) {
		char c;
		stream.get(c);
		switch(c) {
			case ' ':
			case '\n':
			case '\t':
			case '\r':
				stream >> std::ws;
				break;
			default:
				stream.seekg(-1, std::ios::cur);
				return;
		}
	}
}

void Config::ExpectSemicolon(std::istream& stream) {
	std::string error;
	if (stream.eof()) error = "Expected ; but found EOF";
	else {
		char c = '\0'; // Make sure c has always content
		stream.get(c);
		if (c != ';') error = (std::string)"Expected ; but found " + c;
	}

	if (!error.empty())
		throw ParseError(error);
}

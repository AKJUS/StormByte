#include <StormByte/config/exception.hxx>
#include <StormByte/config/file.hxx>
#include <StormByte/config/item/value/double.hxx>
#include <StormByte/config/item/value/integer.hxx>
#include <StormByte/config/item/value/string.hxx>
#include <StormByte/config/item/group.hxx>

#include <fstream>
#include <sstream>

using namespace StormByte::Config;

File::File(const std::filesystem::path& file):m_file(file) {
	m_root = std::make_unique<Group>("root");
}

File::File(std::filesystem::path&& file):m_file(std::move(file)) {
	m_root = std::make_unique<Group>("root");
}

File::File(const File& file):m_file(file.m_file) {
	m_root = std::make_unique<Group>(*file.m_root);
}

File& File::operator=(const File& file) {
	if (this != &file) {
		m_file = file.m_file;
		m_root = std::make_unique<Group>(*file.m_root);
	}
	return *this;
}

std::shared_ptr<Item> File::Add(const std::string& name, const Item::Type& type) {
	return m_root->Add(name, type);
}

void File::Clear() noexcept { m_root = std::make_unique<Group>("root"); }

void File::Read() {
	Clear();
	std::ifstream file;
	file.open(m_file, std::ios::in);
	if (file.fail())
		throw System::FileIOError(m_file, System::FileIOError::Read);

	file >> std::noskipws;
	Parse(file, m_root);
	file.close();

	this->PostRead();
}

void File::ReadFromString(const std::string& cfg_str) {
	Clear();

	std::istringstream is(cfg_str);
	is >> std::noskipws;
	Parse(is, m_root);

	this->PostRead();
}

void File::Write() {
	std::ofstream file;
	file.open(m_file, std::ios::out);
	if (file.fail())
		throw System::FileIOError(m_file, System::FileIOError::Write);
	
	for (auto it = m_root->m_ordered.begin(); it != m_root->m_ordered.end(); it++)
		file << it->get()->Serialize(0) << "\n";

	file.close();
}

std::shared_ptr<Item> File::Child(const std::string& path) const {
	return m_root->Child(path);
}

bool File::Exists(const std::string& path) const noexcept {
	return m_root->Exists(path);
}

std::shared_ptr<Item> File::LookUp(const std::string& path) const {
	return m_root->LookUp(path);
}

void File::Parse(std::istream& stream, std::shared_ptr<Group>& group) {
	while (FindAndParseComment(stream, group)) {}
	while (!stream.eof()) {
		// The first thing we expect to encounter is the item name
		std::string item_name = ParseItemName(stream);
		ExpectEqualSign(stream);
		Item::Type item_type = GuessType(stream);
		switch (item_type) {
			case Item::Type::Integer: {
				auto item = std::make_shared<Integer>(item_name);
				item->SetInteger(ParseIntValue(stream));
				group->Add(item);
				break;
			}
			case Item::Type::String: {
				auto item = std::make_shared<String>(item_name);
				item->SetString(ParseStringValue(stream));
				group->Add(item);
				break;
			}
			case Item::Type::Group: {
				auto item = std::make_shared<Group>(item_name);
				std::istringstream group_stream(ParseGroupContent(stream));
				Parse(group_stream, item);
				group->Add(item);
				break;
			}
			case Item::Type::Double: {
				auto item = std::make_shared<Double>(item_name);
				item->SetDouble(ParseDoubleValue(stream));
				group->Add(item);
				break;
			}
			case Item::Type::Comment:
				// This should never happen but we make compiler happy
				break;
		}
		while (FindAndParseComment(stream, group)) {}
	}
}

std::string File::ParseItemName(std::istream& stream) {
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

void File::ExpectEqualSign(std::istream& stream) {
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

Item::Type File::GuessType(std::istream& stream) {
	ConsumeEmptyChars(stream);
	// We store current position to restore once guessed the type
	auto current_position = stream.tellg();
	Item::Type item_type;
	if (stream.eof())
		throw ParseError("Expected number, \" or { but found EOF");
	// While is to implement in a future Item::Type::Double
	while (!stream.eof()) {
		char c;
		stream.get(c);
		if (std::isdigit(c)) {
			item_type = Item::Type::Integer;
			// We assumed integer but will keep reading until we find a . for double or a ; which confirms it is an integer
			while (!stream.eof()) {
				stream.get(c);
				if (c == '.') {
					item_type = Item::Type::Double;
					break;
				}
				else if (c == ';')
					break;
			}
			break;
		}
		else if (c == '"') {
			item_type = Item::Type::String;
			break;
		}
		else if (c == '{') {
			item_type = Item::Type::Group;
			break;
		}
		else if (c == '#') {
			item_type = Item::Type::Comment;
			break;
		}
		else
			throw ParseError(std::string("Expected number, \" or { but found ") + c);
	}
	stream.seekg(current_position);
	return item_type;
}

int File::ParseIntValue(std::istream& stream) {
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

std::string File::ParseStringValue(std::istream& stream) {
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

std::string File::ParseGroupContent(std::istream& stream) {
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

double File::ParseDoubleValue(std::istream& stream) {
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
		else
			throw ParseError((std::string)"Expected number or ; but got " + c);
	}
	if (accumulator.empty())
		throw ParseError("Expected number but got ; without any");
	
	return std::stod(accumulator);
}

bool File::FindAndParseComment(std::istream& stream, std::shared_ptr<Group>& group) {
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
			group->Add("_comment_", Item::Type::Comment)->SetString(accumulator);
	}
	else {
		// Leave the position before this read
		stream.seekg(-1, std::ios::cur);
	}

	return !accumulator.empty();
}

void File::ConsumeEmptyChars(std::istream& stream) {
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

void File::ExpectSemicolon(std::istream& stream) {
	std::string error;
	if (stream.eof()) error = "Expected ; but found EOF";
	else {
		char c;
		stream.get(c);
		if (c != ';') error = (std::string)"Expected ; but found " + c;
	}

	if (!error.empty())
		throw ParseError(error);
}
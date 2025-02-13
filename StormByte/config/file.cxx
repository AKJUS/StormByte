#include <StormByte/config/exception.hxx>
#include <StormByte/config/file.hxx>
#include <StormByte/config/item/value/bool.hxx>
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
			case Item::Type::Bool:
				auto item = std::make_shared<Bool>(item_name);
				item->SetBool(ParseBoolValue(stream));
				group->Add(item);
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

std::string File::ParseExpValue(std::istream& stream) {
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
			accumulator += std::to_string(ParseIntValue(stream));
			break;
		default:
			throw ParseError(std::string("Expected expontial sign + or - but got ") + c);
			break;
	}
	// ParseIntValue already skips ; so we have finished
	return accumulator;
}

bool File::ParseBoolValue(std::istream& stream) {
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
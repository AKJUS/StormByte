#include <StormByte/logger.hxx>

#include <ostream>
#include <chrono>
#include <iomanip>

using namespace StormByte;

Logger::Logger(std::ostream& out, const Logger::Level& level, const std::string& format) noexcept :
	m_out(out),
	m_print_level(level),
	m_current_level(std::nullopt),
	m_header_displayed(false),
	m_format(format),
	m_human_readable_format(String::Format::Raw) {
}

Logger& Logger::operator<<(const Level& level) noexcept {
	if (m_current_level) {
		if (level != m_current_level && m_current_level >= m_print_level && m_header_displayed) {
			m_out << std::endl;
			m_header_displayed = false;
		}
	}

	m_current_level = level;
	return *this;
}

Logger& Logger::operator<<(std::ostream& (*manip)(std::ostream&)) noexcept {
	if (m_current_level) {
		if (m_current_level >= m_print_level) {
			m_out << manip;
			m_header_displayed = false;
		}
	}

	return *this;
}

void Logger::print_time() const noexcept {
	auto now = std::chrono::system_clock::now();
	std::time_t rawtime = std::chrono::system_clock::to_time_t(now);
	struct tm timeinfo;

#ifdef LINUX
	timeinfo = *std::localtime(&rawtime);
#else
	localtime_s(&timeinfo, &rawtime);
#endif

	m_out << std::put_time(&timeinfo, "%d/%m/%Y %H:%M:%S");
}

void Logger::print_level() const noexcept {
	constexpr std::size_t fixed_width = 8; // Set a fixed width for all level strings
	const std::string level_str = LevelToString(*m_current_level);
	m_out << level_str << std::string(fixed_width - level_str.size(), ' ');
}

void Logger::print_header() const noexcept {
	for (char c : m_format) {
		if (c == '%') {
			continue;
		}
		switch (c) {
		case 'L':
			print_level();
			break;
		case 'T':
			print_time();
			break;
		default:
			m_out << c;
			break;
		}
	}
	m_out << " ";
}

void Logger::print_message(const std::string& message) noexcept {
	if (!m_current_level) m_current_level = m_print_level;
	if (m_current_level >= m_print_level) {
		if (!m_header_displayed) {
			print_header();
			m_header_displayed = true;
		}
		m_out << message;
	}
}

void Logger::print_message(const wchar_t& value) noexcept {
	print_message(String::UTF8Encode(std::wstring(1, value)));
}

namespace StormByte {
	// Explicit instantiation for normalized (decayed) types
	template STORMBYTE_PUBLIC Logger& Logger::operator<<<bool>(const bool& value) noexcept;

	// Numeric types
	template STORMBYTE_PUBLIC Logger& Logger::operator<<<short>(const short& value) noexcept;
	template STORMBYTE_PUBLIC Logger& Logger::operator<<<unsigned short>(const unsigned short& value) noexcept;
	template STORMBYTE_PUBLIC Logger& Logger::operator<<<int>(const int& value) noexcept;
	template STORMBYTE_PUBLIC Logger& Logger::operator<<<unsigned int>(const unsigned int& value) noexcept;
	template STORMBYTE_PUBLIC Logger& Logger::operator<<<long>(const long& value) noexcept;
	template STORMBYTE_PUBLIC Logger& Logger::operator<<<unsigned long>(const unsigned long& value) noexcept;
	template STORMBYTE_PUBLIC Logger& Logger::operator<<<long long>(const long long& value) noexcept;
	template STORMBYTE_PUBLIC Logger& Logger::operator<<<unsigned long long>(const unsigned long long& value) noexcept;

	// Floating-point types
	template STORMBYTE_PUBLIC Logger& Logger::operator<<<float>(const float& value) noexcept;
	template STORMBYTE_PUBLIC Logger& Logger::operator<<<double>(const double& value) noexcept;
	template STORMBYTE_PUBLIC Logger& Logger::operator<<<long double>(const long double& value) noexcept;

	// Character types
	template STORMBYTE_PUBLIC Logger& Logger::operator<<<char>(const char& value) noexcept;
	template STORMBYTE_PUBLIC Logger& Logger::operator<<<unsigned char>(const unsigned char& value) noexcept;
	template STORMBYTE_PUBLIC Logger& Logger::operator<<<wchar_t>(const wchar_t& value) noexcept;

	// String types
	template STORMBYTE_PUBLIC Logger& Logger::operator<<<std::string>(const std::string& value) noexcept;
	template STORMBYTE_PUBLIC Logger& Logger::operator<<<std::wstring>(const std::wstring& value) noexcept;
	template STORMBYTE_PUBLIC Logger& Logger::operator<<<const char*>(const char* const& value) noexcept;
	template STORMBYTE_PUBLIC Logger& Logger::operator<<<const wchar_t*>(const wchar_t* const& value) noexcept;
}
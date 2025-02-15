#include <StormByte/log/logger.hxx>

#include <ostream>
#include <chrono>
#include <iomanip>

using namespace StormByte::Log;

Logger::Logger(std::ostream& out, const Level& level, const std::string& format) noexcept:m_out(out),
m_print_level(level), m_current_level(level), m_line_started(false), m_format(format) {}

Logger& Logger::operator<<(const Level& level) noexcept {
	if (m_current_level >= m_print_level && m_line_started)
		m_out << std::endl;

	m_current_level = level;
	m_line_started = false;
	return *this;
}

Logger& Logger::operator<<(const std::string& str) noexcept {
	print_message(str);
	return *this;
}

Logger& Logger::operator<<(const char* str) noexcept {
	print_message(str);
	return *this;
}

Logger& Logger::operator<<(const int& value) noexcept {
	print_message(std::to_string(value));
	return *this;
}

Logger& Logger::operator<<(const double& value) noexcept {
	print_message(std::to_string(value));
	return *this;
}

Logger& Logger::operator<<(const bool& value) noexcept {
	print_message(value ? "true" : "false");
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
	m_out << GetLevelAsString(m_current_level);
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
}

void Logger::print_message(const std::string& message) noexcept {
	if (m_current_level >= m_print_level) {
		if (!m_line_started) {
			print_header();
			m_line_started = true;
		}
		m_out << message;
	}
}
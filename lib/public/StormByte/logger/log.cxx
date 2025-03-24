#include <StormByte/logger/log.hxx>

#include <ostream>
#include <chrono>
#include <iomanip>

using namespace StormByte::Logger;

Log::Log(std::ostream& out, const Level& level, const std::string& format) noexcept:m_out(out),
m_print_level(level), m_current_level(level), m_line_started(false), m_format(format) {}

Log& Log::operator<<(const Level& level) noexcept {
	if (m_current_level >= m_print_level && m_line_started) {
		m_out << std::endl;
	}
	m_current_level = level;
	m_line_started = false;
	return *this;
}

Log& Log::operator<<(std::ostream& (*manip)(std::ostream&)) noexcept {
    if (m_current_level >= m_print_level) {
		if (m_line_started) {
			m_out << manip;
			m_line_started = false;
		}
	}
    return *this;
}

void Log::print_time() const noexcept {
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

void Log::print_level() const noexcept {
    constexpr std::size_t fixed_width = 8; // Set a fixed width for all level strings
    const std::string level_str = LevelToString(m_current_level);
    
    // Pad the level string with spaces to match the fixed width
    m_out << level_str << std::string(fixed_width - level_str.size(), ' ');
}

void Log::print_header() const noexcept {
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

void Log::print_message(const std::string& message) noexcept {
	if (m_current_level >= m_print_level) {
		if (!m_line_started) {
			print_header();
			m_line_started = true;
		}
		m_out << message;
	}
}

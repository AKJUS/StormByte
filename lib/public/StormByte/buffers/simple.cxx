#include <StormByte/buffers/simple.hxx>

#include <algorithm>
#include <cstring>
#include <format>

using namespace StormByte::Buffers;

Simple::Simple() noexcept : m_data(), m_position(0), m_minimum_chunk_size(0) {}

Simple::Simple(const std::size_t& size): m_data(size), m_position(0), m_minimum_chunk_size(size) {}

Simple::Simple(const char* data, const std::size_t& length): m_data(length), m_position(0), m_minimum_chunk_size(0) {
	if (data != nullptr && length > 0) {
		std::copy(reinterpret_cast<const std::byte*>(data),
				reinterpret_cast<const std::byte*>(data) + length,
				m_data.begin());
	}
}

Simple::Simple(const std::string& data): m_data(data.size()), m_position(0), m_minimum_chunk_size(0) {
	if (!data.empty()) {
		std::copy(reinterpret_cast<const std::byte*>(data.data()),
				reinterpret_cast<const std::byte*>(data.data()) + data.size(),
				m_data.begin());
	}
}

Simple::Simple(const Buffers::Data& data): m_data(data), m_position(0), m_minimum_chunk_size(0) {}

Simple::Simple(Buffers::Data&& data): m_data(std::move(data)), m_position(0) {}

Simple::Simple(const std::span<const Byte>& data): m_data(data.begin(), data.end()), m_position(0), m_minimum_chunk_size(0) {}

bool Simple::operator==(const Simple& other) const {
	return m_data == other.m_data;
}

bool Simple::operator!=(const Simple& other) const {
	return !(*this == other);
}

Simple& Simple::operator<<(const Simple& buffer) {
	if (this != &buffer) {
		EnsureCapacity(buffer.m_data.size());
		m_data.insert(m_data.end(), buffer.m_data.begin(), buffer.m_data.end());
	}
	return *this;
}

Simple& Simple::operator<<(Simple&& buffer) {
	if (this != &buffer) {
		EnsureCapacity(buffer.m_data.size());
		m_data.insert(m_data.end(),
					std::make_move_iterator(buffer.m_data.begin()),
					std::make_move_iterator(buffer.m_data.end()));
	}
	return *this;
}

Simple& Simple::operator<<(const std::string& data) {
	EnsureCapacity(data.size());
	m_data.insert(m_data.end(),
				reinterpret_cast<const std::byte*>(data.data()),
				reinterpret_cast<const std::byte*>(data.data()) + data.size());
	return *this;
}

Simple& Simple::operator<<(const Buffers::Data& data) {
	EnsureCapacity(data.size());
	m_data.insert(m_data.end(), data.begin(), data.end());
	return *this;
}

Simple& Simple::operator<<(Buffers::Data&& data) {
	EnsureCapacity(data.size());
	m_data.insert(m_data.end(),
				std::make_move_iterator(data.begin()),
				std::make_move_iterator(data.end()));
	return *this;
}

Simple& Simple::operator>>(Simple& buffer) {
	if (this != &buffer) {
		buffer.EnsureCapacity(m_data.size());
		buffer.m_data.insert(buffer.m_data.end(),
							m_data.begin() + m_position,
							m_data.end());
		m_position = m_data.size();
	}
	return buffer;
}

size_t Simple::Capacity() const noexcept {
	return m_data.capacity();
}

void Simple::Clear() {
	m_data.clear();
	m_position = 0;
}

StormByte::Buffers::Data Simple::Data() const noexcept {
	return m_data;
}

void Simple::Discard() noexcept {
	m_data.erase(m_data.begin(), m_data.begin() + m_position);
	m_data.shrink_to_fit();
	m_position = 0;
}

bool Simple::Empty() const noexcept {
	return m_data.empty();
}

bool Simple::End() const noexcept {
	return m_position == m_data.size();
}

ExpectedData<BufferOverflow> Simple::Extract(const std::size_t& length) {
	if (m_position + length > m_data.size()) {
		return StormByte::Unexpected<BufferOverflow>(
			"Insufficient data to read {} bytes (only have {} bytes)", length, m_data.size() - m_position
		);
	}

	auto start = m_data.begin() + m_position;
	auto end = start + length;

	Buffers::Data extracted_data(std::make_move_iterator(start), std::make_move_iterator(end));
	m_position += length;

	Discard();

	return extracted_data;
}

bool Simple::HasEnoughData(const std::size_t& length) const {
	return m_position + length <= m_data.size();
}

std::string Simple::HexData(const std::size_t& column_size) const {
	std::string hex_data;
	std::string hex_line, char_line;
	std::size_t line_char_count = 0;

	for (const auto& byte : m_data) {
		hex_line += std::format("{:02X} ", static_cast<unsigned char>(byte));
		char_line += std::isprint(static_cast<char>(byte)) ? static_cast<char>(byte) : '.';
		if (++line_char_count == column_size) {
			std::size_t padding = (column_size * 3) - hex_line.size();
			hex_data += hex_line + std::string(padding, ' ') + "\t" + char_line + "\n";
			hex_line.clear();
			char_line.clear();
			line_char_count = 0;
		}
	}

	if (!hex_line.empty()) {
		std::size_t padding = (column_size * 3) - hex_line.size();
		hex_data += hex_line + std::string(padding, ' ') + "\t" + char_line + "\n";
	}

	return hex_data;
}

std::size_t Simple::Position() const noexcept {
	return m_position;
}

ExpectedData<BufferOverflow> Simple::Read(const std::size_t& length) const {
	if (m_position + length > m_data.size()) {
		return StormByte::Unexpected<BufferOverflow>(
			"Insufficient data to read {} bytes (only have {} bytes)",
			length,
			m_data.size() - m_position);
	}
	auto start = m_data.begin() + m_position;
	auto end = start + length;

	Buffers::Data read_data(start, end); // Create a copy of the requested data
	m_position += length; // Advance the read position

	return read_data;
}

void Simple::Reserve(const std::size_t& size) {
	m_data.reserve(size);
}

void Simple::Seek(const std::ptrdiff_t& position, const Read::Position& mode) const {
	std::size_t new_position = 0;

	if (static_cast<unsigned short>(mode) & static_cast<unsigned short>(Read::Position::Begin)) {
		new_position = static_cast<std::size_t>(std::max<ptrdiff_t>(0, position));
	}
	if (static_cast<unsigned short>(mode) & static_cast<unsigned short>(Read::Position::End)) {
		new_position = static_cast<std::size_t>(
			std::max<ptrdiff_t>(0, static_cast<ptrdiff_t>(m_data.size()) + position));
	}
	if (static_cast<unsigned short>(mode) & static_cast<unsigned short>(Read::Position::Relative)) {
		new_position = static_cast<std::size_t>(
			std::max<ptrdiff_t>(0, static_cast<ptrdiff_t>(m_position) + position));
	}
	if (static_cast<unsigned short>(mode) & static_cast<unsigned short>(Read::Position::Absolute)) {
		new_position = static_cast<std::size_t>(std::max<ptrdiff_t>(0, position));
	}

	m_position = new_position;
}

std::size_t Simple::Size() const noexcept {
	return m_data.size();
}

const std::span<const Byte> Simple::Span() const noexcept {
	return std::span<const Byte>(m_data.data(), m_data.size());
}

std::span<Byte> Simple::Span() noexcept {
	return std::span<Byte>(m_data.data(), m_data.size());
}

ExpectedByte<BufferOverflow> Simple::Peek() const {
	if (m_position >= m_data.size()) {
		return StormByte::Unexpected<BufferOverflow>(
			"Cannot peek: no more data available in the buffer."
		);
	}
	return m_data[m_position];
}

void Simple::EnsureCapacity(const std::size_t& size) {
	const std::size_t new_size = m_data.size() + size;
	if (m_minimum_chunk_size > 0 && m_data.capacity() < new_size) {
		const std::size_t new_capacity = (new_size / m_minimum_chunk_size + 1) * m_minimum_chunk_size;
		m_data.reserve(new_capacity);
	}
}
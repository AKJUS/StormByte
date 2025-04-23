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

Simple& Simple::operator<<(const Simple& buffer) {
	Write(buffer);
	return *this;
}

Simple& Simple::operator<<(Simple&& buffer) {
	Write(std::move(buffer));
	return *this;
}

Simple& Simple::operator<<(const std::string& data) {
	Write(data);
	return *this;
}

Simple& Simple::operator<<(const Buffers::Data& data) {
	Write(data);
	return *this;
}

Simple& Simple::operator<<(Buffers::Data&& data) {
	Write(std::move(data));
	return *this;
}

Simple& Simple::operator>>(Simple& buffer) {
	if (this != &buffer) {
		buffer.Write(m_data);
		m_position = m_data.size();
	}
	return buffer;
}

size_t Simple::AvailableBytes() const noexcept {
	return m_data.size() - m_position;
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

void Simple::Discard(const std::size_t& length, const Read::Position& mode) noexcept {
	std::size_t discard_start = 0;

	// Determine the starting position for the discard operation based on the mode
	if (static_cast<unsigned short>(mode) & static_cast<unsigned short>(Read::Position::Begin)) {
		discard_start = 0; // Discard from the beginning
	} else if (static_cast<unsigned short>(mode) & static_cast<unsigned short>(Read::Position::End)) {
		discard_start = (length > m_data.size()) ? 0 : m_data.size() - length; // Discard from the end
	} else if (static_cast<unsigned short>(mode) & static_cast<unsigned short>(Read::Position::Relative)) {
		discard_start = m_position; // Discard relative to the current position
	} else if (static_cast<unsigned short>(mode) & static_cast<unsigned short>(Read::Position::Absolute)) {
		discard_start = (length > m_data.size()) ? m_data.size() : 0; // Discard from the absolute position
	}

	// Ensure the discard range is valid
	discard_start = std::min(discard_start, m_data.size());
	const std::size_t discard_end = std::min(discard_start + length, m_data.size());

	// Perform the discard operation
	if (discard_start < discard_end) {
		m_data.erase(m_data.begin() + discard_start, m_data.begin() + discard_end);
	}

	// Adjust the read position if necessary
	if (m_position >= discard_end) {
		m_position -= (discard_end - discard_start); // Adjust position if it's after the discarded range
	} else if (m_position >= discard_start) {
		m_position = discard_start; // Adjust position to the start of the discarded range
	}
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

	Discard(length, Read::Position::Relative);

	return extracted_data;
}

Read::Status Simple::ExtractInto(const std::size_t& length, Simple& output) noexcept {
	if (!HasEnoughData(length)) {
		return Read::Status::Error;
	}

	// Lock the data to extract
	auto start = m_data.begin() + m_position;
	auto end = start + length;

	// Move the data directly into the output buffer
	output.m_data.reserve(output.m_data.size() + length);
	output.m_data.insert(output.m_data.end(),
						std::make_move_iterator(start),
						std::make_move_iterator(end));

	// Use Discard to remove the extracted data
	Discard(length, Read::Position::Relative);

	return Read::Status::Success;
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

bool Simple::IsEoF() const noexcept {
	return m_position >= m_data.size();
}

std::size_t Simple::Position() const noexcept {
	return m_position;
}

Read::Status Simple::Process(const std::size_t& length, Processor function, Simple& output) noexcept {
	if (!HasEnoughData(length)) {
		return Read::Status::Error;
	}

	auto data = Read(length);
	if (!data) {
		return Read::Status::Error;
	}

	std::shared_ptr<Simple> result = function(data.value());
	if (!result) {
		return Read::Status::Error;
	}
	output.Write(std::move(*result));

	Discard(m_position, Read::Position::Absolute);

	return Read::Status::Success;
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

Write::Status Simple::Write(const Simple& buffer) {
	if (this != &buffer) {
		m_data.reserve(m_data.size() + buffer.m_data.size());
		m_data.insert(m_data.end(), buffer.m_data.begin(), buffer.m_data.end());
	}
	return Write::Status::Success;
}

Write::Status Simple::Write(Simple&& buffer) {
	if (this != &buffer) {
		m_data.reserve(m_data.size() + buffer.m_data.size());
		m_data.insert(m_data.end(),
					std::make_move_iterator(buffer.m_data.begin()),
					std::make_move_iterator(buffer.m_data.end()));
	}
	return Write::Status::Success;
}

Write::Status Simple::Write(const std::string& data) {
	m_data.reserve(m_data.size() + data.size());
	m_data.insert(m_data.end(),
				reinterpret_cast<const std::byte*>(data.data()),
				reinterpret_cast<const std::byte*>(data.data()) + data.size());
	return Write::Status::Success;
}

Write::Status Simple::Write(const Buffers::Data& data) {
	m_data.reserve(m_data.size() + data.size());
	m_data.insert(m_data.end(), data.begin(), data.end());
	return Write::Status::Success;
}

Write::Status Simple::Write(Buffers::Data&& data) {
	m_data.reserve(m_data.size() + data.size());
	m_data.insert(m_data.end(),
				std::make_move_iterator(data.begin()),
				std::make_move_iterator(data.end()));
	return Write::Status::Success;
}
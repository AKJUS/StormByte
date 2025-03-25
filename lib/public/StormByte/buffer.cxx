#include <StormByte/buffer.hxx>

#include <algorithm>
#include <cstring>
#include <format>

using namespace StormByte;

Buffer::Buffer(const char* data, const std::size_t& length): m_data(length), m_position(0) {
	if (data != nullptr && length > 0) {
		std::copy(reinterpret_cast<const std::byte*>(data), 
				reinterpret_cast<const std::byte*>(data) + length, 
				m_data.begin());
	}
}

Buffer::Buffer(const std::string& data): m_data(data.size()), m_position(0) {
	if (!data.empty())
		std::copy(reinterpret_cast<const std::byte*>(data.data()), 
				reinterpret_cast<const std::byte*>(data.data()) + data.size(), 
				m_data.begin());
}

Buffer::Buffer(const DataType& data): m_data(data), m_position(0) {}

Buffer::Buffer(DataType&& data): m_data(std::move(data)), m_position(0) {}

Buffer::Buffer(const std::span<const Byte>& data): m_data(data.begin(), data.end()), m_position(0) {}

Buffer::ExpectedConstByte Buffer::operator[](const std::size_t& index) const {
	if (index >= m_data.size())
		return StormByte::Unexpected<BufferOverflow>("Index {} is out of bounds (size: {})", index, m_data.size());
	return m_data[index];
}

Buffer& Buffer::operator<<(const Buffer& buffer) {
	if (this != &buffer)
		m_data.insert(m_data.end(), buffer.m_data.begin(), buffer.m_data.end());
	return *this;
}

Buffer& Buffer::operator<<(Buffer&& buffer) {
	if (this != &buffer)
		m_data.insert(m_data.end(), std::make_move_iterator(buffer.m_data.begin()), std::make_move_iterator(buffer.m_data.end()));
	return *this;
}

Buffer& Buffer::operator<<(const std::string& data) {
	m_data.insert(m_data.end(), reinterpret_cast<const std::byte*>(data.data()), reinterpret_cast<const std::byte*>(data.data()) + data.size());
	return *this;
}

Buffer& Buffer::operator<<(const DataType& data) {
	m_data.insert(m_data.end(), data.begin(), data.end());
	return *this;
}

Buffer& Buffer::operator<<(DataType&& data) {
	m_data.insert(m_data.end(), std::make_move_iterator(data.begin()), std::make_move_iterator(data.end()));
	return *this;
}

Buffer& Buffer::operator>>(Buffer& buffer) {
	if (this != &buffer) {
		buffer.m_data.insert(buffer.m_data.end(), m_data.begin() + m_position, m_data.end());
		m_position = m_data.size();
	}
	return buffer;
}

std::string Buffer::HexData(const std::size_t& column_size) const {
	std::string hex_data;
	std::string hex_line, char_line;
	std::size_t line_char_count = 0;

	for (const auto& byte : m_data) {
		hex_line += std::format("{:02X} ", static_cast<unsigned char>(byte));
		char_line += std::isprint(static_cast<char>(byte)) ? static_cast<char>(byte) : '.';
		if (++line_char_count == column_size) {
			// Calculate padding
			std::size_t padding = (column_size * 3) - hex_line.size();
			hex_data += hex_line + std::string(padding, ' ') + "\t" + char_line + "\n";
			hex_line.clear();
			char_line.clear();
			line_char_count = 0;
		}
	}

	// Handle any remaining data
	if (!hex_line.empty()) {
		// Calculate padding
		std::size_t padding = (column_size * 3) - hex_line.size();
		hex_data += hex_line + std::string(padding, ' ') + "\t" + char_line + "\n";
	}

	return hex_data;
}

Buffer::ExpectedConstByteSpan Buffer::Read(const std::size_t& length) const {
	if (m_position + length > m_data.size())
		return StormByte::Unexpected<BufferOverflow>("Insufficient data to read {} bytes (only have {} bytes)", length, m_data.size() - m_position);
	std::span<const Byte> data(m_data.data() + m_position, length);
	m_position += length;
	return data;
}

void Buffer::Seek(const std::ptrdiff_t& position, const ReadPosition& mode) const {
	if (static_cast<unsigned short>(mode) & static_cast<unsigned short>(ReadPosition::Begin))
		m_position = static_cast<std::size_t>(std::max<ptrdiff_t>(0, position));

	if (static_cast<unsigned short>(mode) & static_cast<unsigned short>(ReadPosition::End))
		m_position = static_cast<std::size_t>(std::max<ptrdiff_t>(0, static_cast<ptrdiff_t>(m_data.size()) + position));

	if (static_cast<unsigned short>(mode) & static_cast<unsigned short>(ReadPosition::Relative))
		m_position = static_cast<std::size_t>(std::max<ptrdiff_t>(0, static_cast<ptrdiff_t>(m_position) + position));

	if (static_cast<unsigned short>(mode) & static_cast<unsigned short>(ReadPosition::Absolute))
		m_position = static_cast<std::size_t>(std::max<ptrdiff_t>(0, position));
}
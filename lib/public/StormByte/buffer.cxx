#include <StormByte/buffer.hxx>
#include <algorithm>
#include <cstring>
#include <format>
#include <atomic>
#include <mutex>

using namespace StormByte;

Buffer::Buffer() noexcept : m_position(0) {}

Buffer::Buffer(const std::size_t& size) : m_data(size), m_position(0) {}

Buffer::Buffer(const char* data, const std::size_t& length) : m_data(length), m_position(0) {
	if (data != nullptr && length > 0) {
		std::copy(reinterpret_cast<const std::byte*>(data),
				reinterpret_cast<const std::byte*>(data) + length,
				m_data.begin());
	}
}

Buffer::Buffer(const std::string& data) : m_data(data.size()), m_position(0) {
	if (!data.empty()) {
		std::copy(reinterpret_cast<const std::byte*>(data.data()),
				reinterpret_cast<const std::byte*>(data.data()) + data.size(),
				m_data.begin());
	}
}

Buffer::Buffer(const DataType& data) : m_data(data), m_position(0) {}

Buffer::Buffer(DataType&& data) : m_data(std::move(data)), m_position(0) {}

Buffer::Buffer(const std::span<const Byte>& data) : m_data(data.begin(), data.end()), m_position(0) {}

Buffer::Buffer(const Buffer& other) : m_data(other.m_data), m_position(other.m_position.load()) {}

Buffer::Buffer(Buffer&& other) noexcept {
	std::unique_lock lock(other.m_data_mutex); // Lock to ensure thread-safety for other.m_data
	m_data = std::move(other.m_data);
	m_position.store(other.m_position.load());
	other.m_position.store(0);
}

Buffer& Buffer::operator=(const Buffer& other) {
	if (this != &other) {
		std::unique_lock lock(m_data_mutex); // Lock to ensure thread-safety for m_data
		std::unique_lock other_lock(const_cast<Buffer&>(other).m_data_mutex); // Lock to ensure thread-safety for other.m_data
		m_data = other.m_data;
		m_position.store(other.m_position.load());
	}
	return *this;
}

Buffer& Buffer::operator=(Buffer&& other) noexcept {
	if (this != &other) {
		std::unique_lock lock(m_data_mutex); // Lock to ensure thread-safety for m_data
		std::unique_lock other_lock(other.m_data_mutex); // Lock to ensure thread-safety for other.m_data
		m_data = std::move(other.m_data);
		m_position.store(other.m_position.load());
		other.m_position.store(0);
	}
	return *this;
}

bool Buffer::operator==(const Buffer& other) const {
	std::shared_lock lock(m_data_mutex); // Lock to ensure thread-safety for m_data
	std::shared_lock other_lock(other.m_data_mutex); // Lock to ensure thread-safety for other.m_data
	return m_data == other.m_data;
}

bool Buffer::operator!=(const Buffer& other) const {
	return !(*this == other);
}

Buffer::ExpectedConstByte Buffer::operator[](const std::size_t& index) const {
	std::shared_lock lock(m_data_mutex); // Lock to ensure thread-safety for m_data
	if (index >= m_data.size()) {
		return StormByte::Unexpected<BufferOverflow>("Index {} is out of bounds (size: {})", index, m_data.size());
	}
	return m_data[index];
}

void Buffer::Clear() {
	std::unique_lock lock(m_data_mutex); // Lock to ensure thread-safety for m_data
	m_data.clear();
	m_position.store(0);
}

const std::span<const Buffer::Byte>	Buffer::Data() const noexcept {
	std::shared_lock lock(m_data_mutex); // Lock to ensure thread-safety for m_data
	return std::span<const Byte>(m_data.data(), m_data.size());
}

std::span<Buffer::Byte>	Buffer::Data() noexcept {
	std::shared_lock lock(m_data_mutex); // Lock to ensure thread-safety for m_data
	return std::span<Byte>(m_data.data(), m_data.size());
}

Buffer& Buffer::operator<<(const Buffer& buffer) {
	std::unique_lock lock(m_data_mutex); // Lock for thread-safety
	std::shared_lock otherlock(buffer.m_data_mutex); // Lock for thread-safety
	if (this != &buffer) {
		m_data.reserve(m_data.size() + buffer.m_data.size());
		m_data.insert(m_data.end(), buffer.m_data.begin(), buffer.m_data.end());
	}
	return *this;
}

Buffer& Buffer::operator<<(Buffer&& buffer) {
	std::unique_lock lock(m_data_mutex); // Lock for thread-safety
	std::unique_lock other_lock(buffer.m_data_mutex); // Lock for thread-safety
	if (this != &buffer) {
		m_data.reserve(m_data.size() + buffer.m_data.size());
		m_data.insert(m_data.end(),
					std::make_move_iterator(buffer.m_data.begin()),
					std::make_move_iterator(buffer.m_data.end()));
	}
	return *this;
}

Buffer& Buffer::operator<<(const std::string& data) {
	std::unique_lock lock(m_data_mutex); // Thread-safe insertion
	m_data.reserve(m_data.size() + data.size());
	m_data.insert(m_data.end(),
				reinterpret_cast<const std::byte*>(data.data()),
				reinterpret_cast<const std::byte*>(data.data()) + data.size());
	return *this;
}

Buffer& Buffer::operator<<(const DataType& data) {
	std::unique_lock lock(m_data_mutex); // Thread-safe insertion
	m_data.reserve(m_data.size() + data.size());
	m_data.insert(m_data.end(), data.begin(), data.end());
	return *this;
}

Buffer& Buffer::operator<<(DataType&& data) {
	std::unique_lock lock(m_data_mutex); // Thread-safe insertion
	m_data.reserve(m_data.size() + data.size());
	m_data.insert(m_data.end(),
				std::make_move_iterator(data.begin()),
				std::make_move_iterator(data.end()));
	return *this;
}

Buffer& Buffer::operator>>(Buffer& buffer) {
	std::shared_lock lock(m_data_mutex); // Lock for thread-safety
	std::unique_lock otherlock(buffer.m_data_mutex); // Lock for thread-safety
	if (this != &buffer) {
		buffer.m_data.insert(buffer.m_data.end(),
							m_data.begin() + m_position.load(),
							m_data.end());
		m_position.store(m_data.size());
	}
	return buffer;
}

std::string Buffer::HexData(const std::size_t& column_size) const {
	std::string hex_data;
	std::string hex_line, char_line;
	std::size_t line_char_count = 0;

	{
		std::shared_lock lock(m_data_mutex); // Lock for thread-safety
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
	}

	if (!hex_line.empty()) {
		std::size_t padding = (column_size * 3) - hex_line.size();
		hex_data += hex_line + std::string(padding, ' ') + "\t" + char_line + "\n";
	}

	return hex_data;
}

bool Buffer::HasEnoughData(const std::size_t& length) const {
	std::shared_lock lock(m_data_mutex); // Lock for thread-safety
	return m_position.load() + length <= m_data.size();
}

Buffer::ExpectedConstByteSpan Buffer::Read(const std::size_t& length) const {
	std::shared_lock lock(m_data_mutex); // Lock for thread-safety
	std::size_t current_pos = m_position.load();
	while (current_pos + length <= m_data.size() &&
		!m_position.compare_exchange_weak(current_pos, current_pos + length)) {
		// Retry atomic update
	}
	if (current_pos + length > m_data.size()) {
		return StormByte::Unexpected<BufferOverflow>(
			"Insufficient data to read {} bytes (only have {} bytes)",
			length,
			m_data.size() - m_position);
	}
	return std::span<const Byte>(m_data.data() + current_pos, length);
}

Buffer::ExpectedDataType Buffer::Extract(const std::size_t& length) {
	std::unique_lock lock(m_data_mutex); // Lock for thread-safety

	// Check if there is enough data to read
	if (m_position.load() + length > m_data.size()) {
		return StormByte::Unexpected<BufferOverflow>(
			"Insufficient data to read {} bytes (only have {} bytes)", length, m_data.size() - m_position.load()
		);
	}

	// Calculate the range of data to extract
	auto start = m_data.begin() + m_position.load();
	auto end = start + length;

	// Move the data into a new vector
	DataType extracted_data(std::make_move_iterator(start), std::make_move_iterator(end));

	// Erase the moved-from elements from the buffer
	m_data.erase(start, end);

	// The read position does not need to be updated as the data before the current position is removed.

	return extracted_data;
}

void Buffer::Reserve(const std::size_t& size) {
	std::unique_lock lock(m_data_mutex); // Lock to ensure consistent state
	m_data.reserve(size);
}

void Buffer::Seek(const std::ptrdiff_t& position, const ReadPosition& mode) const {
	std::size_t new_position = 0;

	if (static_cast<unsigned short>(mode) & static_cast<unsigned short>(ReadPosition::Begin)) {
		new_position = static_cast<std::size_t>(std::max<ptrdiff_t>(0, position));
	}
	if (static_cast<unsigned short>(mode) & static_cast<unsigned short>(ReadPosition::End)) {
		std::shared_lock lock(m_data_mutex); // Lock to ensure consistent state
		new_position = static_cast<std::size_t>(
			std::max<ptrdiff_t>(0, static_cast<ptrdiff_t>(m_data.size()) + position));
	}
	if (static_cast<unsigned short>(mode) & static_cast<unsigned short>(ReadPosition::Relative)) {
		new_position = static_cast<std::size_t>(
			std::max<ptrdiff_t>(0, static_cast<ptrdiff_t>(m_position.load()) + position));
	}
	if (static_cast<unsigned short>(mode) & static_cast<unsigned short>(ReadPosition::Absolute)) {
		new_position = static_cast<std::size_t>(std::max<ptrdiff_t>(0, position));
	}

	m_position.store(new_position);
}

std::size_t Buffer::Size() const noexcept {
	std::shared_lock lock(m_data_mutex); // Lock to ensure consistent state
	return m_data.size();
}

bool Buffer::Empty() const noexcept {
	std::shared_lock lock(m_data_mutex); // Lock to ensure consistent state
	return m_data.empty();
}

bool Buffer::End() const noexcept {
	std::shared_lock lock(m_data_mutex); // Lock to ensure consistent state
	return m_position.load() == m_data.size();
}

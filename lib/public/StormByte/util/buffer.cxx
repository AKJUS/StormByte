#include <StormByte/util/buffer.hxx>

#include <cstring>

using namespace StormByte::Util;

Buffer::Buffer() noexcept: m_buffer(nullptr), m_length(0) {}

Buffer::Buffer(const Buffer& other): m_buffer(nullptr), m_length(0) {
	Append(other.m_buffer, other.m_length);
}

Buffer::Buffer(Buffer&& other) noexcept {
	m_buffer = other.m_buffer;
	m_length = other.m_length;
	other.m_buffer = nullptr;
	other.m_length = 0;
}

Buffer& Buffer::operator=(const Buffer& other) {
	if (this != &other) {
		Clear();
		Append(other.m_buffer, other.m_length);
	}
	return *this;
}

Buffer& Buffer::operator=(Buffer&& other) noexcept {
	if (this != &other) {
		Clear();
		m_buffer = other.m_buffer;
		m_length = other.m_length;
		other.m_buffer = nullptr;
		other.m_length = 0;
	}
	return *this;
}

Buffer::~Buffer() noexcept {
	Clear();
}

Buffer Buffer::operator+(const Buffer& other) const {
	Buffer result(*this);
	result.Append(other.m_buffer, other.m_length);
	return result;
}

Buffer& Buffer::operator+=(const Buffer& other) {
	Append(other.m_buffer, other.m_length);
	return *this;
}

void Buffer::Append(const char* data, const std::size_t& length) {
	if (data == nullptr || length == 0) {
		return; // Do nothing if the input is invalid
	}

	if (m_buffer == nullptr) {
		// Allocate new buffer if m_buffer is uninitialized
		m_buffer = new char[length];
		std::memcpy(m_buffer, data, length);
		m_length = length; // Update length after initialization
	} else {
		// Allocate new buffer for the combined size
		char* new_buffer = new char[m_length + length];

		// Copy old data into the new buffer
		std::memcpy(new_buffer, m_buffer, m_length);

		// Append new data
		std::memcpy(new_buffer + m_length, data, length);

		// Free the old buffer and replace it with the new one
		delete[] m_buffer;
		m_buffer = new_buffer;

		// Update the length
		m_length += length;
	}
}

void Buffer::Clear() {
	if (m_buffer != nullptr) {
		delete[] m_buffer;
		m_buffer = nullptr;
		m_length = 0;
	}
}

const char* Buffer::Data() const {
	return m_buffer;
}

const std::size_t& Buffer::Length() const {
	return m_length;
}
#include <StormByte/buffers/shared.hxx>

#include <algorithm>

using namespace StormByte::Buffers;

Shared::Shared() noexcept: Simple() {}

Shared::Shared(const std::size_t& size): Simple(size) {}

Shared::Shared(const char* data, const std::size_t& length): Simple(data, length) {}

Shared::Shared(const std::string& data): Simple(data) {}

Shared::Shared(const Buffers::Data& data): Simple(data) {}

Shared::Shared(Buffers::Data&& data): Simple(std::move(data)) {}

Shared::Shared(const std::span<const Byte>& data): Simple(data) {}

Shared::Shared(const Shared& other): Simple(other) {}

Shared::Shared(Shared&& other) noexcept: Simple(std::move(other)) {}

Shared& Shared::operator=(const Shared& other) {
	if (this != &other) {
		std::unique_lock lock(m_data_mutex);
		std::unique_lock other_lock(other.m_data_mutex);
		Simple::operator=(other);
	}
	return *this;
}

Shared& Shared::operator=(Shared&& other) noexcept {
	if (this != &other) {
		std::unique_lock lock(m_data_mutex);
		std::unique_lock other_lock(other.m_data_mutex);
		Simple::operator=(std::move(other));
	}
	return *this;
}

Shared& Shared::operator<<(const Simple& buffer) {
	std::unique_lock lock(m_data_mutex);
	Simple::operator<<(buffer);
	return *this;
}

Shared& Shared::operator<<(Simple&& buffer) {
	std::unique_lock lock(m_data_mutex);
	Simple::operator<<(std::move(buffer));
	return *this;
}

Shared& Shared::operator<<(const std::string& data) {
	std::unique_lock lock(m_data_mutex);
	Simple::operator<<(data);
	return *this;
}

Shared& Shared::operator<<(const Buffers::Data& data) {
	std::unique_lock lock(m_data_mutex);
	Simple::operator<<(data);
	return *this;
}

Shared& Shared::operator<<(Buffers::Data&& data) {
	std::unique_lock lock(m_data_mutex);
	Simple::operator<<(std::move(data));
	return *this;
}

Shared& Shared::operator>>(Shared& buffer) {
	std::shared_lock lock(m_data_mutex);
	std::unique_lock other_lock(buffer.m_data_mutex);
	Simple::operator>>(buffer);
	return *this;
}

size_t Shared::Capacity() const noexcept {
	std::shared_lock lock(m_data_mutex);
	return Simple::Capacity();
}

void Shared::Clear() noexcept {
	std::unique_lock lock(m_data_mutex);
	Simple::Clear(); // Call base class Clear method
}

Data Shared::Data() const noexcept {
	std::shared_lock lock(m_data_mutex);
	return Simple::Data();
}

void Shared::Discard(const std::size_t& length, const Read::Position& mode) noexcept {
	std::unique_lock lock(m_data_mutex);
	Simple::Discard(length, mode);
}

bool Shared::Empty() const noexcept {
	std::shared_lock lock(m_data_mutex);
	return Simple::Empty();
}

bool Shared::End() const noexcept {
	std::shared_lock lock(m_data_mutex);
	return Simple::End();
}

void Shared::Lock() {
	m_data_mutex.lock();
}

ExpectedData<BufferOverflow> Shared::Extract(const std::size_t& length) {
	std::unique_lock lock(m_data_mutex);
	auto expected_data = Simple::Read(length);
	if (expected_data) {
		Simple::Discard(0, Read::Position::Relative);
	}
	return expected_data;
}

bool Shared::HasEnoughData(const std::size_t& length) const {
	std::shared_lock lock(m_data_mutex);
	return Simple::HasEnoughData(length);
}

std::string Shared::HexData(const std::size_t& column_size) const {
	std::shared_lock lock(m_data_mutex);
	return Simple::HexData(column_size);
}

ExpectedByte<BufferOverflow> Shared::Peek() const {
	std::shared_lock lock(m_data_mutex);
	return Simple::Peek();
}

std::size_t Shared::Position() const noexcept {
	std::shared_lock lock(m_data_mutex);
	return Simple::Position();
}

ExpectedData<BufferOverflow> Shared::Read(const std::size_t& length) const {
	std::shared_lock lock(m_data_mutex);
	return Simple::Read(length);
}

void Shared::Reserve(const std::size_t& size) {
	std::unique_lock lock(m_data_mutex);
	Simple::Reserve(size);
}

void Shared::Seek(const std::ptrdiff_t& position, const Read::Position& mode) const {
	std::unique_lock lock(m_data_mutex);
	Simple::Seek(position, mode);
}

std::size_t Shared::Size() const noexcept {
	std::shared_lock lock(m_data_mutex);
	return Simple::Size();
}

void Shared::Unlock() {
	m_data_mutex.unlock();
}

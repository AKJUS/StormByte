#include <StormByte/buffers/shared.hxx>

#include <algorithm>
#include <thread>

using namespace StormByte::Buffers;

Shared::Shared() noexcept: Simple(), m_status(Status::Ready) {}

Shared::Shared(const std::size_t& size): Simple(size), m_status(Status::Ready) {}

Shared::Shared(const char* data, const std::size_t& length): Simple(data, length), m_status(Status::Ready) {}

Shared::Shared(const std::string& data): Simple(data), m_status(Status::Ready) {}

Shared::Shared(const Buffers::Data& data): Simple(data), m_status(Status::Ready) {}

Shared::Shared(Buffers::Data&& data): Simple(std::move(data)), m_status(Status::Ready) {}

Shared::Shared(const std::span<const Byte>& data): Simple(data), m_status(Status::Ready) {}

Shared::Shared(const Shared& other): Simple(other), m_status(Status::Ready) {}

Shared::Shared(Shared&& other) noexcept: Simple(std::move(other)), m_status(other.m_status.load()) {}

Shared& Shared::operator=(const Shared& other) {
    if (this != &other) {
        std::unique_lock lock(m_data_mutex);
        std::unique_lock other_lock(other.m_data_mutex);
        Simple::operator=(other);
        m_status.store(other.m_status.load());
    }
    return *this;
}

Shared& Shared::operator=(Shared&& other) noexcept {
    if (this != &other) {
        std::unique_lock lock(m_data_mutex);
        std::unique_lock other_lock(other.m_data_mutex);
        Simple::operator=(std::move(other));
        m_status.store(other.m_status.load());
    }
    return *this;
}

Shared& Shared::operator<<(const enum Status& status) {
	m_status.store(status);
	return *this;
}

Shared& Shared::operator<<(const Simple& buffer) {
    Write(buffer);
    return *this;
}

Shared& Shared::operator<<(Simple&& buffer) {
    Write(std::move(buffer));
    return *this;
}

Shared& Shared::operator<<(const std::string& data) {
    Write(data);
    return *this;
}

Shared& Shared::operator<<(const Buffers::Data& data) {
    Write(data);
    return *this;
}

Shared& Shared::operator<<(Buffers::Data&& data) {
    Write(std::move(data));
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
    Simple::Clear();
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
    auto expected_data = ReadWaiting(length);
    if (expected_data) {
        std::unique_lock lock(m_data_mutex);
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
   return ReadWaiting(length);
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

enum Status Shared::Status() const noexcept {
    return m_status.load();
}

void Shared::Unlock() {
    m_data_mutex.unlock();
}

Write::Status Shared::Write(const Buffers::Data& data) {
    if (IsEoF()) {
        return Write::Status::Error;
    }
    std::unique_lock lock(m_data_mutex);
    return Simple::Write(data);
}

Write::Status Shared::Write(Buffers::Data&& data) {
    if (IsEoF()) {
        return Write::Status::Error;
    }
    std::unique_lock lock(m_data_mutex);
    return Simple::Write(std::move(data));
}

Write::Status Shared::Write(const Simple& buffer) {
    if (IsEoF()) {
        return Write::Status::Error;
    }
    std::unique_lock lock(m_data_mutex);
    return Simple::Write(buffer);
}

Write::Status Shared::Write(Simple&& buffer) {
    if (IsEoF()) {
        return Write::Status::Error;
    }
    std::unique_lock lock(m_data_mutex);
    return Simple::Write(std::move(buffer));
}

Write::Status Shared::Write(const std::string& data) {
    if (IsEoF()) {
        return Write::Status::Error;
    }
    std::unique_lock lock(m_data_mutex);
    return Simple::Write(data);
}

ExpectedData<BufferOverflow> Shared::ReadWaiting(const std::size_t& length) const noexcept {
    if (HasEnoughData(length)) {
        std::shared_lock lock(m_data_mutex);
        return Simple::Read(length);
    } else {
        while (IsReadable() && !IsEoF() && !HasEnoughData(length)) {
            std::this_thread::yield();
            std::this_thread::sleep_for(std::chrono::milliseconds(1)); // Add a small sleep to reduce CPU usage
        }
        if (IsReadable() && !IsEoF()) {
            std::shared_lock lock(m_data_mutex);
            return Simple::Read(length);
        } else {
            return StormByte::Unexpected<BufferOverflow>("Cannot read: buffer is not readable.");
        }
    }
}

bool Shared::IsReadable() const noexcept {
    return m_status.load() != Status::Error;
}

bool Shared::IsWritable() const noexcept {
    return m_status.load() == Status::Ready;
}

bool Shared::IsEoF() const noexcept {
    return m_status.load() == Status::EoF;
}
#include <StormByte/buffer/external.hxx>

using namespace StormByte::Buffer;

External::External(const class Reader& reader) noexcept
:Shared(), m_external_reader(reader.Clone()) {}

External::External(class Reader&& reader) noexcept
:Shared(), m_external_reader(reader.Move()) {}

External& External::operator<<(const Buffer::Status& status) {
	Shared::operator<<(status);
	return *this;
}

External& External::operator<<(const Simple& buffer) {
	Shared::operator<<(buffer);
	return *this;
}

External& External::operator<<(Simple&& buffer) {
	Shared::operator<<(std::move(buffer));
	return *this;
}

External& External::operator<<(const std::string& data) {
	Shared::operator<<(data);
	return *this;
}

External& External::operator<<(const Buffer::Data& data) {
	Shared::operator<<(data);
	return *this;
}

External& External::operator<<(Buffer::Data&& data) {
	Shared::operator<<(std::move(data));
	return *this;
}

External& External::operator>>(External& buffer) {
	Shared::operator>>(buffer);
	return *this;
}

void External::Reader(const class Reader& reader) noexcept {
	m_external_reader = reader.Clone();
}

void External::Reader(class Reader&& reader) noexcept {
	m_external_reader = reader.Move();
}

bool External::HasEnoughData(const std::size_t& length) {
	ReadExternalData(length);
	return Shared::HasEnoughData(length);
}

ExpectedData<BufferOverflow> External::Read(const size_t& length) const {
	const_cast<External*>(this)->ReadExternalData(length);
	return Shared::Read(length);
}

Read::Status External::Wait(const std::size_t& length) noexcept {
	if (ReadExternalData(length))
		return Read::Status::Success;
	else
		return Read::Status::Error;
}

bool External::ReadExternalData(const size_t& length) noexcept {
	while(Shared::AvailableBytes() < length) {
		auto expected_data = m_external_reader->Read(length);
		if (!expected_data) {
			operator<<(Buffer::Status::ReadOnly);
			return false;
		}

		Write(std::move(expected_data.value()));
	}
	return true;
}
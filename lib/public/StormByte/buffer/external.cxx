#include <StormByte/buffer/external.hxx>

using namespace StormByte::Buffer;

External::External(const ExternalReaderFunction& fn) noexcept
:Shared(), m_external_reader(fn) {}

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

void External::Reader(const ExternalReaderFunction& fn) noexcept {
	m_external_reader = fn;
}

size_t External::AvailableBytes() const noexcept {
	const_cast<External*>(this)->WriteExternalData();
	return Shared::AvailableBytes();
}

ExpectedData<BufferOverflow> External::Extract(const size_t& length) {
	WriteExternalData();
	return Shared::Extract(length);
}

Read::Status External::ExtractInto(const size_t& length, External& output) noexcept {
	WriteExternalData();
	return Shared::ExtractInto(length, output);
}

bool External::HasEnoughData(const std::size_t& length) const {
	const_cast<External*>(this)->WriteExternalData();
	return Shared::HasEnoughData(length);
}

ExpectedData<BufferOverflow> External::Read(const size_t& length) const {
	const_cast<External*>(this)->WriteExternalData();
	return Shared::Read(length);
}

Read::Status External::Wait(const std::size_t& length) const noexcept {
	const_cast<External*>(this)->WriteExternalData();
	return Shared::Wait(length);
}

void External::WriteExternalData() noexcept {
	auto expected_data = m_external_reader();
	if (!expected_data) {
		operator<<(Buffer::Status::ReadOnly);
	}
	else {
		operator<<(std::move(expected_data.value()));
	}
}
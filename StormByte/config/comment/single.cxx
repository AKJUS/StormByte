#include <StormByte/config/comment/single.hxx>
#include <StormByte/util/string/string.hxx>

using namespace StormByte::Config::Comment;

std::string Single::Serialize(const int&) const noexcept {
	return "#" + m_comment; // It is expected to start alreadyu indented
}

std::shared_ptr<StormByte::Config::Serializable> Single::Clone() const {
	return std::make_shared<Single>(*this);
}

std::shared_ptr<StormByte::Config::Serializable> Single::Move() {
	return std::make_shared<Single>(std::move(*this));
}
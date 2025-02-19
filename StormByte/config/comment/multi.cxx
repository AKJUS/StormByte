#include <StormByte/config/comment/multi.hxx>
#include <StormByte/util/string/string.hxx>

using namespace StormByte::Config::Comment;

std::string Multi::Serialize(const int&) const noexcept {
	// The multiline comments already have the indent
	std::stringstream ss(*this);
	std::string item;
	std::string serial = "/*";
	std::getline(ss, item);
	serial += item;
	if (!ss.eof()) {
		serial += "\n";
		while (std::getline(ss, item)) {
			serial += item; 
			if (!ss.eof()) serial += "\n";
		}
	}
	return serial + "*/";
}

std::shared_ptr<StormByte::Config::Serializable> Multi::Clone() const {
	return std::make_shared<Multi>(*this);
}

std::shared_ptr<StormByte::Config::Serializable> Multi::Move() {
	return std::make_shared<Multi>(std::move(*this));
}
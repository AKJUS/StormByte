#include <StormByte/serializable.hxx>
#include <StormByte/test_handlers.h>

#include <format>
#include <map>
#include <optional>
#include <string>
#include <vector>

int github_clang_libstdcpp_bus_error() {
	StormByte::Buffers::Simple buffer;
	
	{
		std::string str = "Hello World!";
		StormByte::Serializable<std::string> serialization(str);
		buffer << serialization.Serialize();
	}

	std::cout << "Buffer: " << std::endl << buffer.HexData();
	return 0;
}

int github_clang_libstdcpp_bus_error2() {
	StormByte::Buffers::Simple buffer1, buffer2;;
	
	{
		std::string str = "Hello World!";
		StormByte::Serializable<std::string> serialization(str);
		buffer2 << serialization.Serialize();
		buffer1 = buffer2;
	}

	std::cout << "Buffer1: " << std::endl << buffer1.HexData();
	return 0;
}

int github_clang_libstdcpp_bus_error3() {
	StormByte::Buffers::Simple buffer1, buffer2;;
	
	{
		std::string str = "Hello World!";
		StormByte::Serializable<std::string> serialization(str);
		buffer2 = std::move(serialization.Serialize());
		buffer1 << std::move(buffer2);
	}

	std::cout << "Buffer1: " << std::endl << buffer1.HexData();
	return 0;
}

int main() {
	int result = 0;

	result += github_clang_libstdcpp_bus_error();
	result += github_clang_libstdcpp_bus_error2();
	result += github_clang_libstdcpp_bus_error3();

	if (result == 0) {
		std::cout << "All tests passed!" << std::endl;
	} else {
		std::cout << result << " tests failed." << std::endl;
	}
	return result;
}
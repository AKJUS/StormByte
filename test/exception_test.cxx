#include <StormByte/exception.hxx>

#include <iostream>

int exception_memory_test() {
	try {
		throw StormByte::Exception("Test");
	} catch (const StormByte::Exception& ex) {
		std::cout << "Expected exception with text: " << ex.what() << std::endl;
		return 0;
	}
	return 1;
}

int main() {
	int result = 0;
	result += exception_memory_test();
	return result;
}
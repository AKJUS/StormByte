#include <StormByte/exception.hxx>
#include <StormByte/system.hxx>

#include <StormByte/test_handlers.h>

using namespace StormByte::System;

int test_several_sleeps() {
	constexpr const int allowed_jitter = 20;
	int result = 0;
	try {
		auto start = std::chrono::high_resolution_clock::now();
		Sleep(std::chrono::milliseconds(100));
		auto end = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
		ASSERT_TRUE("test_several_sleeps", (duration - 100) < allowed_jitter); // In Windows sleep is lees accurate, we allow 10ms shift

		start = std::chrono::high_resolution_clock::now();
		Sleep(std::chrono::seconds(1));
		end = std::chrono::high_resolution_clock::now();
		duration = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
		ASSERT_EQUAL("test_several_sleeps", 1, duration);
	} catch (const StormByte::Exception& ex) {
		std::cerr << ex.what() << std::endl;
		result++;
	}
	RETURN_TEST("test_several_sleeps", result);
}

int main() {
	int result = 0;
		result += test_several_sleeps();

	if (result == 0) {
		std::cout << "All tests passed!" << std::endl;
	} else {
		std::cout << result << " tests failed." << std::endl;
	}
	return result;
}

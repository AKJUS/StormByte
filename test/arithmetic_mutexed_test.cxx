#include <StormByte/exception.hxx>
#include <StormByte/util/arithmetic_mutexed.hxx>
#include <StormByte/test_handlers.h>
#include <iostream>
#include <thread>

// Test basic arithmetic operations
int test_arithmetic_mutexed_operations() {
    int result = 0;
    try {
        StormByte::Util::ArithmeticMutexed<int> a1(10);
        StormByte::Util::ArithmeticMutexed<int> a2(20);

        auto a3 = a1 + a2;
        ASSERT_EQUAL("test_arithmetic_mutexed_operations", 30, *a3);

        a1 += a2;
        ASSERT_EQUAL("test_arithmetic_mutexed_operations", 30, *a1);

    } catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        result++;
    }
    RETURN_TEST("test_arithmetic_mutexed_operations", result);
}

// Test string arithmetic (concatenation)
int test_arithmetic_mutexed_string() {
    int result = 0;
    try {
        StormByte::Util::ArithmeticMutexed<std::string> s1("Hello");
        StormByte::Util::ArithmeticMutexed<std::string> s2(", World!");

        auto s3 = s1 + s2;
        ASSERT_EQUAL("test_arithmetic_mutexed_string", "Hello, World!", *s3);

        s1 += s2;
        ASSERT_EQUAL("test_arithmetic_mutexed_string", "Hello, World!", *s1);

    } catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        result++;
    }
    RETURN_TEST("test_arithmetic_mutexed_string", result);
}

// Test comparisons
int test_arithmetic_mutexed_comparisons() {
    int result = 0;
    try {
        StormByte::Util::ArithmeticMutexed<int> a1(10);
        StormByte::Util::ArithmeticMutexed<int> a2(20);

        ASSERT_TRUE("test_arithmetic_mutexed_comparisons", a1 < a2);
        ASSERT_TRUE("test_arithmetic_mutexed_comparisons", !(a1 > a2));
        ASSERT_TRUE("test_arithmetic_mutexed_comparisons", a1 != a2);
        ASSERT_TRUE("test_arithmetic_mutexed_comparisons", a2 >= a1);

    } catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        result++;
    }
    RETURN_TEST("test_arithmetic_mutexed_comparisons", result);
}

int main() {
    int result = 0;
    try {
		result += test_arithmetic_mutexed_operations();
		result += test_arithmetic_mutexed_string();
		result += test_arithmetic_mutexed_comparisons();
    } catch (const StormByte::Exception& ex) {
        std::cerr << ex.what() << std::endl;
        result++;
    }
    if (result == 0) {
        std::cout << "All tests passed!" << std::endl;
    } else {
        std::cout << result << " tests failed." << std::endl;
    }
    return result;
}
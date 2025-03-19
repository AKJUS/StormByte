#include <StormByte/exception.hxx>
#include <StormByte/util/mutexed.hxx>
#include <StormByte/test_handlers.h>
#include <thread>
#include <queue>
#include <iostream>

// Test default constructor and assignment
int test_mutexed_basic() {
    int result = 0;
    try {
        StormByte::Util::Mutexed<int> m1;
        m1 = 10;

        ASSERT_EQUAL("test_mutexed_basic", 10, *m1);

        StormByte::Util::Mutexed<int> m2(20);
        ASSERT_EQUAL("test_mutexed_basic", 20, *m2);

        m2 = m1;
        ASSERT_EQUAL("test_mutexed_basic", 10, *m2);

    } catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        result++;
    }
    RETURN_TEST("test_mutexed_basic", result);
}

// Test thread safety
int test_mutexed_thread_safety() {
    int result = 0;
    try {
        StormByte::Util::Mutexed<int> m1(0);

        auto increment = [&m1]() {
            for (int i = 0; i < 1000; ++i) {
                m1.Lock();
                *m1 += 1;
				m1.Unlock();
            }
        };

        std::thread t1(increment);
        std::thread t2(increment);
        t1.join();
        t2.join();

        ASSERT_EQUAL("test_mutexed_thread_safety", 2000, *m1);
    } catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        result++;
    }
    RETURN_TEST("test_mutexed_thread_safety", result);
}

int main() {
    int result = 0;
    try {
		result += test_mutexed_basic();
		result += test_mutexed_thread_safety();
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
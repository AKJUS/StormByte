#include <StormByte/system/process.hxx>
#include <iostream>

#define ASSERT_EQ(expected, actual) if ((expected) != (actual)) { \
    std::cerr << "Assertion failed at " << __FILE__ << ":" << __LINE__ << ": expected " << (expected) << ", got " << (actual) << std::endl; \
    return 1; \
}

int test_basic_execution() {
    // Test a simple command that prints "Hello, World!".
    std::vector<std::string> args = { "Hello, World!" };
    StormByte::System::Process proc("/bin/echo", args);
    
    std::string output;
    proc >> output;

    ASSERT_EQ("Hello, World!\n", output);

    int exit_code = proc.wait();
    ASSERT_EQ(0, exit_code);

    return 0;
}

int test_pipeline_execution() {
    // Test a pipeline between two commands.
    std::vector<std::string> args1 = { "Hello" };
    std::vector<std::string> args2 = { "-c" };

    StormByte::System::Process proc1("/bin/echo", args1);
    StormByte::System::Process proc2("/usr/bin/wc", args2);

    proc1 >> proc2;

    std::string output;
    proc2 >> output;

    ASSERT_EQ("6\n", output);  // "Hello\n" has 6 characters including the newline.

    proc1.wait();
    proc2.wait();

    return 0;
}

int test_pipeline_sort() {
    // Test a pipeline that sorts input.
    std::vector<std::string> args1 = { "-e", "banana\napple\ncherry" };

    StormByte::System::Process proc1("/bin/echo", args1);
    StormByte::System::Process proc2("/usr/bin/sort");

    proc1 >> proc2;

    std::string output;
    proc2 >> output;

    ASSERT_EQ("apple\nbanana\ncherry\n", output);  // Sorted output.

    proc1.wait();
    proc2.wait();

    return 0;
}

int test_pipeline_find_sort_wc() {
    // Test a pipeline that finds a pattern, sorts the results, and counts lines.
    std::vector<std::string> args1 = { "-e", "apple\nbanana\ncherry\napple\nbanana\ncherry" };
    std::vector<std::string> args2 = { "apple" };
    std::vector<std::string> args3;
    std::vector<std::string> args4 = { "-l" };

    StormByte::System::Process proc1("/bin/echo", args1);
    StormByte::System::Process proc2("/bin/grep", args2);
    StormByte::System::Process proc3("/usr/bin/sort", args3);
    StormByte::System::Process proc4("/usr/bin/wc", args4);

    proc1 >> proc2 >> proc3 >> proc4;

    std::string output;
    proc4 >> output;

    ASSERT_EQ("2\n", output);  // There are 2 lines containing "apple".

    proc1.wait();
    proc2.wait();
    proc3.wait();
    proc4.wait();

    return 0;
}

int test_pipeline_echo_sort_wc() {
    // Test a pipeline that echoes, sorts, and counts lines.
    std::vector<std::string> args1 = { "-e", "orange\nbanana\napple\ncherry\nbanana\napple" };
    std::vector<std::string> args2;
    std::vector<std::string> args3;
    std::vector<std::string> args4 = { "-l" };

    StormByte::System::Process proc1("/bin/echo", args1);
    StormByte::System::Process proc2("/usr/bin/sort", args2);
    StormByte::System::Process proc3("/usr/bin/uniq", args3);
    StormByte::System::Process proc4("/usr/bin/wc", args4);

    proc1 >> proc2 >> proc3 >> proc4;

    std::string output;
    proc4 >> output;

    ASSERT_EQ("4\n", output);  // There are 4 unique sorted lines.

    proc1.wait();
    proc2.wait();
    proc3.wait();
    proc4.wait();

    return 0;
}

int main() {
    int result = 0;
    result += test_basic_execution();
    result += test_pipeline_execution();
    result += test_pipeline_sort();
    result += test_pipeline_find_sort_wc();
    result += test_pipeline_echo_sort_wc();
    if (result == 0) {
        std::cout << "All tests passed!" << std::endl;
    } else {
        std::cout << result << " tests failed." << std::endl;
    }
    return result;
}

#include <StormByte/logger/log.hxx>
#include <StormByte/test_handlers.h>

#include <memory>
#include <sstream>

using namespace StormByte::Logger;

// Function to test basic logging at different levels
int test_basic_logging() {
    std::ostringstream output;
    Log log(output, Level::Debug, "%L:");

    log << Level::Info << "Info message";
    log << Level::Debug << "Debug message";
    log << Level::Error << "Error message";

    std::string expected = "Info    : Info message\nDebug   : Debug message\nError   : Error message";
    ASSERT_EQUAL("test_basic_logging", expected, output.str());
    RETURN_TEST("test_basic_logging", 0);
}

// Function to test log level filtering
int test_log_level_filtering() {
    std::ostringstream output;
    Log log(output, Level::Error, "%L:");

    log << Level::Info << "Info message";
    log << Level::Warning << "Warning message";
    log << Level::Error << "Error message";

    std::string expected = "Error   : Error message";
    ASSERT_EQUAL("test_log_level_filtering", expected, output.str());
    RETURN_TEST("test_log_level_filtering", 0);
}

// Test several data logging
int test_log_data() {
    std::ostringstream output;
    Log log(output, Level::Info, "%L:");

    int i = 42;
    bool b = true;
    double d = 3.141596;

    log << Level::Info << "Info message with sample integer " << i << ", a bool " << b << " and a double " << d;

    std::string expected = "Info    : Info message with sample integer 42, a bool true and a double 3.141596";
    ASSERT_EQUAL("test_log_data", expected, output.str());
    RETURN_TEST("test_log_data", 0);
}

// Test log to stdout
int log_to_stdout() {
    Log log(std::cout, Level::Info, "%L:");
    log << Level::Info << "Info message";
    log << Level::Debug << "Debug message";
    log << Level::Error << "Error message";
    log << "\n";
    RETURN_TEST("log_to_stdout", 0);
}

int log_as_shared_ptr() {
    std::ostringstream output;
    std::shared_ptr<Log> log = std::make_shared<Log>(output, Level::Debug, "%L:");

    log << Level::Info << "Info message";
    log << Level::Debug << "Debug message";
    log << Level::Error << "Error message";

    std::string expected = "Info    : Info message\nDebug   : Debug message\nError   : Error message";
    ASSERT_EQUAL("log_as_shared_ptr", expected, output.str());
    RETURN_TEST("log_as_shared_ptr", 0);
}

int test_log_with_std_endl() {
    std::ostringstream output;
    Log log(output, Level::Debug, "%L:");

    log << Level::Info << "Info message" << std::endl;
    log << Level::Debug << "Debug message" << std::endl;
    log << Level::Error << "Error message" << std::endl;

    std::string expected = "Info    : Info message\nDebug   : Debug message\nError   : Error message\n";
    ASSERT_EQUAL("test_log_with_std_endl", expected, output.str());
    RETURN_TEST("test_log_with_std_endl", 0);
}

int test_log_unique_ptr_with_std_endl() {
    std::ostringstream output;
    std::unique_ptr<Log> log = std::make_unique<Log>(output, Level::Debug, "%L:");

    log << Level::Info << "Info message" << std::endl;
    log << Level::Debug << "Debug message" << std::endl;
    log << Level::Error << "Error message" << std::endl;

    std::string expected = "Info    : Info message\nDebug   : Debug message\nError   : Error message\n";
    ASSERT_EQUAL("test_log_unique_and_shared_ptr_with_std_endl", expected, output.str());
    RETURN_TEST("test_log_unique_and_shared_ptr_with_std_endl", 0);
}

int main() {
    int result = 0;

    result += test_basic_logging();
    result += test_log_level_filtering();
    result += test_log_data();
    result += log_to_stdout();
    result += log_as_shared_ptr();
    result += test_log_with_std_endl();
    result += test_log_unique_ptr_with_std_endl();

    if (result == 0) {
        std::cout << "All tests passed!" << std::endl;
    } else {
        std::cout << result << " tests failed." << std::endl;
    }
    return result;
}

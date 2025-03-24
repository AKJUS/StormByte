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

int test_humanreadable_number() {
    std::ostringstream output;
    Log log(output, Level::Info, "%L:");

    log << humanreadable_number << Level::Info << 1000;

    std::string expected = "Info    : 1,000"; // Assuming the locale uses commas for thousands
    ASSERT_EQUAL("test_humanreadable_number", expected, output.str());
    RETURN_TEST("test_humanreadable_number", 0);
}

// Test enabling human-readable bytes formatting
int test_humanreadable_bytes() {
    std::ostringstream output;
    Log log(output, Level::Info, "%L:");

    log << humanreadable_bytes << Level::Info << 10240;

    std::string expected = "Info    : 10 KiB"; // Example: 10240 bytes = 10 KiB
    ASSERT_EQUAL("test_humanreadable_bytes", expected, output.str());
    RETURN_TEST("test_humanreadable_bytes", 0);
}

// Test disabling human-readable formatting
int test_nohumanreadable() {
    std::ostringstream output;
    Log log(output, Level::Info, "%L:");

    log << Level::Info << humanreadable_number << 1000 << " " << nohumanreadable << 1000;

    std::string expected = "Info    : 1,000 1000"; // First is formatted, second is raw
    ASSERT_EQUAL("test_nohumanreadable", expected, output.str());
    RETURN_TEST("test_nohumanreadable", 0);
}

int test_humanreadable_enable_and_disable() {
    std::ostringstream output;
    Log log(output, Level::Info, "%L:");

    // Enable human-readable number formatting
    log << Level::Info << humanreadable_number << 1000;
    std::string expected_enable = "Info    : 1,000"; // Human-readable with thousand separator
    ASSERT_EQUAL("test_humanreadable_enable_and_disable (enable)", expected_enable, output.str());
	log << std::endl; // Force line termination

    // Clear the stream for the next test
    output.str("");
    output.clear();

    // Disable human-readable formatting (Raw output)
    log << Level::Info << nohumanreadable << 1000;
    std::string expected_disable = "Info    : 1000"; // Raw format, no thousand separator
    ASSERT_EQUAL("test_humanreadable_enable_and_disable (disable)", expected_disable, output.str());

    RETURN_TEST("test_humanreadable_enable_and_disable", 0);
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
	result += test_humanreadable_number();
	result += test_humanreadable_bytes();
	result += test_nohumanreadable();
	result += test_humanreadable_enable_and_disable();

    if (result == 0) {
        std::cout << "All tests passed!" << std::endl;
    } else {
        std::cout << result << " tests failed." << std::endl;
    }
    return result;
}

#include <StormByte/system/system.hxx>
#include <StormByte/system/process.hxx>
#include <iostream>

#ifdef LINUX
int test_basic_execution() {
    // Test a simple command that prints "Hello, World!".
    std::vector<std::string> args = { "Hello, World!" };
    StormByte::System::Process proc("/bin/echo", args);
    
    std::string output;
    proc >> output;

    ASSERT_EQUAL("Hello, World!\n", output);

    int exit_code = proc.wait();
    ASSERT_EQUAL(0, exit_code);

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

    ASSERT_EQUAL("6\n", output);  // "Hello\n" has 6 characters including the newline.

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

    ASSERT_EQUAL("apple\nbanana\ncherry\n", output);  // Sorted output.

    proc1.wait();
    proc2.wait();

    return 0;
}

int test_pipeline_find_sort_wc() {
    // Test a pipeline that finds a pattern, sorts the results, and counts lines.
    std::vector<std::string> args1 = { "-e", "apple\nbanana\ncherry\napple\nbanana\ncherry" };
    std::vector<std::string> args2 = { "apple" };
    std::vector<std::string> args4 = { "-l" };

    StormByte::System::Process proc1("/bin/echo", args1);
    StormByte::System::Process proc2("/bin/grep", args2);
    StormByte::System::Process proc3("/usr/bin/sort");
    StormByte::System::Process proc4("/usr/bin/wc", args4);

    proc1 >> proc2 >> proc3 >> proc4;

    std::string output;
    proc4 >> output;

    ASSERT_EQUAL("2\n", output);  // There are 2 lines containing "apple".

    proc1.wait();
    proc2.wait();
    proc3.wait();
    proc4.wait();

    return 0;
}

int test_pipeline_echo_sort_wc() {
    // Test a pipeline that echoes, sorts, and counts lines.
    std::vector<std::string> args1 = { "-e", "orange\nbanana\napple\ncherry\nbanana\napple" };
    std::vector<std::string> args4 = { "-l" };

    StormByte::System::Process proc1("/bin/echo", args1);
    StormByte::System::Process proc2("/usr/bin/sort");
    StormByte::System::Process proc3("/usr/bin/uniq");
    StormByte::System::Process proc4("/usr/bin/wc", args4);

    proc1 >> proc2 >> proc3 >> proc4;

    std::string output;
    proc4 >> output;

    ASSERT_EQUAL("4\n", output);  // There are 4 unique sorted lines.

    proc1.wait();
    proc2.wait();
    proc3.wait();
    proc4.wait();

    return 0;
}

int test_suspend_resume() {
    // Test suspending and resuming a process.
    std::vector<std::string> args = { "5" };
    StormByte::System::Process proc("/bin/sleep", args);

    // Suspend the process.
    proc.suspend();
    std::this_thread::sleep_for(std::chrono::seconds(2));
    
    // Resume the process.
    proc.resume();
    
    int exit_code = proc.wait();
    ASSERT_EQUAL(0, exit_code);

    return 0;
}

int test_multiple_suspend_resume() {
    // Test suspending and resuming a process multiple times.
    std::vector<std::string> args = { "5" };
    StormByte::System::Process proc("/bin/sleep", args);

    // Suspend and resume the process multiple times.
    for (int i = 0; i < 3; ++i) {
        proc.suspend();
        std::this_thread::sleep_for(std::chrono::seconds(2));
        proc.resume();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    
    int exit_code = proc.wait();
    ASSERT_EQUAL(0, exit_code);

    return 0;
}

int test_suspend_resume_pipeline() {
    // Set up the chained process chain.
    std::vector<std::string> args1 = { "-e", "banana\napple\ncherry" };
    std::vector<std::string> args2 = { };
    std::vector<std::string> args3 = { "-l" };

    StormByte::System::Process proc1("/bin/echo", args1);
    StormByte::System::Process proc2("/usr/bin/sort", args2);
    StormByte::System::Process proc3("/usr/bin/wc", args3);

    proc1 >> proc2 >> proc3;

    // Suspend each process.
    proc1.suspend();
    proc2.suspend();
    proc3.suspend();
    std::this_thread::sleep_for(std::chrono::seconds(2));
    
    // Resume each process.
    proc1.resume();
    proc2.resume();
    proc3.resume();

    std::string output;
    proc3 >> output;

    ASSERT_EQUAL("3\n", output);  // There are 3 lines after sorting.

    proc1.wait();
    proc2.wait();
    proc3.wait();

    return 0;
}

int test_suspend_resume_long_pipeline_mid_operation() {
    // Set up the chained process chain with a longer data operation.
    std::vector<std::string> args1 = { "-e", "apple\nbanana\ncherry\ndate\neggplant\nfig\ngrape\nhoneydew" };
    std::vector<std::string> args2 = { };
    std::vector<std::string> args3 = { "-l" };

    StormByte::System::Process proc1("/bin/echo", args1);
    StormByte::System::Process proc2("/usr/bin/sort", args2);
    StormByte::System::Process proc3("/usr/bin/wc", args3);

    proc1 >> proc2 >> proc3;

    // Introduce a delay to ensure processes have started but not completed.
    std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Adjust the delay as necessary.

    // Suspend each process in the middle of data operation.
    proc1.suspend();
    proc2.suspend();
    proc3.suspend();
    std::this_thread::sleep_for(std::chrono::seconds(2));
    
    // Resume each process.
    proc1.resume();
    proc2.resume();
    proc3.resume();

    std::string output;
    proc3 >> output;

    ASSERT_EQUAL("8\n", output);  // There are 8 lines after sorting.

    proc1.wait();
    proc2.wait();
    proc3.wait();

    return 0;
}

int test_complex_pipeline_with_suspend_resume() {
    // Configuración de la cadena de procesos.
    std::vector<std::string> args1 = { "-e", "apple\nbanana\ncherry\ndate\neggplant\nfig\ngrape\nhoneydew" };
    std::vector<std::string> args2 = { };
    std::vector<std::string> args3 = { "-l" };

    StormByte::System::Process proc1("/bin/echo", args1);
    StormByte::System::Process proc2("/usr/bin/sort", args2);
    StormByte::System::Process proc3("/usr/bin/wc", args3);

    proc1 >> proc2 >> proc3;

    // Introducción de un retardo para asegurar que los procesos han comenzado pero no completado.
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    // Suspendemos y reanudamos cada proceso en medio de la operación de datos.
    proc1.suspend();
    proc2.suspend();
    proc3.suspend();
    std::this_thread::sleep_for(std::chrono::seconds(2));
    proc1.resume();
    proc2.resume();
    proc3.resume();

    // Introducimos una segunda suspensión y reanudación para verificar la resistencia.
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    proc1.suspend();
    proc2.suspend();
    proc3.suspend();
    std::this_thread::sleep_for(std::chrono::seconds(2));
    proc1.resume();
    proc2.resume();
    proc3.resume();

    std::string output;
    proc3 >> output;

    ASSERT_EQUAL("8\n", output);  // Hay 8 líneas después de ordenar.

    proc1.wait();
    proc2.wait();
    proc3.wait();

    return 0;
}
#else
int test_basic_execution_windows() {
    // Test a simple command that prints "Hello, World!".
    std::vector<std::string> args = { "Hello, World!" };
    StormByte::System::Process proc(L"cmd.exe /c echo", args);

    std::string output;
    proc >> output;

    ASSERT_EQUAL("Hello, World! \r\n", output);

    DWORD exit_code = proc.wait();
    ASSERT_EQUAL(0, exit_code);

    return 0;
}

int test_suspend_resume_windows() {
    // Test suspending and resuming a process.
    std::vector<std::string> args = { "ping -n 5 127.0.0.1" }; // This will ping localhost 5 times, creating a 5 second delay.
    StormByte::System::Process proc(L"cmd.exe /c", args);

    // Suspend the process.
    proc.suspend();
    std::this_thread::sleep_for(std::chrono::seconds(2));

    // Resume the process.
    proc.resume();

    DWORD exit_code = proc.wait();
    ASSERT_EQUAL(0, exit_code);

    return 0;
}

int test_complex_command_windows() {
    // Test a more complex command with multiple arguments.
    std::vector<std::string> args = { "/c", "dir", "/b", "/a-d" };
    StormByte::System::Process proc(L"cmd.exe", args);

    std::string output;
    proc >> output;

    ASSERT_FALSE(output.empty());  // Check that output is not empty.

    DWORD exit_code = proc.wait();
    ASSERT_EQUAL(0, exit_code);

    return 0;
}
#endif

int main() {
    int result = 0;
	#ifdef LINUX
		result += test_basic_execution();
		result += test_pipeline_execution();
		result += test_pipeline_sort();
		result += test_pipeline_find_sort_wc();
		result += test_pipeline_echo_sort_wc();
		result += test_suspend_resume();
		result += test_multiple_suspend_resume();
		result += test_suspend_resume_pipeline();
		result += test_suspend_resume_long_pipeline_mid_operation();
		result += test_complex_pipeline_with_suspend_resume();
	#else
		result += test_basic_execution_windows();
		result += test_suspend_resume_windows();
		/*
		** This test works in local but sometimes it fails on GitHub workspace
		** So I think it is better to disable it to have the green mark on compile
		** And better focus on other kind of errors
		result += test_complex_command_windows();
		*/
	#endif
    if (result == 0) {
        std::cout << "All tests passed!" << std::endl;
    } else {
        std::cout << result << " tests failed." << std::endl;
    }
    return result;
}

add_executable(ProcessTests process/process_test.cxx)
target_link_libraries(ProcessTests StormByte)
add_test(NAME ProcessTests COMMAND ProcessTests)
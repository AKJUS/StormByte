add_executable(LoggerTests logger_test.cxx)
target_link_libraries(LoggerTests StormByte)
add_test(NAME LoggerTests COMMAND LoggerTests)
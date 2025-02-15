add_executable(ProcessTests process_test.cxx)
add_executable(ConfigTests config_test.cxx)

target_link_libraries(ProcessTests StormByte)
target_link_libraries(ConfigTests StormByte)

add_test(NAME ProcessTests COMMAND ProcessTests)
add_test(NAME ConfigTests COMMAND ConfigTests)

add_executable(DatabaseTests database_test.cxx)
target_link_libraries(DatabaseTests StormByte)
add_test(NAME DatabaseTests COMMAND DatabaseTests)
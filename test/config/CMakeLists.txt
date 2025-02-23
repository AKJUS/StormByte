add_executable(ConfigTests config_test.cxx)
target_link_libraries(ConfigTests StormByte)
add_test(NAME ConfigTests COMMAND ConfigTests)

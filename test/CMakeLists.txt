option(ENABLE_TEST "Enable Unit Tests" OFF)
if(ENABLE_TEST)
	enable_testing()

	add_executable(StringTests string_test.cxx)
	target_link_libraries(StringTests StormByte)
	add_test(NAME StringTests COMMAND StringTests)
endif()
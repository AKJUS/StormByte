option(ENABLE_TEST "Enable Unit Tests" OFF)
if(ENABLE_TEST)
	enable_testing()
	
	# Process Tests
	add_executable(ProcessTests process_test.cxx)
	target_link_libraries(ProcessTests StormByte)
	add_test(NAME ProcessTests COMMAND ProcessTests)

	# Config Tests
	add_executable(ConfigTests config_test.cxx)
	target_link_libraries(ConfigTests StormByte)
	add_test(NAME ConfigTests COMMAND ConfigTests)

	# Database Tests
	add_executable(DatabaseTests database_test.cxx)
	target_link_libraries(DatabaseTests StormByte)
	add_test(NAME DatabaseTests COMMAND DatabaseTests)

	# Logger Tests
	add_executable(LoggerTests logger_test.cxx)
	target_link_libraries(LoggerTests StormByte)
	add_test(NAME LoggerTests COMMAND LoggerTests)
endif()

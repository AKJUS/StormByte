option(ENABLE_TEST "Enable Unit Tests" OFF)
if(ENABLE_TEST)
	enable_testing()

	include_directories(${CMAKE_CURRENT_LIST_DIR})
	
	add_subdirectory(config)
	add_subdirectory(database)
	add_subdirectory(logger)
	add_subdirectory(system)
endif()

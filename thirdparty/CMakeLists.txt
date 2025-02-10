include(CMakeDependentOption)

# Default is off to be used as subproject
option(ENABLE_SQLITE							"Enable SQLite"					ON)
cmake_dependent_option(WITH_SYSTEM_SQLITE		"Use system sqlite3 library"	OFF ENABLE_SQLITE ON)

# If this is used as subproject then we force some things
if (STORMBYTE_AS_SUBPROJECT OR ENABLE_SQLITE)
	set(STORMBYTE_ENABLE_SQLITE ON)
else()
	set(STORMBYTE_ENABLE_SQLITE OFF)
endif()

add_subdirectory(sqlite3)
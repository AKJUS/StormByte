cmake_minimum_required(VERSION 3.12)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project("StormByte"
	VERSION 0.0.2
	DESCRIPTION "The conveninence StormByte C++ library"
	HOMEPAGE_URL "https://github.com/StormBytePP/StormByte"
)

set(STORMBYTE_DIR ${PROJECT_SOURCE_DIR})
set(STORMBYTE_CMAKE_DIR ${PROJECT_SOURCE_DIR}/cmake)
list(APPEND CMAKE_MODULE_PATH ${STORMBYTE_CMAKE_DIR})

if(MSVC)
	add_compile_options(/wd4251 /EHsc)
	add_compile_definitions(UNICODE) # Create problems if disabled
else()
	set(CMAKE_CXX_FLAGS_DEBUG "-pipe -g -ggdb -Wall -Wextra -Wnon-virtual-dtor -pedantic -pedantic-errors -O0")
endif()

add_subdirectory(thirdparty)
add_subdirectory(StormByte)

option(ENABLE_TEST "Enable Unit Tests" OFF)
if(ENABLE_TEST) # This is internal
	enable_testing()
	add_subdirectory(test)
endif()

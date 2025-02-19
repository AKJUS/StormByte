include(GNUInstallDirs)
set(STORMBYTE_SOURCES
	${STORMBYTE_DIR}/StormByte/exception.cxx
	${STORMBYTE_DIR}/StormByte/config/comment.cxx
	${STORMBYTE_DIR}/StormByte/config/comment/multi.cxx
	${STORMBYTE_DIR}/StormByte/config/comment/single.cxx
	${STORMBYTE_DIR}/StormByte/config/config.cxx
	${STORMBYTE_DIR}/StormByte/config/container.cxx
	${STORMBYTE_DIR}/StormByte/config/exception.cxx
	${STORMBYTE_DIR}/StormByte/config/group.cxx
	${STORMBYTE_DIR}/StormByte/config/item.cxx
	${STORMBYTE_DIR}/StormByte/config/list.cxx
	${STORMBYTE_DIR}/StormByte/config/serializable.cxx
	${STORMBYTE_DIR}/StormByte/database/sqlite/exception.cxx
	${STORMBYTE_DIR}/StormByte/database/sqlite/prepared_stmt.cxx
	${STORMBYTE_DIR}/StormByte/database/sqlite/result.cxx
	${STORMBYTE_DIR}/StormByte/database/sqlite/row.cxx
	${STORMBYTE_DIR}/StormByte/database/sqlite/sqlite3.cxx
	${STORMBYTE_DIR}/StormByte/log/logger.cxx
	${STORMBYTE_DIR}/StormByte/system/exception.cxx
	${STORMBYTE_DIR}/StormByte/system/pipe.cxx
	${STORMBYTE_DIR}/StormByte/system/process.cxx
	${STORMBYTE_DIR}/StormByte/system/variable.cxx
	${STORMBYTE_DIR}/StormByte/system/system.cxx
	${STORMBYTE_DIR}/StormByte/util/string/string.cxx
)

if (NOT STORMBYTE_AS_SUBPROJECT OR WIN32)
	add_library(StormByte SHARED ${STORMBYTE_SOURCES})
else()
	add_library(StormByte STATIC ${STORMBYTE_SOURCES})
endif()
set_target_properties(StormByte PROPERTIES
	LINKER_LANGUAGE CXX
	SOVERSION		${CMAKE_PROJECT_VERSION}
	VERSION 		${CMAKE_PROJECT_VERSION}
)

if (NOT WIN32)
	target_compile_options(StormByte PRIVATE -fvisibility=hidden $<$<COMPILE_LANGUAGE:CXX>:-fvisibility-inlines-hidden>)
endif()

if (STORMBYTE_AS_SUBPROJECT)
	target_include_directories(StormByte
		SYSTEM INTERFACE ${CMAKE_CURRENT_LIST_DIR}
	)
endif()

target_include_directories(StormByte
	PRIVATE ${CMAKE_BINARY_DIR}/
	PRIVATE ${CMAKE_BINARY_DIR}/generated
	PRIVATE ${STORMBYTE_DIR}
)
target_include_directories(StormByte SYSTEM BEFORE
	PRIVATE ${CMAKE_BINARY_DIR}/generated
	PUBLIC ${STORMBYTE_DIR}
)

target_sources(StormByte PRIVATE ${STORMBYTE_SQLITE_SOURCES})
target_link_libraries(StormByte PUBLIC sqlite3)

# Install
if (WIN32 OR NOT STORMBYTE_AS_SUBPROJECT)
	install(TARGETS StormByte
		ARCHIVE 		DESTINATION "${CMAKE_INSTALL_LIBDIR}"
		LIBRARY 		DESTINATION "${CMAKE_INSTALL_LIBDIR}"
		RUNTIME 		DESTINATION ${CMAKE_INSTALL_BINDIR}
	)
	if (NOT WIN32)
		install(DIRECTORY ${STORMBYTE_DIR}/StormByte
			DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
			FILES_MATCHING
			PATTERN "*.h"
			PATTERN "*.hxx"
		)
		install(FILES ${CMAKE_BINARY_DIR}/generated/StormByte.h 			DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/StormByte")
		install(FILES ${STORMBYTE_CMAKE_DIR}/FindStormByte.cmake 			DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/cmake/Modules")
	endif()
endif()


file(CONFIGURE OUTPUT ${CMAKE_BINARY_DIR}/generated/StormByte.h CONTENT "#pragma once
#define STORMBYTE_VERSION \"${${PROJECT_NAME}_VERSION}\"")

option(ENABLE_DOC "Enable documentation generation with Doxygen" OFF)
if(ENABLE_DOC)
	find_package(Doxygen REQUIRED)
	set(DOXYGEN_PROJECT_NAME "StormByte C++ Library")
	set(DOXYGEN_PROJECT_NUMBER "${PROJECT_VERSION}")
	set(DOXYGEN_PROJECT_BRIEF "StormByte is a comprehensive, cross-platform C++ library aimed at easing system programming, configuration management, logging, and database handling tasks. This library provides a unified API that abstracts away the complexities and inconsistencies of different platforms (Windows, Linux).")
	set(DOXYGEN_OUTPUT_DIRECTORY "${STORMBYTE_DIR}/docs")
	set(DOXYGEN_USE_MDFILE_AS_MAINPAGE "${STORMBYTE_DIR}/README.md")
	set(DOXYGEN_EXCLUDE_PATTERNS
		"*/CMakeLists.txt"
		"*/.git/*"
		"*/doc/*"
		"*/thirdparty/*"
		"*/build/*"
		"*/*.cxx"
	)
	set(DOXYGEN_GENERATE_HTML YES)
	set(DOXYGEN_HTML_OUTPUT ".")
	set(DOXYGEN_GENERATE_MAN NO)
	set(DOXYGEN_GENERATE_LATEX NO)

	doxygen_add_docs(
		StormByteDoc
		${STORMBYTE_DIR}
		COMMENT "Generate HTML pages"
		WORKING_DIRECTORY "${STORMBYTE_DIR}"
	)
endif()
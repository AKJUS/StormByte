include(GNUInstallDirs)
set(STORMBYTE_SOURCES
	${STORMBYTE_DIR}/StormByte/config/exception.cxx
	${STORMBYTE_DIR}/StormByte/config/file.cxx
	${STORMBYTE_DIR}/StormByte/config/item.cxx
	${STORMBYTE_DIR}/StormByte/config/item/group.cxx
	${STORMBYTE_DIR}/StormByte/config/item/value.cxx
	${STORMBYTE_DIR}/StormByte/config/item/value/double.cxx
	${STORMBYTE_DIR}/StormByte/config/item/value/integer.cxx
	${STORMBYTE_DIR}/StormByte/config/item/value/string.cxx
	${STORMBYTE_DIR}/StormByte/log/file.cxx
	${STORMBYTE_DIR}/StormByte/log/logger.cxx
	${STORMBYTE_DIR}/StormByte/system/exception.cxx
	${STORMBYTE_DIR}/StormByte/system/pipe.cxx
	${STORMBYTE_DIR}/StormByte/system/process.cxx
	${STORMBYTE_DIR}/StormByte/system/variable.cxx
	${STORMBYTE_DIR}/StormByte/system/worker.cxx
)

# SQLite support
set(STORMBYTE_SQLITE_SOURCES
	${STORMBYTE_DIR}/StormByte/database/sqlite/exception.cxx
	${STORMBYTE_DIR}/StormByte/database/sqlite/prepared_stmt.cxx
	${STORMBYTE_DIR}/StormByte/database/sqlite/result.cxx
	${STORMBYTE_DIR}/StormByte/database/sqlite/row.cxx
	${STORMBYTE_DIR}/StormByte/database/sqlite/sqlite3.cxx
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
	target_compile_options(StormByte PRIVATE -fPIC -fvisibility=hidden $<$<COMPILE_LANGUAGE:CXX>:-fvisibility-inlines-hidden>)
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

# Enable features
if (ENABLE_SQLITE)
	target_sources(StormByte PRIVATE ${STORMBYTE_SQLITE_SOURCES})
	target_compile_definitions(StormByte PUBLIC STORMBYTE_ENABLE_SQLITE)
	target_link_libraries(StormByte PUBLIC sqlite3)
endif()

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
		install(FILES ${CMAKE_BINARY_DIR}/generated/StormByte/Features.h 	DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/StormByte")
		install(FILES ${STORMBYTE_CMAKE_DIR}/FindStormByte.cmake 			DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/cmake/Modules")
	endif()
endif()


file(CONFIGURE OUTPUT ${CMAKE_BINARY_DIR}/generated/StormByte.h CONTENT "#pragma once
#define STORMBYTE_VERSION \"${${PROJECT_NAME}_VERSION}\"")

file(CONFIGURE OUTPUT ${CMAKE_BINARY_DIR}/generated/StormByte/Features.h CONTENT "#pragma once
#define STORMBYTE_SQLITE ${STORMBYTE_ENABLE_SQLITE}")
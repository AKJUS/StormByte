# Look for the necessary header
find_path(StormByte++_INCLUDE_DIR NAMES "StormByte++/visibility.h")
mark_as_advanced(StormByte++_INCLUDE_DIR)

# Look for the necessary library
find_library(StormByte++_LIBRARY NAMES StormByte++)
mark_as_advanced(StormByte++_LIBRARY)

# Extract version information from the header file
if(StormByte++_INCLUDE_DIR)
	set(StormByte++_FOUND TRUE)
	set(StormByte++_INCLUDE_DIRS "${StormByte++_INCLUDE_DIR}")
	set(StormByte++_LIBRARIES "${StormByte++_LIBRARY}")
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(StormByte
    REQUIRED_VARS StormByte++_INCLUDE_DIR StormByte++_LIBRARY StormByte++_LIBRARIES
    HANDLE_COMPONENTS
)

# Create the imported target
if(StormByte++_FOUND)
    if(NOT TARGET StormByte++)
        add_library(StormByte++ UNKNOWN IMPORTED GLOBAL)
        set_target_properties(StormByte++ PROPERTIES
			IMPORTED_LOCATION 				"${StormByte_LIBRARY}"
			INTERFACE_INCLUDE_DIRECTORIES 	"${StormByte_INCLUDE_DIRS}"
			INTERFACE_LINK_LIBRARIES      	"${StormByte_LIBRARIES}"
		)
    endif()
endif()

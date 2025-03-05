# Find the main base component library and header
find_path(StormByte_INCLUDE_DIR NAMES "StormByte/visibility.h")
mark_as_advanced(StormByte_INCLUDE_DIR)
find_library(StormByte_LIBRARY NAMES StormByte)
mark_as_advanced(StormByte_LIBRARY)

if(StormByte_INCLUDE_DIR AND StormByte_LIBRARY)
    set(StormByte_FOUND TRUE)
    set(StormByte_INCLUDE_DIRS "${StormByte_INCLUDE_DIR}")
    set(StormByte_LIBRARIES "${StormByte_LIBRARY}")
else()
    set(StormByte_FOUND FALSE)
endif()

# Find the necessary headers and libraries for each optional subcomponent
include(FeatureSummary)

if(NOT StormByte_FIND_COMPONENTS)
    set(StormByte_FIND_COMPONENTS Config Database Logger System)
endif()

foreach(component IN LISTS StormByte_FIND_COMPONENTS)
    string(TOUPPER ${component} COMPONENT_UPPER)

    find_path(StormByte${component}_INCLUDE_DIR NAMES "${component}/visibility.h")
    mark_as_advanced(StormByte${component}_INCLUDE_DIR)
    find_library(StormByte${component}_LIBRARY NAMES "StormByte${component}")
    mark_as_advanced(StormByte${component}_LIBRARY)

    if(StormByte${component}_INCLUDE_DIR AND StormByte${component}_LIBRARY)
        set(StormByte${component}_FOUND TRUE)
        set(StormByte${component}_INCLUDE_DIRS "${StormByte${component}_INCLUDE_DIR}")
        set(StormByte${component}_LIBRARIES "${StormByte${component}_LIBRARY}")
    else()
        set(StormByte${component}_FOUND FALSE)
    endif()

    add_feature_info("StormByte::${component}" StormByte${component}_FOUND "Found the ${component} subcomponent of StormByte.")
endforeach()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(StormByte REQUIRED_VARS StormByte_INCLUDE_DIR StormByte_LIBRARY HANDLE_COMPONENTS)

# Create imported targets and aliases for the base and optional subcomponents
if(StormByte_FOUND)
    if(NOT TARGET StormByte)
        add_library(StormByte UNKNOWN IMPORTED GLOBAL)
        set_target_properties(StormByte PROPERTIES
            IMPORTED_LOCATION                 "${StormByte_LIBRARY}"
            INTERFACE_INCLUDE_DIRECTORIES     "${StormByte_INCLUDE_DIRS}"
        )
    endif()
endif()

foreach(component IN LISTS StormByte_FIND_COMPONENTS)
    if(StormByte${component}_FOUND)
        if(NOT TARGET StormByte${component})
            add_library(StormByte${component} UNKNOWN IMPORTED GLOBAL)
            set_target_properties(StormByte${component} PROPERTIES
                IMPORTED_LOCATION                 "${StormByte${component}_LIBRARY}"
                INTERFACE_INCLUDE_DIRECTORIES     "${StormByte${component}_INCLUDE_DIRS}"
            )
        endif()
		target_link_libraries(StormByte PUBLIC StormByte${component})
        add_library(StormByte::${component} ALIAS StormByte${component})
    endif()
endforeach()

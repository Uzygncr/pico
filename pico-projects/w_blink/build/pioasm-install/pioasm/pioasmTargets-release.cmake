#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "pioasm" for configuration "Release"
set_property(TARGET pioasm APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(pioasm PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/pioasm/pioasm"
  )

list(APPEND _IMPORT_CHECK_TARGETS pioasm )
list(APPEND _IMPORT_CHECK_FILES_FOR_pioasm "${_IMPORT_PREFIX}/pioasm/pioasm" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)

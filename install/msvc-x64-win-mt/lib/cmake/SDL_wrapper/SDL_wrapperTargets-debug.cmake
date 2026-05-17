#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "SDL_wrapper::graphics" for configuration "Debug"
set_property(TARGET SDL_wrapper::graphics APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(SDL_wrapper::graphics PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "CXX"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/SDL_wrapper-graphics-d.lib"
  )

list(APPEND _cmake_import_check_targets SDL_wrapper::graphics )
list(APPEND _cmake_import_check_files_for_SDL_wrapper::graphics "${_IMPORT_PREFIX}/lib/SDL_wrapper-graphics-d.lib" )

# Import target "SDL_wrapper::audio" for configuration "Debug"
set_property(TARGET SDL_wrapper::audio APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(SDL_wrapper::audio PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "CXX"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/SDL_wrapper-audio-d.lib"
  )

list(APPEND _cmake_import_check_targets SDL_wrapper::audio )
list(APPEND _cmake_import_check_files_for_SDL_wrapper::audio "${_IMPORT_PREFIX}/lib/SDL_wrapper-audio-d.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)

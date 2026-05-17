#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "SDL_wrapper::Core" for configuration "Debug"
set_property(TARGET SDL_wrapper::Core APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(SDL_wrapper::Core PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "CXX"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/SDL_wrapper-Core-d.lib"
  )

list(APPEND _cmake_import_check_targets SDL_wrapper::Core )
list(APPEND _cmake_import_check_files_for_SDL_wrapper::Core "${_IMPORT_PREFIX}/lib/SDL_wrapper-Core-d.lib" )

# Import target "SDL_wrapper::Graphics" for configuration "Debug"
set_property(TARGET SDL_wrapper::Graphics APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(SDL_wrapper::Graphics PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "CXX"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/SDL_wrapper-Graphics-d.lib"
  )

list(APPEND _cmake_import_check_targets SDL_wrapper::Graphics )
list(APPEND _cmake_import_check_files_for_SDL_wrapper::Graphics "${_IMPORT_PREFIX}/lib/SDL_wrapper-Graphics-d.lib" )

# Import target "SDL_wrapper::Mixer" for configuration "Debug"
set_property(TARGET SDL_wrapper::Mixer APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(SDL_wrapper::Mixer PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "CXX"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/SDL_wrapper-Mixer-d.lib"
  )

list(APPEND _cmake_import_check_targets SDL_wrapper::Mixer )
list(APPEND _cmake_import_check_files_for_SDL_wrapper::Mixer "${_IMPORT_PREFIX}/lib/SDL_wrapper-Mixer-d.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)

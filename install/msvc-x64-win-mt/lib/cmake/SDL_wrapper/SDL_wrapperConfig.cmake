
####### Expanded from @PACKAGE_INIT@ by configure_package_config_file() #######
####### Any changes to this file will be overwritten by the next CMake run ####
####### The input file was SDL_wrapperConfig.cmake.in                            ########

get_filename_component(PACKAGE_PREFIX_DIR "${CMAKE_CURRENT_LIST_DIR}/../../../" ABSOLUTE)

macro(set_and_check _var _file)
  set(${_var} "${_file}")
  if(NOT EXISTS "${_file}")
    message(FATAL_ERROR "File or directory ${_file} referenced by variable ${_var} does not exist !")
  endif()
endmacro()

macro(check_required_components _NAME)
  foreach(comp ${${_NAME}_FIND_COMPONENTS})
    if(NOT ${_NAME}_${comp}_FOUND)
      if(${_NAME}_FIND_REQUIRED_${comp})
        set(${_NAME}_FOUND FALSE)
      endif()
    endif()
  endforeach()
endmacro()

####################################################################################

include(CMakeFindDependencyMacro)

set(SDL_wrapper_SUPPORTED_COMPONENTS Core Graphics Mixer)

if(NOT SDL_wrapper_FIND_COMPONENTS)
    set(SDL_wrapper_FIND_COMPONENTS Core Graphics Mixer)
endif()

find_dependency(SDL3 CONFIG)

if(Graphics IN_LIST SDL_wrapper_FIND_COMPONENTS)
    find_dependency(SDL3_image CONFIG)
endif()

if(Mixer IN_LIST SDL_wrapper_FIND_COMPONENTS)
    find_dependency(SDL3_mixer CONFIG)
endif()

include("${CMAKE_CURRENT_LIST_DIR}/SDL_wrapperTargets.cmake")

foreach(component IN LISTS SDL_wrapper_FIND_COMPONENTS)
    if(NOT component IN_LIST SDL_wrapper_SUPPORTED_COMPONENTS)
        set(SDL_wrapper_FOUND FALSE)
        set(SDL_wrapper_NOT_FOUND_MESSAGE "Unsupported SDL_wrapper component: ${component}")
        return()
    endif()

    if(TARGET SDL_wrapper::${component})
        set(SDL_wrapper_${component}_FOUND TRUE)
    else()
        set(SDL_wrapper_${component}_FOUND FALSE)
    endif()
endforeach()

check_required_components(SDL_wrapper)

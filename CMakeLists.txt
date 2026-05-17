cmake_minimum_required(VERSION 3.25)

project(SDL_wrapper VERSION 0.1.0 LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)
include(GenerateExportHeader)

find_package(SDL3 CONFIG REQUIRED)
find_package(SDL3_image CONFIG REQUIRED)
find_package(SDL3_mixer CONFIG REQUIRED)

if(TARGET SDL3::SDL3-static)
    set(SDL_WRAPPER_SDL3_TARGET SDL3::SDL3-static)
else()
    set(SDL_WRAPPER_SDL3_TARGET SDL3::SDL3)
endif()

set(SDL_WRAPPER_CORE_FILES
    src/Clock.cpp
    src/Colors.cpp
    src/FileWorker.cpp
)

set(SDL_WRAPPER_GRAPHICS_FILES
    src/CircleShape.cpp
    src/EllipseShape.cpp
    src/PolygonShape.cpp
    src/RectangleShape.cpp
    src/RendererRegistry.cpp
    src/RenderTarget.cpp
    src/RenderWindow.cpp
    src/Shape.cpp
    src/Sprite.cpp
    src/Texture.cpp
    src/Transformable.cpp
    src/VideoMode.cpp
    src/View.cpp
)

set(SDL_WRAPPER_MIXER_FILES
    src/Audio.cpp
    src/AudioDevice.cpp
    src/MixerRegistry.cpp
    src/Sound.cpp
    src/SoundSettings.cpp
)

set(SDL_WRAPPER_TARGETS
    SDL_wrapper-Core
    SDL_wrapper-Graphics
    SDL_wrapper-Mixer
)

function(sdl_wrapper_generate_export_header target macro_name export_header_subdir)
    set(export_header
        "${CMAKE_CURRENT_BINARY_DIR}/generated/include/SDL_wrapper/${export_header_subdir}/Export.hpp"
    )

    generate_export_header(${target}
        EXPORT_FILE_NAME "${export_header}"
        EXPORT_MACRO_NAME "${macro_name}_EXPORT"
        NO_EXPORT_MACRO_NAME "${macro_name}_NO_EXPORT"
        DEPRECATED_MACRO_NAME "${macro_name}_DEPRECATED"
        NO_DEPRECATED_MACRO_NAME "${macro_name}_NO_DEPRECATED"
        STATIC_DEFINE "${macro_name}_STATIC_DEFINE"
        INCLUDE_GUARD_NAME "${macro_name}_EXPORT_HPP"
    )

    target_include_directories(${target} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/generated/include>
    )

    target_sources(${target} PUBLIC
        FILE_SET generated_headers
        TYPE HEADERS
        BASE_DIRS "${CMAKE_CURRENT_BINARY_DIR}/generated/include"
        FILES "${export_header}"
    )
endfunction()

function(sdl_wrapper_setup_target target export_name)
    set_target_properties(${target} PROPERTIES
        CXX_STANDARD 23
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
        DEBUG_POSTFIX "-d"
        EXPORT_NAME ${export_name}
        FOLDER "Libraries"
    )

    target_include_directories(${target} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    )
endfunction()

add_library(SDL_wrapper-Core ${SDL_WRAPPER_CORE_FILES})
add_library(SDL_wrapper-Graphics ${SDL_WRAPPER_GRAPHICS_FILES})
add_library(SDL_wrapper-Mixer ${SDL_WRAPPER_MIXER_FILES})

add_library(SDL_wrapper::Core ALIAS SDL_wrapper-Core)
add_library(SDL_wrapper::Graphics ALIAS SDL_wrapper-Graphics)
add_library(SDL_wrapper::Mixer ALIAS SDL_wrapper-Mixer)

sdl_wrapper_setup_target(SDL_wrapper-Core Core)
sdl_wrapper_setup_target(SDL_wrapper-Graphics Graphics)
sdl_wrapper_setup_target(SDL_wrapper-Mixer Mixer)

sdl_wrapper_generate_export_header(SDL_wrapper-Core SDL_WRAPPER_CORE Core)
sdl_wrapper_generate_export_header(SDL_wrapper-Graphics SDL_WRAPPER_GRAPHICS Graphics)
sdl_wrapper_generate_export_header(SDL_wrapper-Mixer SDL_WRAPPER_MIXER Mixer)

target_link_libraries(SDL_wrapper-Core PUBLIC
    ${SDL_WRAPPER_SDL3_TARGET}
)

target_link_libraries(SDL_wrapper-Graphics PUBLIC
    SDL_wrapper-Core
    SDL3_image::SDL3_image
)

target_link_libraries(SDL_wrapper-Mixer PUBLIC
    SDL_wrapper-Core
    SDL3_mixer::SDL3_mixer
)

install(TARGETS ${SDL_WRAPPER_TARGETS}
    EXPORT SDL_wrapperTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}

    FILE_SET generated_headers
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/"
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(EXPORT SDL_wrapperTargets
    FILE SDL_wrapperTargets.cmake
    NAMESPACE SDL_wrapper::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/SDL_wrapper
)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/SDL_wrapperConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/SDL_wrapperConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/SDL_wrapperConfig.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/SDL_wrapper
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/SDL_wrapperConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/SDL_wrapperConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/SDL_wrapper
)

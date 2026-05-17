cmake_minimum_required(VERSION 3.25)

project(SDL_wrapper VERSION 0.1.0 LANGUAGES CXX)

# ===================================================
# ==================== НАСТРОЙКА ====================
# ===================================================

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

set(CPP_GRAPHICS_FILES
    src/CircleShape.cpp
    src/Clock.cpp
    src/Colors.cpp
    src/EllipseShape.cpp
    src/FileWorker.cpp
    src/PolygonShape.cpp
    src/RectangleShape.cpp
    src/RenderTarget.cpp
    src/RenderWindow.cpp
    src/SDL3GlobalMeneger.cpp
    src/Shape.cpp
    src/Sprite.cpp
    src/Texture.cpp
    src/Transformable.cpp
    src/VideoMode.cpp
    src/View.cpp
)

set(CPP_AUDIO_FILES
    src/Audio.cpp
    src/AudioDevice.cpp
    src/Sound.cpp
    src/SoundSettings.cpp
    src/SDL3MixerMeneger.cpp
)

set(SDL_WRAPPER_TARGETS
    SDL_wrapper-graphics
    SDL_wrapper-audio
)

# ===================================================
# ============ ПОДКЛЮЧЕНИЕ ЗАВИСИМОСТЕЙ =============
# ===================================================

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)
include(GenerateExportHeader)

function(SDL_wrapper_generate_export_header target macro_name export_header_subdir)
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

find_package(SDL3 CONFIG REQUIRED)
find_package(SDL3_image CONFIG REQUIRED)
find_package(SDL3_mixer CONFIG REQUIRED)

# ===================================================
# =============== СОЗДАНИЕ БИНАРНИКОВ ===============
# ===================================================

add_library(SDL_wrapper-graphics ${CPP_GRAPHICS_FILES})
add_library(SDL_wrapper-audio ${CPP_AUDIO_FILES})

SDL_wrapper_generate_export_header(SDL_wrapper-graphics SDL_WRAPPER_GRAPHICS graphics)
SDL_wrapper_generate_export_header(SDL_wrapper-audio SDL_WRAPPER_AUDIO audio)

add_library(SDL_wrapper::Graphics ALIAS SDL_wrapper-graphics)
add_library(SDL_wrapper::Audio ALIAS SDL_wrapper-audio)

# ===================================================
# ================= TARGET НАСТРОЙКА ================
# ===================================================

# =================== ОБЩИЙ TARGET ==================

foreach(target ${SDL_WRAPPER_TARGETS})
    set_target_properties(${target} PROPERTIES
        CXX_STANDARD 23
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
        DEBUG_POSTFIX "-d"
    )
    target_include_directories(${target} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    )
    string(REPLACE "SDL_wrapper-" "" SHORT_NAME ${target})
    set_target_properties(${target} PROPERTIES EXPORT_NAME ${SHORT_NAME})
    set_target_properties(${target} PROPERTIES FOLDER "Libraries")
endforeach()

if(TARGET SDL3::SDL3-static)
    target_link_libraries(SDL_wrapper-graphics PUBLIC
        SDL3::SDL3-static
        SDL3_image::SDL3_image
    )
else()
    target_link_libraries(SDL_wrapper-graphics PUBLIC
        SDL3::SDL3
        SDL3_image::SDL3_image
    )
endif()

target_link_libraries(SDL_wrapper-audio PUBLIC SDL3_mixer::SDL3_mixer)

# ================== TARGET МОДУЛЕЙ =================

target_link_libraries(SDL_wrapper-audio PUBLIC SDL_wrapper-graphics)

# ===================================================
# ================ INSTALL БИБЛИОТЕКИ ===============
# ===================================================

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
include(FetchContent)

if(CMAKE_BUILD_TYPE MATCHES "Release|MinSizeRel")
    include(cmake/AndroidOptimizationFlags.cmake)
endif()



########################Link FreeType###############################
FetchContent_Declare(
        freetype
        GIT_REPOSITORY https://gitlab.freedesktop.org/freetype/freetype.git
        GIT_TAG VER-2-13-2
)
FetchContent_MakeAvailable(freetype)

add_library(Freetype::Freetype ALIAS freetype)
########################End FreeType###############################



########################Link RmlUi###############################
set(RMLUI_DIR "D:/dev/libraries/RmlUi-6.1")
set(BUILD_SHARED_LIBS        OFF CACHE BOOL "Build shared libraries")

add_subdirectory(${RMLUI_DIR} ${CMAKE_BINARY_DIR}/RmlUi)
########################End RmlUi###############################



########################Link SDL3###############################
find_package(SDL3          REQUIRED CONFIG)
find_package(SDL3_image    REQUIRED CONFIG)
########################End RmlUi###############################



########################Final###############################
add_library(${EXECUTABLE_NAME} SHARED
        ${SRC_DIR}/main.cpp
        ${EXTERNAL_DIR}/RmlUi/Backends/RmlUi_Platform_SDL.cpp
        ${EXTERNAL_DIR}/RmlUi/Backends/RmlUi_Renderer_SDL.cpp
)

target_sources(${EXECUTABLE_NAME} PRIVATE
        ${SRC_DIR}/SDLWrapper/RenderWindow.cpp
        ${SRC_DIR}/SDLWrapper/VideoMode.cpp
)
#target_sources(${EXECUTABLE_NAME} PRIVATE
#  ${RMLUI_DIR}/Backends/RmlUi_Platform_SDL.cpp
#  ${RMLUI_DIR}/Backends/RmlUi_Renderer_SDL.cpp
#)

#add_library(${EXECUTABLE_NAME} SHARED
#        ${SRC_DIR}/main.cpp
#        ${EXTERNAL_DIR}/RmlUi/Backends/RmlUi_Platform_SDL.cpp
#        ${EXTERNAL_DIR}/RmlUi/Backends/RmlUi_Renderer_SDL.cpp
#)

target_compile_definitions(${EXECUTABLE_NAME} PRIVATE RMLUI_SDL_VERSION_MAJOR=3 SDL_MAIN_USE_CALLBACKS)
target_include_directories(${EXECUTABLE_NAME} PRIVATE
        ${EXTERNAL_DIR}/SDL3_gfx
        ${RMLUI_DIR}/Backends
        ${SRC_DIR}
)
target_sources(${EXECUTABLE_NAME} PRIVATE
        ${EXTERNAL_DIR}/SDL3_gfx/SDL3_gfx/SDL3_framerate.c
        ${EXTERNAL_DIR}/SDL3_gfx/SDL3_gfx/SDL3_gfxPrimitives.c
        ${EXTERNAL_DIR}/SDL3_gfx/SDL3_gfx/SDL3_imageFilter.c
        ${EXTERNAL_DIR}/SDL3_gfx/SDL3_gfx/SDL3_rotozoom.c
)

if(CMAKE_BUILD_TYPE MATCHES "Release|MinSizeRel")
        include(cmake/AndroidTargetOptimization.cmake)
endif()

target_link_libraries(${EXECUTABLE_NAME}
        PRIVATE
        android
        log
        freetype
        RmlUi::RmlUi
        SDL3::SDL3
        SDL3_image::SDL3_image
)

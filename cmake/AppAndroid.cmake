set(RmlUi_DIR "${EXTERNAL_DIR}/rmlui/${CMAKE_ANDROID_ARCH_ABI}/${CMAKE_BUILD_TYPE}/lib/cmake/RmlUi")
set(Freetype_DIR "${EXTERNAL_DIR}/FreeType/${CMAKE_ANDROID_ARCH_ABI}/${CMAKE_BUILD_TYPE}/lib/cmake/freetype")

if(CMAKE_BUILD_TYPE MATCHES "Release|MinSizeRel")
    include(cmake/AndroidOptimizationFlags.cmake)
endif()

find_package(SDL3_image CONFIG REQUIRED)
find_package(SDL3 CONFIG REQUIRED COMPONENTS SDL3-static)
find_package(Freetype CONFIG REQUIRED)
find_package(RmlUi CONFIG REQUIRED)
find_package(ZLIB REQUIRED)

add_library(${EXECUTABLE_NAME} SHARED ${SOURCE_CPP})

target_include_directories(${EXECUTABLE_NAME} PRIVATE ${INCLUDE_PATHS})
target_compile_definitions(${EXECUTABLE_NAME} PRIVATE
  RMLUI_SDL_VERSION_MAJOR=3
  RMLUI_STATIC_LIB
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
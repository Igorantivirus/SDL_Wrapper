cmake_minimum_required(VERSION 3.22.1)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project("SDL3_Wrapper" LANGUAGES CXX C)

set(EXECUTABLE_NAME ${PROJECT_NAME})
set(SRC_DIR "${PROJECT_SOURCE_DIR}/src")
set(THIRD_PARTY_BUILD_DIR "${CMAKE_BINARY_DIR}/_3rdparty")

set(SOURCE_CPP
    ${SRC_DIR}/main.cpp
    ${SRC_DIR}/SDL3_gfx/SDL3_framerate.c
    ${SRC_DIR}/SDL3_gfx/SDL3_framerate.h
    ${SRC_DIR}/SDL3_gfx/SDL3_gfxPrimitives.c
    ${SRC_DIR}/SDL3_gfx/SDL3_gfxPrimitives.h
    ${SRC_DIR}/SDL3_gfx/SDL3_gfxPrimitives_font.h
    ${SRC_DIR}/SDL3_gfx/SDL3_imageFilter.c
    ${SRC_DIR}/SDL3_gfx/SDL3_imageFilter.h
    ${SRC_DIR}/SDL3_gfx/SDL3_rotozoom.c
    ${SRC_DIR}/SDL3_gfx/SDL3_rotozoom.h

)
set(INCLUDE_PATHS
    ${SRC_DIR}
)

if(NOT DEFINED EXTERNAL_DIR)
    message(ERROR EXTERNAL_DIR is not defined)
endif()

if(ANDROID)
    include(cmake/AppAndroid.cmake)
elseif(WIN32)
    include(cmake/AppWindows.cmake)
endif()
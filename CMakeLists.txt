cmake_minimum_required(VERSION 3.22.1)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project("Android-SDL3-Example" LANGUAGES CXX C)

set(EXECUTABLE_NAME ${PROJECT_NAME})
set(SRC_DIR "${PROJECT_SOURCE_DIR}/src")
set(THIRD_PARTY_BUILD_DIR "${CMAKE_BINARY_DIR}/_3rdparty")

if(NOT DEFINED EXTERNAL_DIR)
    message(ERROR EXTERNAL_DIR is not defined)
endif()

set(SOURCE_CPP
    ${SRC_DIR}/main.cpp
)
set(INCLUDE_PATHS
    ${SRC_DIR}
)

if(ANDROID)
    include(cmake/AppAndroid.cmake)
elseif(WIN32)
    include(cmake/AppWindows.cmake)
endif()
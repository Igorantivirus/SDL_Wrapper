cmake_minimum_required(VERSION 3.22.1)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project("Android-SDL3-Example" LANGUAGES CXX C)

set(EXECUTABLE_NAME ${PROJECT_NAME})
set(SRC_DIR "${PROJECT_SOURCE_DIR}/src")
set(INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include")
set(THIRD_PARTY_BUILD_DIR "${CMAKE_BINARY_DIR}/_3rdparty")

if(NOT DEFINED EXTERNAL_DIR)
    message(ERROR EXTERNAL_DIR is not defined)
endif()

set(SOURCE_CPP
    ${SRC_DIR}/main.cpp
    
    ${SRC_DIR}/CircleShape.cpp
    ${SRC_DIR}/Convert.cpp
    ${SRC_DIR}/EllipseShape.cpp
    ${SRC_DIR}/Log.cpp
    ${SRC_DIR}/Matrix3x3.cpp
    ${SRC_DIR}/Operators.cpp
    ${SRC_DIR}/RectangleShape.cpp
    ${SRC_DIR}/RenderMeneger.cpp
    ${SRC_DIR}/RenderTarget.cpp
    ${SRC_DIR}/RenderWindow.cpp
    ${SRC_DIR}/Shape.cpp
    ${SRC_DIR}/Sprite.cpp
    ${SRC_DIR}/Texture.cpp
    ${SRC_DIR}/Transformable.cpp
    ${SRC_DIR}/VectorMath.cpp
    ${SRC_DIR}/VideoMode.cpp
    ${SRC_DIR}/View.cpp
)
set(INCLUDE_PATHS
    ${INCLUDE_DIR}
)

if(ANDROID)
    include(cmake/AppAndroid.cmake)
elseif(WIN32)
    include(cmake/AppWindows.cmake)
endif()

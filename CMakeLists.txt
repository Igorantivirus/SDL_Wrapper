cmake_minimum_required(VERSION 3.20)

project(SDLWrapper 
    VERSION 0.1.0 
    LANGUAGES C CXX
)

# Только статическая библиотека
add_library(SDLWrapper STATIC)

set_target_properties(SDLWrapper PROPERTIES 
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    POSITION_INDEPENDENT_CODE ON
    DEBUG_POSTFIX "-d"
)

# Все исходные файлы
target_sources(SDLWrapper PRIVATE
    src/CircleShape.cpp
    src/Colors.cpp
    src/EllipseShape.cpp
    src/RectangleShape.cpp
    src/RenderMeneger.cpp
    src/RenderTarget.cpp
    src/RenderWindow.cpp
    src/Shape.cpp
    src/Sprite.cpp
    src/Texture.cpp
    src/Transformable.cpp
    src/VideoMode.cpp
    src/View.cpp
    src/Clock.cpp
)

# Подключение заголовков (пути переданы из пресетов)
target_include_directories(SDLWrapper 
    PUBLIC 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        "${SDL3_INC_DIR}"
        "${SDL3_IMAGE_INC_DIR}"
)

# --- Инсталляция ---
include(GNUInstallDirs)

install(TARGETS SDLWrapper
    EXPORT SDLWrapperTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(EXPORT SDLWrapperTargets
    FILE SDLWrapperConfig.cmake
    NAMESPACE SDLWrapper::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/SDLWrapper
)

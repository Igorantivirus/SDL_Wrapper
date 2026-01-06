cmake_minimum_required(VERSION 3.20)

# 1. Настройка проекта
project(SDLWrapper 
    VERSION 1.0.0 
    LANGUAGES CXX
)

# Поддержка C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT DEFINED CMAKE_MSVC_RUNTIME_LIBRARY)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

# Опция для типа сборки (Static/Shared)
option(BUILD_SHARED_LIBS "Build SDLWrapper as a shared library" OFF)

# Определение суффикса для имени библиотеки в зависимости от типа сборки
if(BUILD_SHARED_LIBS)
    set(LIB_TYPE_NAME "shared")
    add_library(SDLWrapper SHARED)
else()
    set(LIB_TYPE_NAME "static")
    add_library(SDLWrapper STATIC)
endif()

# Устанавливаем итоговое имя файла (например, SDLWrapper-static.lib)
set_target_properties(SDLWrapper PROPERTIES 
    OUTPUT_NAME "SDLWrapper-${LIB_TYPE_NAME}"
    DEBUG_POSTFIX "d"
)

# 2. Исходные файлы
target_sources(SDLWrapper PRIVATE
    src/CircleShape.cpp
    src/Convert.cpp
    src/EllipseShape.cpp
    src/Log.cpp
    src/Matrix3x3.cpp
    src/Operators.cpp
    src/RectangleShape.cpp
    src/RenderMeneger.cpp
    src/RenderTarget.cpp
    src/RenderWindow.cpp
    src/Shape.cpp
    src/Sprite.cpp
    src/Texture.cpp
    src/Transformable.cpp
    src/VectorMath.cpp
    src/VideoMode.cpp
    src/View.cpp
)

# 3. Зависимости (SDL3)
# Мы используем QUIET, так как установка SDL лежит на конечном пользователе,
# но нам нужны заголовки для компиляции самой обертки.
find_package(SDL3 CONFIG REQUIRED COMPONENTS)
find_package(SDL3_image REQUIRED)

# Подключаем зависимости к нашей цели
# PUBLIC означает, что тот, кто линкует SDLWrapper, тоже получит SDL3 в include
target_link_libraries(SDLWrapper PUBLIC 
    SDL3::SDL3 
    SDL3_image::SDL3_image
)

# Настройка путей include
target_include_directories(SDLWrapper 
    PUBLIC 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# 4. Логика установки (Installation)

include(GNUInstallDirs)

# Определяем архитектуру для пути установки
if(ANDROID)
    set(ARCH_DIR "${ANDROID_ABI}")
elseif(MSVC)
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(ARCH_DIR "msvc-x64-win")
    else()
        set(ARCH_DIR "msvc-x86-win")
    endif()
else()
    set(ARCH_DIR "${CMAKE_SYSTEM_PROCESSOR}")
endif()

# Пути для экспорта конфига CMake
set(CONFIG_INSTALL_DIR "${CMAKE_INSTALL_LIBDIR}/cmake/SDLWrapper")

# Установка бинарников и библиотек
# В Ninja Multi-Config путь будет дополнен конфигурацией (Debug/Release) автоматически при установке
install(TARGETS SDLWrapper
    EXPORT SDLWrapperTargets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

# Установка заголовков
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# Генерируем файл целей для find_package
install(EXPORT SDLWrapperTargets
    FILE SDLWrapperTargets.cmake
    NAMESPACE SDLWrapper::
    DESTINATION ${CONFIG_INSTALL_DIR}
)

# Создаем файл конфига SDLWrapperConfig.cmake
include(CMakePackageConfigHelpers)
configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/SDLWrapperConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/SDLWrapperConfig.cmake"
    INSTALL_DESTINATION ${CONFIG_INSTALL_DIR}
)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/SDLWrapperConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/SDLWrapperConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/SDLWrapperConfigVersion.cmake"
    DESTINATION ${CONFIG_INSTALL_DIR}
)
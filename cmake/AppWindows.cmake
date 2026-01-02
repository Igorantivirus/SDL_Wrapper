set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
set(RmlUi_DIR "${EXTERNAL_DIR}/rmlui/msvc_x64_win/Debug/lib/cmake/RmlUi")
set(Freetype_DIR "${EXTERNAL_DIR}/FreeType/msvc_x64_win/Debug/lib/cmake/freetype")

add_executable(${EXECUTABLE_NAME} ${SOURCE_CPP})

target_include_directories(${EXECUTABLE_NAME} PRIVATE ${INCLUDE_PATHS})

find_package(SDL3_image CONFIG REQUIRED)
find_package(SDL3 CONFIG REQUIRED COMPONENTS SDL3-static)
find_package(Freetype CONFIG REQUIRED)
find_package(RmlUi CONFIG REQUIRED)

target_compile_definitions(${EXECUTABLE_NAME} PRIVATE
  RMLUI_SDL_VERSION_MAJOR=3
  RMLUI_STATIC_LIB
  SDL_MAIN_USE_CALLBACKS
)

target_link_libraries(${EXECUTABLE_NAME} PRIVATE
    SDL3::SDL3-static
    SDL3_image::SDL3_image-static
    Freetype::Freetype
    RmlUi::RmlUi
)

if(NOT USE_CONSOLE)
  if(MSVC)
    target_link_options(${EXECUTABLE_NAME} PRIVATE "/SUBSYSTEM:WINDOWS")
  elseif(MINGW)
    target_link_options(${EXECUTABLE_NAME} PRIVATE "-mwindows")
  endif()
endif()
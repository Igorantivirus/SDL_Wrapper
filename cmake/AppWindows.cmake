set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

add_executable(${EXECUTABLE_NAME} ${SOURCE_CPP})

target_include_directories(${EXECUTABLE_NAME} PRIVATE ${INCLUDE_PATHS})

find_package(SDL3_image CONFIG REQUIRED)
find_package(SDL3 CONFIG REQUIRED COMPONENTS SDL3-static)
# find_package(RmlUi CONFIG REQUIRED)
# find_package(pugixml CONFIG REQUIRED)

# target_compile_definitions(${EXECUTABLE_NAME} PRIVATE
#   RMLUI_SDL_VERSION_MAJOR=3
#   RMLUI_STATIC_LIB
# )

target_link_libraries(${EXECUTABLE_NAME} PRIVATE
    SDL3::SDL3-static
    SDL3_image::SDL3_image-static
    # RmlUi::RmlUi
    # pugixml::pugixml
)

if(NOT USE_CONSOLE)
  if(MSVC)
    target_link_options(${EXECUTABLE_NAME} PRIVATE "/SUBSYSTEM:WINDOWS")
  elseif(MINGW)
    target_link_options(${EXECUTABLE_NAME} PRIVATE "-mwindows")
  endif()
endif()
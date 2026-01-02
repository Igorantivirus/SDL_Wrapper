function(generate_executable TARGET_NAME)
    if(ANDROID)
        add_library(${TARGET_NAME} SHARED ${ARGN})
    else()
        add_executable(${TARGET_NAME} ${ARGN})
    endif()
endfunction()

function(set_abi OUT_VAR)
    # 1) Android ABI всегда берём из toolchain/NDK
    if(ANDROID)
        if(NOT DEFINED CMAKE_ANDROID_ARCH_ABI OR CMAKE_ANDROID_ARCH_ABI STREQUAL "")
            message(FATAL_ERROR "CMAKE_ANDROID_ARCH_ABI is empty (Android toolchain not set?)")
        endif()
        set(_abi "${CMAKE_ANDROID_ARCH_ABI}")
    else()
        # 2) Если пользователь уже задал PROJECT_ABI (или OUT_VAR) в cache/CLI — используем его
        #    Проверяем и cache, и обычную переменную.
        if(DEFINED ${OUT_VAR} AND NOT "${${OUT_VAR}}" STREQUAL "")
            set(_abi "${${OUT_VAR}}")
        elseif(DEFINED PROJECT_ABI AND NOT "${PROJECT_ABI}" STREQUAL "")
            set(_abi "${PROJECT_ABI}")
        else()
            # 3) Иначе — берём дефолт (можно передать как второй аргумент)
            if(ARGC GREATER 1 AND NOT "${ARGV1}" STREQUAL "")
                set(_abi "${ARGV1}")
            else()
                # Авто-детект дефолта
                # Архитектура (упрощённо)
                if(CMAKE_SIZEOF_VOID_P EQUAL 8)
                    set(_arch "x64")
                else()
                    set(_arch "x86")
                endif()

                if(WIN32)
                    if(MSVC)
                        set(_abi "msvc_${_arch}_win")
                    elseif(MINGW)
                        set(_abi "mingw_${_arch}_win")
                    else()
                        set(_abi "win_${_arch}")
                    endif()
                elseif(APPLE)
                    # если хочешь, можно учитывать CMAKE_OSX_ARCHITECTURES
                    set(_abi "mac_${_arch}")
                elseif(UNIX)
                    set(_abi "linux_${_arch}")
                else()
                    message(FATAL_ERROR "set_abi: unsupported platform")
                endif()
            endif()
        endif()
    endif()

    # Сделаем ещё и cache переменную для удобства пользователя (чтобы видел/мог менять)
    set(PROJECT_ABI "${_abi}" CACHE STRING "ABI folder name for prebuilt deps (e.g. msvc_x64_win)" FORCE)

    # Вернём наружу
    set(${OUT_VAR} "${_abi}" PARENT_SCOPE)
endfunction()

function(set_target_console TARGET_NAME USE_CONSOLE)
    if(NOT TARGET "${TARGET_NAME}")
        message(FATAL_ERROR "set_target_console: target '${TARGET_NAME}' not found")
    endif()

    if(NOT WIN32)
        return()
    endif()

    get_target_property(_type "${TARGET_NAME}" TYPE)
    if(NOT _type STREQUAL "EXECUTABLE")
        return() # на Android/библиотеках просто ничего не делаем
    endif()

    set_target_properties("${TARGET_NAME}" PROPERTIES
        WIN32_EXECUTABLE $<NOT:$<BOOL:${USE_CONSOLE}>>
    )
endfunction()

function(resolve_build_type OUT_VAR CURRENT_BUILD_TYPE)
    if(NOT DEFINED CURRENT_BUILD_TYPE OR "${CURRENT_BUILD_TYPE}" STREQUAL "")
        set(_result "Release")
    elseif("${CURRENT_BUILD_TYPE}" STREQUAL "MinSizeRel")
        set(_result "Release")
    else()
        set(_result "${CURRENT_BUILD_TYPE}")
    endif()

    set(${OUT_VAR} "${_result}" PARENT_SCOPE)
endfunction()
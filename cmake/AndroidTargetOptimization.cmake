
target_compile_options(${EXECUTABLE_NAME} PRIVATE
        -Oz
        -ffunction-sections -fdata-sections
        -fvisibility=hidden -fvisibility-inlines-hidden
        -fmerge-all-constants
        -fomit-frame-pointer
)
target_link_options(${EXECUTABLE_NAME} PRIVATE
        -Wl,--gc-sections
        -Wl,--as-needed
        -Wl,--strip-all
        -Wl,--icf=all
        -Wl,--exclude-libs,ALL
        -Wl,--hash-style=gnu
        -Wl,--pack-dyn-relocs=android+relr
)

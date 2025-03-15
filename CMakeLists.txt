cmake_minimum_required(VERSION 3.10)
project(ASDP_GUI C)

# Tell CMake we're building for Windows with MinGW.
set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_PROCESSOR x86_64)
set(CMAKE_C_COMPILER x86_64-w64-mingw32-gcc)

# Force static linking if you have static libs available.
set(CMAKE_EXE_LINKER_FLAGS "-static -static-libgcc -static-libstdc++ ${CMAKE_EXE_LINKER_FLAGS}")

# Paths to your local SDL2 and SDL2_image
set(SDL2_INCLUDE_DIR       "${CMAKE_SOURCE_DIR}/third_party/SDL2/include")
set(SDL2_LIB_DIR           "${CMAKE_SOURCE_DIR}/third_party/SDL2/lib")

set(SDL2_IMAGE_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/third_party/SDL2_image/include")
set(SDL2_IMAGE_LIB_DIR     "${CMAKE_SOURCE_DIR}/third_party/SDL2_image/lib")

# Add your source files here
add_executable(asdp_gui
    main.c
    # e.g. play_menu.c
    # e.g. leaderboards_menu.c
    # ...
)

# If you have nuklear.h in third_party/nuklear:
target_include_directories(asdp_gui PRIVATE
    "${CMAKE_SOURCE_DIR}/third_party/nuklear"
)

# Include SDL2 + SDL2_image
target_include_directories(asdp_gui PRIVATE
    "${SDL2_INCLUDE_DIR}"
    "${SDL2_IMAGE_INCLUDE_DIR}"
)

# Link with MinGW libs + SDL2 + SDL2_image
target_link_libraries(asdp_gui PRIVATE
    -L"${SDL2_LIB_DIR}"
    -L"${SDL2_IMAGE_LIB_DIR}"
    -lmingw32
    -lSDL2main
    -lSDL2
    -lSDL2_image
    -mwindows
    -lm
    -lsetupapi
    -lwinmm
    -limm32
    -lole32
    -loleaut32
    -lversion
    -lcfgmgr32
)

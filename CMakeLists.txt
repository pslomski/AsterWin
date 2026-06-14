cmake_minimum_required(VERSION 3.22.1)

project(Asteroids
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(project_warnings INTERFACE)
target_compile_options(project_warnings INTERFACE
    -Wall
    -Wextra
    -pedantic
    -Werror
    -O3
    -m64
    -Wno-expansion-to-defined
    -Wno-format-security
)

include_directories(src)

include(CMakePrintHelpers)
cmake_print_variables(CMAKE_MODULE_PATH)

find_package(OpenGL REQUIRED)

if(NOT DEFINED BASS_DIR)
    if(WIN32)
        set(BASS_DIR d:/dev/Bass)
    else()
        set(BASS_DIR /mnt/d/dev/bass24-linux)
    endif()
endif()
find_package(BASS REQUIRED)
include_directories(${BASS_INCLUDE_DIRS})

include(FetchContent)

# Build all dependencies statically so the game ships without their DLLs.
# (BASS is closed-source and DLL-only, so bass.dll is the one exception.)
set(BUILD_SHARED_LIBS OFF)
set(SDL_SHARED OFF CACHE BOOL "" FORCE)
set(SDL_STATIC ON CACHE BOOL "" FORCE)
set(SDL_TEST OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG        release-3.2.14
)
FetchContent_MakeAvailable(SDL3)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.14.0
)
FetchContent_MakeAvailable(googletest)
enable_testing()

set(ASTER_EXE Asteroids)
add_executable(${ASTER_EXE} WIN32)
add_subdirectory(src)
target_link_libraries(${ASTER_EXE}
  PRIVATE
    project_warnings
    OpenGL::GL
    SDL3::SDL3-static
    ${BASS_LIBRARIES}
)

# Link the C/C++ runtime statically so the exe does not depend on
# libstdc++-6.dll / libwinpthread-1.dll / libgcc_s_seh-1.dll at runtime
# (this is what removes the nanosleep64 loader error for good).
target_link_options(${ASTER_EXE} PRIVATE -static)

if(WIN32)
    # BASS is closed-source and ships only as a DLL, so copy bass.dll next to the
    # executable. It is the sole remaining bundled DLL (besides system opengl32).
    add_custom_command(TARGET ${ASTER_EXE} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${BASS_DIR}/lib/x64/bass.dll" "$<TARGET_FILE_DIR:${ASTER_EXE}>"
        VERBATIM
    )
endif()

include(GoogleTest)

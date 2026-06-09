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

set(BASS_DIR d:/dev/Bass)
#set(BASS_DIR /mnt/d/dev/bass24-linux)
find_package(BASS REQUIRED)
include_directories(${BASS_INCLUDE_DIRS})

include(FetchContent)

FetchContent_Declare(
    SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG        release-3.2.14
)
FetchContent_MakeAvailable(SDL3)

set(SDL3TTF_VENDORED ON CACHE BOOL "" FORCE)
set(SDL3TTF_HARFBUZZ OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
    SDL3_ttf
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git
    GIT_TAG        release-3.2.2
)
FetchContent_MakeAvailable(SDL3_ttf)

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
    SDL3::SDL3
    SDL3_ttf::SDL3_ttf
    ${BASS_LIBRARIES}
)

include(GoogleTest)

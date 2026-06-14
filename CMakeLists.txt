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

include(FetchContent)

# Build all dependencies statically so the game ships without any third-party DLLs.
set(BUILD_SHARED_LIBS OFF)
set(SDL_SHARED OFF CACHE BOOL "" FORCE)
set(SDL_STATIC ON CACHE BOOL "" FORCE)
set(SDL_TEST OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG        release-3.4.10
)
FetchContent_MakeAvailable(SDL3)

# SDL3_mixer for sound effects. Only Ogg Vorbis is needed, decoded by the
# bundled stb_vorbis (no external libs/submodules), so disable every other
# codec and skip submodule fetching to keep the build self-contained.
set(SDLMIXER_VENDORED OFF CACHE BOOL "" FORCE)
set(SDLMIXER_DEPS_SHARED OFF CACHE BOOL "" FORCE)
set(SDLMIXER_VORBIS_STB ON CACHE BOOL "" FORCE)
set(SDLMIXER_VORBIS_VORBISFILE OFF CACHE BOOL "" FORCE)
set(SDLMIXER_WAVE ON CACHE BOOL "" FORCE)
set(SDLMIXER_FLAC OFF CACHE BOOL "" FORCE)
set(SDLMIXER_MP3 OFF CACHE BOOL "" FORCE)
set(SDLMIXER_MOD OFF CACHE BOOL "" FORCE)
set(SDLMIXER_MIDI OFF CACHE BOOL "" FORCE)
set(SDLMIXER_OPUS OFF CACHE BOOL "" FORCE)
set(SDLMIXER_GME OFF CACHE BOOL "" FORCE)
set(SDLMIXER_WAVPACK OFF CACHE BOOL "" FORCE)
set(SDLMIXER_AIFF OFF CACHE BOOL "" FORCE)
set(SDLMIXER_VOC OFF CACHE BOOL "" FORCE)
set(SDLMIXER_AU OFF CACHE BOOL "" FORCE)
set(SDLMIXER_TESTS OFF CACHE BOOL "" FORCE)
set(SDLMIXER_SAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
    SDL3_mixer
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_mixer.git
    GIT_TAG        release-3.2.4
    GIT_SUBMODULES ""
)
FetchContent_MakeAvailable(SDL3_mixer)

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
    SDL3_mixer::SDL3_mixer-static
)

# Link the C/C++ runtime statically so the exe does not depend on
# libstdc++-6.dll / libwinpthread-1.dll / libgcc_s_seh-1.dll at runtime
# (this is what removes the nanosleep64 loader error for good). With every
# dependency static, the game ships as a single self-contained executable.
target_link_options(${ASTER_EXE} PRIVATE -static)

include(GoogleTest)

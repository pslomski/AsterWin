cmake_minimum_required(VERSION 3.22.1)

project(Asteroids
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options(-Wall)
add_compile_options(-Wextra)
add_compile_options(-pedantic)
add_compile_options(-Werror)
add_compile_options(-O1)
add_compile_options(-m64)
add_compile_options(-Wno-expansion-to-defined)
add_compile_options(-Wno-format-security)

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
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.14.0
)
FetchContent_MakeAvailable(googletest)
enable_testing()
  
set(ASTER_EXE Asteroids)
add_executable(${ASTER_EXE})
add_subdirectory(src)
target_link_libraries(${ASTER_EXE}
  PRIVATE
    OpenGL::GL
    ${BASS_LIBRARIES}
)

include(GoogleTest)

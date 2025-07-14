function(add_gtest test_name)
    set(options)
    set(oneValueArgs)
    set(multiValueArgs SOURCES)
    cmake_parse_arguments(ADD_GTEST "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    add_executable(${test_name} ${ADD_GTEST_SOURCES})

    target_compile_definitions(${test_name} PRIVATE UNIT_TEST)

    target_link_libraries(${test_name}
      PRIVATE
        OpenGL::GL #TODO: Remove dependency to OpenGL in tests
        GTest::gtest_main
        GTest::gmock_main
    )

    add_test(NAME ${test_name} COMMAND ${test_name})
endfunction()

target_sources(${ASTER_EXE}
  PRIVATE
    Asteroids.cpp
)

add_subdirectory(game)
add_subdirectory(ui)
add_subdirectory(utils)

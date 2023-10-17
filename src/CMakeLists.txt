target_sources(${ASTER_EXE}
  PRIVATE
    Asteroids.cpp
)

add_subdirectory(game)
add_subdirectory(ui)
add_subdirectory(utils)

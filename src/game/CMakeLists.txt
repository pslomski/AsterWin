add_subdirectory(geom)
add_subdirectory(objects)
add_subdirectory(types)
add_subdirectory(ut)

target_sources(${ASTER_EXE}
    PRIVATE
      Game.cpp
      GameArea.cpp
      ScoreCounter.cpp
      Time.cpp
)

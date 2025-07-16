add_subdirectory(geom)
add_subdirectory(objects)
add_subdirectory(ut)

target_sources(${ASTER_EXE}
  PRIVATE
    Asteroid.cpp
    Bonuses.cpp
    Bullet.cpp
    Game.cpp
    ScoreCounter.cpp
    Ship.cpp
    Ufo.cpp
    World.cpp
)

add_subdirectory(ut)

target_sources(${ASTER_EXE}
  PRIVATE
    Asteroid.cpp
    Bonuses.cpp
    Bullet.cpp
    Game.cpp
    Object.cpp
    ObjectSound.cpp
    ScoreCounter.cpp
    Ship.cpp
    Sound.cpp
    Ufo.cpp
    World.cpp
)

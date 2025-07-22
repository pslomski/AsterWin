add_subdirectory(ut)

target_sources(${ASTER_EXE}
  PRIVATE
    Asteroid.cpp
    BonusAddBullets.cpp
    BonusBulletSpeed.cpp
    BonusObject.cpp
    BonusPoints.cpp
    Bonuses.cpp  
    Bullet.cpp
    Object.cpp
    PowerUp.cpp
    PowerUpAddBullets.cpp
    Ship.cpp
    Ufo.cpp    
    TempObject.cpp
  )

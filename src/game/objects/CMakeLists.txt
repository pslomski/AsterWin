add_subdirectory(ut)

target_sources(${ASTER_EXE}
  PRIVATE
    Asteroid.cpp
    BonusAddBullets.cpp
    BonusBulletSpeed.cpp
    BonusObject.cpp
    Bonuses.cpp  
    Bullet.cpp
    Object.cpp
    Ship.cpp
    Ufo.cpp    
    TempObject.cpp
  )

add_subdirectory(ut)

target_sources(${ASTER_EXE}
  PRIVATE
    PointInPolygon.cpp
    Settings.cpp
    TimeInterval.cpp
    Tools.cpp
)


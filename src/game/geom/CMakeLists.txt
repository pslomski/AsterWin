add_subdirectory(ut)

target_sources(${ASTER_EXE}
  PRIVATE
    LineIntersection.cpp
    PointInPolygon.cpp
)

add_subdirectory(ut)

target_sources(${ASTER_EXE}
    PRIVATE
        Bounds.cpp
        Distance.cpp
        LineIntersection.cpp
        PointInPolygon.cpp
        PolygWithPointCheck.cpp
)

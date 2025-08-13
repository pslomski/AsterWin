add_subdirectory(ut)

target_sources(${ASTER_EXE}
    PRIVATE
        Bounds.cpp
        CheckCollision.cpp
        Distance.cpp
        LineIntersection.cpp
        PointInPolygon.cpp
        PolygWithPointCheck.cpp
        Transform.cpp
)

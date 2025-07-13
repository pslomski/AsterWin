add_gtest(TestScoreCounter
    SOURCES
        TestScoreCounter.cpp
        ../ScoreCounter.cpp
)

add_gtest(TestObject
    SOURCES
        TestObject.cpp
        ../Object.cpp
        ../World.cpp #TODO: Remove dependency to World.cpp in tests
        ../../utils/PointInPolygon.cpp #TODO: Remove dependency to PointInPolygon.cpp in tests
        ../../utils/TimeInterval.cpp #TODO: Remove dependency to TimeInterval.cpp in tests
        ../../utils/Tools.cpp #TODO: Remove dependency to Tools.cpp in tests
)

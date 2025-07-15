add_gtest(TestObject
  SOURCES
    ../../World.cpp #TODO: Remove dependency to World.cpp in tests
    ../../geom/PointInPolygon.cpp #TODO: Remove dependency to PointInPolygon.cpp in tests
    ../../../utils/Tools.cpp #TODO: Remove dependency to Tools.cpp in tests
)

add_gtest(TestTempObject)

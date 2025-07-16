add_subdirectory(ut)

target_sources(${ASTER_EXE}
  PRIVATE
    TimeInterval.cpp
    Tools.cpp
)

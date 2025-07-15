add_subdirectory(ut)

target_sources(${ASTER_EXE}
  PRIVATE
    Object.cpp
    TempObject.cpp
  )

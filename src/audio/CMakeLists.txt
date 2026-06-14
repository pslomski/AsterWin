target_sources(${ASTER_EXE}
  PRIVATE
    AudioLib.cpp
    SfxController.cpp
    SfxSample.cpp
    SfxSamples.cpp
)

add_subdirectory(ut)

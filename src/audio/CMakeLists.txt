target_sources(${ASTER_EXE}
  PRIVATE
    AudioLib.cpp
    SfxController.cpp
    SfxSample.cpp
    SfxSamples.cpp
    Sound.cpp
)

add_subdirectory(ut)

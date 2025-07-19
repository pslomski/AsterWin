target_sources(${ASTER_EXE}
  PRIVATE
    AudioLib.cpp
    ObjectSound.cpp
    SfxController.cpp
    SfxSamples.cpp
    Sound.cpp
)

add_subdirectory(ut)

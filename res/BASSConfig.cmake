set(BASS_INCLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}")

# Support both 32 and 64 bit builds
if (${CMAKE_SIZEOF_VOID_P} MATCHES 8)
  set(BASS_LIBRARIES "${CMAKE_CURRENT_LIST_DIR}/libs/x86_64/libbass.so")
else ()
  set(BASS_LIBRARIES "${CMAKE_CURRENT_LIST_DIR}/libs/x86/libbass.so")
endif ()

string(STRIP "${BASS_LIBRARIES}" BASS_LIBRARIES)

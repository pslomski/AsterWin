target_sources(${ASTER_EXE}
  PRIVATE
    Application.cpp
    Font.cpp
    MainWindow.cpp
    Settings.cpp
    TextControl.cpp
)

add_subdirectory(gstate)
add_subdirectory(ut)

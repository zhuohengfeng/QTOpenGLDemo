add_executable(06_LightingMaps main.cpp glwindow.cpp)
target_link_libraries(06_LightingMaps
        Common
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::OpenGL)
add_executable(07_LightCasters main.cpp glwindow.cpp)
target_link_libraries(07_LightCasters
        Common
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::OpenGL
        )

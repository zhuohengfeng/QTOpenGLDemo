add_executable(05_Light main.cpp glwindow.cpp)
target_link_libraries(05_Light
        Common
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::OpenGL)
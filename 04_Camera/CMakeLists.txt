add_executable(04_Camera main.cpp glwindow.cpp)
target_link_libraries(04_Camera
        Common
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::OpenGL)
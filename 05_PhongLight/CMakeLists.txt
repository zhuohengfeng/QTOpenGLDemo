add_executable(05_PhongLight main.cpp glwindow.cpp)
target_link_libraries(05_PhongLight
        Common
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::OpenGL)
add_executable(03_MVP main.cpp glwindow.cpp)
target_link_libraries(03_MVP
        Common
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::OpenGL
        )
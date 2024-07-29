add_executable(11_FrameBuffer main.cpp glwindow.cpp)
target_link_libraries(11_FrameBuffer
        Common
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::OpenGL
        )

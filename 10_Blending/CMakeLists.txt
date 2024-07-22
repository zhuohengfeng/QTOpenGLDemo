add_executable(10_Blending main.cpp glwindow.cpp)
target_link_libraries(10_Blending
        Common
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::OpenGL
        )

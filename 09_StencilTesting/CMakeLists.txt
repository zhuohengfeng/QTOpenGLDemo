add_executable(09_StencilTesting main.cpp glwindow.cpp)
target_link_libraries(09_StencilTesting
        Common
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::OpenGL
        )

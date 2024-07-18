add_executable(02_Texture main.cpp glwindow.cpp)
target_link_libraries(02_Texture
        Common
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::OpenGL
        )

add_executable(08_Multiplelights main.cpp glwindow.cpp)
target_link_libraries(08_Multiplelights
        Common
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::OpenGL
        )

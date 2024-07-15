add_executable(02_Texture main.cpp glwindow.cpp)
target_link_libraries(02_Texture
        Common
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::OpenGL
        )


# Resources:
set(logo_resource_files
        "../resource/marker1.png"
        )

qt_add_resources(02_Texture "02_Texture"
        PREFIX
        "/"
        FILES
        ${logo_resource_files}
        )


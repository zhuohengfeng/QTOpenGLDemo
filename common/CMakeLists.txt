add_library(Common STATIC src/ffImage.cpp src/camera.cpp src/shader.cpp)
target_link_libraries(Common
        Qt::OpenGL)



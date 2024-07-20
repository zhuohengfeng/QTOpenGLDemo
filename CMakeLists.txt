cmake_minimum_required(VERSION 3.25)
project(QTOpenGLDemo)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_PREFIX_PATH "C:/Qt/6.7.2/mingw_64")

set(GLM_ROOT D:/Projects/ThirdpartyLibs/glm-0.9.9.8/glm)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/common/include)

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        OpenGL
        REQUIRED)

add_subdirectory(common)
add_subdirectory(01_Triangle)
add_subdirectory(02_Texture)
add_subdirectory(03_MVP)
add_subdirectory(04_Camera)
add_subdirectory(05_PhongLight)
add_subdirectory(06_LightingMaps)
add_subdirectory(07_LightCasters)
add_subdirectory(08_Multiplelights)
add_subdirectory(09_StencilTesting)
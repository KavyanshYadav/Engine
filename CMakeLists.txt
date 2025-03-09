cmake_minimum_required(VERSION 3.10)
project(OpenGLRenderer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

include_directories(${CMAKE_SOURCE_DIR}/include)

link_directories(${CMAKE_SOURCE_DIR}/libs)
add_definitions(-DGLFW_INCLUDE_NONE)

find_package(OpenGL REQUIRED)
file(GLOB_RECURSE SRC_FILES src/*.cpp)
set(GLAD_SOURCE "${CMAKE_SOURCE_DIR}/glad.c")
file(GLOB SOURCES "${CMAKE_SOURCE_DIR}/*.cpp" "${CMAKE_SOURCE_DIR}/src/*.cpp")

add_executable(OpenGLRenderer ${SOURCES} ${GLAD_SOURCE} ${IMGUI_SOURCES} ${SRC_FILES})

target_link_libraries(OpenGLRenderer
    glfw3
    opengl32
    gdi32
    glfw3dll
)
